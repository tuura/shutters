#if defined(__linux) || defined(__APPLE__)
	#include "eq-generator.h"
#else
	#include "Path\eq-generator.h"
#endif

// Generate wakeup marking and window conditions
int generate_wakeup_conditions() {

    create_tmp_files();

    if ( generate_marking_conditions() ) {
        fprintf(stderr, "Error on the generation of the marking conditions\n");
        remove_tmp_files();
        return -1;
    }

    if ( generate_window_conditions() ) {
        fprintf(stderr, "Error on the generation of the window wakeup conditions\n");
        remove_tmp_files();
        return -1;
    }

    remove_tmp_files();

    return 0;
}

// Generate wakeup window conditions of all the windows
int generate_window_conditions() {

    unsigned n_windows = windows.size();

    for (unsigned i = 0; i < n_windows; i++) {
        if ( generate_window_condition(i) ) {
            fprintf(stderr, "Error on generating wakeup conditions\
                             for the Window %d\n", i+1);
            return -1;
        }
    }

    return 0;
}

// Generate wakeup marking conditions of all the windows
int generate_marking_conditions() {

    unsigned n_windows = windows.size();

    for (unsigned i = 0; i < n_windows; i++) {
        if ( generate_marking_condition(i) ) {
            fprintf(stderr, "Error on generating marking conditions\
                             for the Window %d\n", i+1);
            return -1;
        }
    }

    return 0;
}

// Generate wake window condition of one window
int generate_window_condition(unsigned wi) {

    unsigned n_windows = windows.size();
    kmw.clear();

    for (unsigned i = 0; i < n_windows; i++) {
        if ( wi != i ) {
            vector<string> woken_states = find_woken_states_between_windows(wi, i);

            if ( !woken_states.empty() ) {
                espresso_format kmt;
                kmt.inputs = push_back_inputs_window(woken_states, i);
                kmt.outputs = push_back_output_window(woken_states, wi, i);
                unsigned n_states = windows[i].states.size();
                for (unsigned j = 0; j < n_states; j++) {
                    kmt.states_in.push_back(windows[i].states[j].name);
                    kmt.states_out.push_back(windows[i].states[j].name);
                }
                kmw.push_back(kmt);
            }

        }
    }

    if ( write_karnaugh_map(pla_path) ) {
        fprintf(stderr, "Error on writing the Karnaugh map\n");
        return -1;
    }

    run_espresso((char*)espresso_path.c_str(), pla_path, espresso_result_path);

    vector<string> equations = read_equations(espresso_result_path);

    cout << "Wakeup condition for Window " << wi+1 << ":" << "\n";
    if ( abc_path.empty() ) {
        unsigned n_eqs = equations.size();
        for (unsigned i = 0; i < n_eqs; i++) {
            cout << equations[i] << "\n";
        }

        return 0;
    }

    if ( refactorize_equations(equations) ) {
        fprintf(stderr, "Error refactorizing equations with ABC\n");
        return -1;
    }

    return 0;
}

// Read Espresso generated equations and modify them using the Synopsys format for ABC
vector<string> read_equations(char *file_eq) {

    vector<string> eqs;
    FILE *fp = NULL;
    char c;

    if ( (fp = fopen(file_eq, "r")) == NULL ) {
        eqs.push_back("No equations found");
        return eqs;
    }
    
    while ( (c = fgetc(fp)) != EOF ) {

        if ( c == '\n' ) {
            continue;
        }

        char *eq;
        eq = strdup("");
        eq = cat_char(eq, c);

        while ( (c = fgetc(fp)) != ';' ) {

            switch (c) {
                case '|':
                    eq = cat_char(eq, '+');
                    break;
                case '&':
                    eq = cat_char(eq, '*');
                    break;
                default:
                    eq = cat_char(eq, c);
            }
        }
        eq = cat_char(eq, c);
        eq = replace_zeros_ones(eq);

        eqs.push_back(eq);
    }

    fclose(fp);    

    return eqs;
}

// Replaces "();" with '1' and ';' with '0'
char* replace_zeros_ones(char* eq) {

    char *new_eq;
    int i = 0;

    new_eq = (char *) malloc(sizeof(char) * (strlen(eq) + 1) );

    while ( eq[i] != '=' ) {
        new_eq[i] = eq[i];
        i++;
    }
    new_eq[i++] = '=';
    new_eq[i++] = ' ';

    if ( eq[i] == '(' && eq[i+1] == ')' ) {
        new_eq[i] = '1';
        new_eq[++i] = ';';
        new_eq[++i] = '\0';
    } else if ( eq[i] == ';' ) {
        new_eq[i] = '0';
        new_eq[++i] = ';';
        new_eq[++i] = '\0';
    } else {
        free(new_eq);
        new_eq = strdup(eq);
    }
    
    free(eq);
    return new_eq;
}

// Accumulate all the inputs related to wake up window condition into
// the set for Espresso format
vector<signal_type> push_back_inputs_window(vector<string> active_states, unsigned wi) {

    unsigned n_places = windows[wi].places.size();
    unsigned n_states = windows[wi].states.size();
    vector<signal_type> inputs;

    // Add input names
    for (unsigned i = 0; i < n_places; i++) {
        string name_input = name_place_in_window(wi, i);

        // Add input marking
        for (unsigned j = 0; j < n_states; j++) {
            add_signal(&inputs, name_input, j, wi, i);
        }
    }

    return inputs;
}

// Accumulate all the outputs related to wake up window condition into
// the set for Espresso format
vector<signal_type> push_back_output_window( vector<string> woken_states,\
                                             unsigned wi, unsigned si) {

    unsigned n_states = windows[si].states.size();
    signal_type signal;
    int sig_index;
    string out_name = "w" + to_string(wi+1);
    vector<signal_type> outputs;

    for (unsigned i = 0; i < n_states; i++) {
        int val = state_exist_set(windows[si].states[i].name, woken_states) != -1 ? 1:0;
        if ( (sig_index = signal_exist(out_name, outputs)) == -1 ) {
            signal.name = out_name;
            signal.marking.push_back(val);
            outputs.push_back(signal);
        } else {
            outputs[(unsigned)sig_index].marking.push_back(val);
        }
    }

    return outputs;
}

// Generate wakeup marking conditions of one windows
int generate_marking_condition(unsigned wi) {

    vector<string> woken_states = find_all_woken_states(wi);
    unsigned n_windows = windows.size();
    unsigned n_woken_states = woken_states.size();
    kmw.clear();

    for (unsigned i = 0; i < n_woken_states; i++) {
        for (unsigned j = 0; j < n_windows; j++) {
            espresso_format kmt;

            int si = state_exist_window(woken_states[i], j);

            if ( wi != j && si != -1 ) {
                push_back_signals(&kmt.inputs, (unsigned)si, j);
                kmt.states_in.push_back(windows[j].states[si].name);
            } else if ( wi == j ) {
                push_back_signals(&kmt.outputs, (unsigned)si, j);
                kmt.states_out.push_back(windows[wi].states[si].name);
            }
        
            if( !kmt.states_in.empty() || !kmt.states_out.empty())
            kmw.push_back(kmt);
        }
    }

    if ( write_karnaugh_map(pla_path) ) {
        fprintf(stderr, "Error on writing the Karnaugh map\n");
        return -1;
    }

    run_espresso((char*)espresso_path.c_str(), pla_path, espresso_result_path);

    vector<string> equations = read_equations(espresso_result_path);

    cout << "Wakeup marking conditions for Window " << wi+1 << ":" << "\n";
    if ( abc_path.empty() ) {
        unsigned n_eqs = equations.size();
        for (unsigned i = 0; i < n_eqs; i++) {
            cout << equations[i] << "\n";
        }

        return 0;
    }

    if ( refactorize_equations(equations) ) {
        fprintf(stderr, "Error refactorizing equations with ABC\n");
        return -1;
    }

    return 0;
}

// Accumulate all the signal values into the set for Espresso format
void push_back_signals(vector<signal_type> *sig, unsigned si, unsigned wi) {

    unsigned n_places = windows[wi].places.size();

    // Copy input names and markings
    for (unsigned i = 0; i < n_places; i++) {
        string name_input = name_place_in_window(wi, i);
        add_signal(sig, name_input, si, wi, i);
    }

    return;
}

// Create input name attaching number of the window and the name of the place
string name_place_in_window(unsigned wi, unsigned pi) {
    return ("w" + to_string(wi+1) + windows[wi].places[pi]);
}

// Add signal into the set for the wakeup condition generation
void add_signal(vector<signal_type> *sigs, string name_sig,\
                unsigned si, unsigned wi, unsigned pi) {
    
    int sig_index;
    
    if ( (sig_index = signal_exist(name_sig, (*sigs))) != -1 ) {
        (*sigs)[(unsigned)sig_index].marking.push_back(is_marked(pi, windows[wi].states[si]));
    } else {
        signal_type signal;
        signal.name = name_sig;
        signal.marking.push_back(is_marked(pi, windows[wi].states[si]));
        (*sigs).push_back(signal);
    }
    return;
}

// Is the signal already inside the set?
int signal_exist(string name_sig, vector<signal_type> sigs) {

    unsigned n_sigs = sigs.size();

    for (unsigned i = 0; i < n_sigs; i++) {
        if ( !sigs[i].name.compare(name_sig) ) {
            return (int)i;
        }
    }

    return -1;
}

// Is place marked in the state considered?
int is_marked(unsigned pi, state_type s) {

    unsigned n_marking = s.marking.size();

    for (unsigned i = 0; i < n_marking; i++) {
        if (s.marking[i] == pi) {
            return 1;
        }
    }

    return 0;
}

// Find states which need wakeup Boolean conditions
vector<string> find_all_woken_states(unsigned wi) {
    
    vector<string> woken_states;
    unsigned n_states = windows[wi].states.size();

    for (unsigned j = 0; j < n_states; j++) {

        vector<string> sources = find_source_states(windows[wi].states[j].name);
        unsigned n_sources = sources.size();

        for (unsigned k = 0; k < n_sources; k++) {
            if ( state_exist_window(sources[k], wi) == -1 &&
                 state_exist_set(windows[wi].states[j].name, woken_states) == -1 ) {
                woken_states.push_back(windows[wi].states[j].name);
            }
        }
    }

    return woken_states;
}

// Find states which need wakeup Boolean conditions between two windows
vector<string> find_woken_states_between_windows(unsigned wi, unsigned di) {
    
    vector<string> woken_states;
    unsigned n_states = windows[wi].states.size();

    for (unsigned j = 0; j < n_states; j++) {

        vector<string> sources = find_source_states(windows[wi].states[j].name);
        unsigned n_sources = sources.size();

        for (unsigned k = 0; k < n_sources; k++) {
            if ( state_exist_window(sources[k], wi) == -1 &&
                 state_exist_window(sources[k], di) != -1 &&
                 state_exist_set(windows[wi].states[j].name, woken_states) == -1 ) {
                woken_states.push_back(windows[wi].states[j].name);
            }
        }
    }

    return woken_states;
}

// Does the state exist in the set of states 'set'?
int state_exist_set(string state_name, vector<string> set) {
    unsigned ncs = set.size();

    for (unsigned i = 0; i < ncs; i++) {
        if ( !state_name.compare(set[i]) ) {
            return i;
        }
    }

    return -1;
}

// Does the state exist in the window with index 'wi'?
int state_exist_window(string state_name, unsigned wi) {
    unsigned nsw = windows[wi].states.size();

    for (unsigned i = 0; i < nsw; i++) {
        if ( !state_name.compare(windows[wi].states[i].name) ) {
            return (int)i;
        }
    }

    return -1;
}

// Find all source states in the network
vector<string> find_source_states(string state_name) {

    vector<string> source_states;
    unsigned n_arcs = network.arc.size();

    for (unsigned i = 0; i < n_arcs; i++) {
        if ( !state_name.compare(network.dest[i]) ) {
            source_states.push_back(network.source[i]);
        }
    }

    return source_states;
}

// Run Espresso logic minimizer over a karnaugh map
int run_espresso(char* espresso_path, char* karnaugh_map, char* result_path) {

	char *command;

    // Build command
	command = strdup("");
	command = cat_strings(command, espresso_path);
	command = cat_char(command, ' ');
	command = cat_strings(command, (char*) ESPRESSO_OPTIONS);
	command = cat_char(command, ' ');
	command = cat_strings(command, karnaugh_map);
    command = cat_strings(command, (char*) " > ");
    command = cat_strings(command, result_path);
    command = cat_strings(command, (char*) " 2>&1");

    // Run Espresso
    if ( system(command) == -1 ) {
		fprintf(stderr, "Error running Espresso: %s\n", command);
		return -1;
	}

    free(command);

	return 0;
}

// Write Karnaugh map for marking conditions in Espresso format
int write_karnaugh_map(char* pla_path) {

    FILE *fp = NULL;
    vector<string> inputs;
    vector<string> outputs;
    int n_inputs = count_inputs(&inputs);
    int n_outputs = count_outputs(&outputs);

    if ( (fp = fopen(pla_path, "w")) == NULL) {
        fprintf(stderr, "Error opening the temporary file\n");
        return -1;
    }

    fprintf(fp, ".type fr\n");
    fprintf(fp, ".i %d\n", n_inputs);
    fprintf(fp, ".o %d\n", n_outputs);
    fprintf(fp, ".ilb ");
    for (unsigned i = 0; i < (unsigned) n_inputs; i++) {
        fprintf(fp, "%s ", inputs[i].c_str());
    }
    fprintf(fp, "\n");
    fprintf(fp, ".ob ");
    for (unsigned i = 0; i < (unsigned) n_outputs; i++) {
        fprintf(fp, "%s ", outputs[i].c_str());
    }
    fprintf(fp, "\n");

    write_marking(fp, inputs, n_inputs, outputs, n_outputs);

    fprintf(fp, ".e");

    fclose(fp);

    return 0;
}

// Set the range of the inputs within the inputs in the Karnaugh Map
void limit_input(vector<string> inputs, unsigned n_in) {

    unsigned n_km = kmw.size();
    int l;
    int r;

    for (unsigned w = 0; w < n_km; w++) {
        if ( kmw[w].states_in.empty() ) {
            continue;
        }

        unsigned last_el = kmw[w].inputs.size() - 1;
        l = 0;
        r = 0;

        for (unsigned i = 0; i < n_in && (!l || !r); i++) {
            if ( !l && !kmw[w].inputs[0].name.compare(inputs[i]) ) {
                kmw[w].left = i;
                l = 1;
                continue;
            }

            if ( !r && !kmw[w].inputs[last_el].name.compare(inputs[i]) ) {
                kmw[w].right = i;
                r = 1;
                continue;
            }
        }
    }

    return;
}

// Write inputs and output values inside the Karnaugh map for Espresso
void write_marking(FILE *fp, vector<string> inputs, int n_in,\
                   vector<string> outputs, int n_out) {

    unsigned n_states = states.size();

    limit_input(inputs, (unsigned) n_in);

    for (unsigned s = 0; s < n_states; s++) {

        if ( state_exist_km(states[s]) == -1 ) {
            continue;
        }

        write_inputs_km(fp, n_in, s);
        fprintf(fp," ");
        write_outputs_km(fp, n_out, s);
        fprintf(fp,"\n");
    }

    return;
}

// Write inputs signals inside the Karnaugh map
void write_inputs_km(FILE *fp, int n_in, unsigned s) {

    int m;
    int dc;
    int cin = 0;
    unsigned n_km = kmw.size();

    // Inputs
    while ( cin < n_in ) {

        // Active values
        for (unsigned w = 0; w < n_km && cin < n_in; w++) {

            if ( kmw[w].states_in.empty() || \
               ( cin < kmw[w].left || cin > kmw[w].right ) || \
               ( m = state_exist_set(states[s], kmw[w].states_in) ) == -1 ) {
                continue;
            }

            cin += print_signal_values(fp, kmw[w].inputs, m);

        }

        // DC values
        dc = 0;
        for (unsigned w = 0; w < n_km && cin < n_in && !dc; w++) {

            if ( kmw[w].states_in.empty() || \
               ( cin < kmw[w].left || cin > kmw[w].right ) ) {
                continue;
            }

            unsigned n_inputs = kmw[w].inputs.size();
            for (unsigned i = 0; i < n_inputs; i++) {
                fprintf(fp, "-");
            }
            cin += n_inputs;
            dc = 1;
        }
    }

    return;
}

// Write inputs signals inside the Karnaugh map
void write_outputs_km(FILE *fp, int n_out, unsigned s) {

    int cout = 0;
    int m;
    unsigned n_km = kmw.size();

    for (unsigned w = 0; w < n_km && cout < n_out; w++) {
        if ( kmw[w].states_out.empty() || \
           ( m = state_exist_set(states[s], kmw[w].states_out) ) == -1 ) {
            continue;
        }

        cout += print_signal_values(fp, kmw[w].outputs, m);
    }

    return;
}

// Print the signal values in the Karnaugh map
int print_signal_values(FILE *fp, vector<signal_type> signals, int mark_i) {

    unsigned n_signals = signals.size();

    for (unsigned i = 0; i < n_signals; i++) {
        fprintf(fp, "%d", signals[i].marking[(unsigned)mark_i]);
    }

    return (int)n_signals;
}

// Does the state exist in the Karnaugh maps stored?
int state_exist_km(string state) {

    unsigned n_km = kmw.size();
    int index;
    
    for (unsigned w = 0; w < n_km; w++) {
        if( !kmw[w].states_in.empty() ) {
            if ( (index = state_exist_set(state, kmw[w].states_in)) != -1 ) {
                return index;
            }
        }
    }

    return -1;
}

// Count and list the input signals
int count_inputs(vector<string> *inputs) {

    unsigned n_km = kmw.size();
    int n = 0;

    for (unsigned w = 0; w < n_km; w++ ) {
        unsigned n_inputs = kmw[w].inputs.size();
        for (unsigned i = 0; i < n_inputs; i++) {
            if ( state_exist_set(kmw[w].inputs[i].name, (*inputs)) == -1 ) {
                (*inputs).push_back(kmw[w].inputs[i].name);
            }
        }
    }

    n = (int) (*inputs).size();

    return n;
}

// Count and list the output signals
int count_outputs(vector<string> *outputs) {

    unsigned n_km = kmw.size();
    int n = 0;

    for (unsigned w = 0; w < n_km; w++ ) {
        unsigned n_outputs = kmw[w].outputs.size();
        for (unsigned i = 0; i < n_outputs; i++) {
            if ( state_exist_set(kmw[w].outputs[i].name, (*outputs)) == -1 ) {
                (*outputs).push_back(kmw[w].outputs[i].name);
            }
        }
    }

    n = (int) (*outputs).size();

    return n;
}

// Print signals
void print_signals(vector<signal_type> sigs) {

    unsigned n_sigs = sigs.size();

    for (unsigned i = 0; i < n_sigs; i++) {
        cout << sigs[i].name << ": ";
        unsigned n_marking = sigs[i].marking.size();
        for (unsigned j = 0; j < n_marking; j++) {
            cout << sigs[i].marking[j] << " ";
        }
        cout << "\n";
    }
    cout << "=====\n";

    return;
}
