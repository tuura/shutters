#if defined(__linux) || defined(__APPLE__)
	#include "eq-generator.h"
#else
	#include "Path\eq-generator.h"
#endif

// Generate wakeup marking and window conditions
int generate_wakeup_conditions() {

    create_tmp_filename(tmp_path);

    if ( generate_marking_conditions() ) {
        fprintf(stderr, "Error on the generation of the marking conditions.\n");
        rm_tmp_file(tmp_path);
        return -1;
    }

    // generate_windows_conditions();
    rm_tmp_file(tmp_path);

    return 0;
}

// Generate wakeup marking conditions of all the windows
int generate_marking_conditions() {

    unsigned n_windows = windows.size();

    for (unsigned i = 0; i < n_windows; i++) {
        if ( generate_marking_window(i) ) {
            rm_tmp_file(tmp_path);
            fprintf(stderr, "Error on generating marking conditions\
                             for the Window %d\n", i+1);
            return -1;
        }
    }

    return 0;
}

// Generate wakeup marking conditions of one windows
int generate_marking_window(unsigned wi) {

    vector<string> woken_states = find_woken_states(wi);
    unsigned n_windows = windows.size();
    unsigned n_woken_states = woken_states.size();
    clear_karnaugh_map();

    for (unsigned i = 0; i < n_woken_states; i++) {
        for (unsigned j = 0; j < n_windows; j++) {

            int si = state_exist_window(woken_states[i], j);

            if ( wi != j && si != -1 ) {
                push_back_signals(&km.inputs, (unsigned)si, j);
            } else if ( wi == j ) {
                push_back_signals(&km.outputs, (unsigned)si, j);
            }

        }
    }

    if ( write_karnaugh_map(tmp_path) ) {
        fprintf(stderr, "Error on writing the Karnaugh map\n");
        return -1;
    }

    cout << "Wakeup marking conditions for Window " << wi+1 << ":\n";
    run_espresso((char*)espresso_path.c_str(), tmp_path);

    return 0;
}

// Write Karnaugh map in Espresso format
int write_karnaugh_map(char* tmp_path) {

    FILE *fp = NULL;
    int n_inputs = (int) km.inputs.size();
    int n_outputs = (int) km.outputs.size();

    if ( (fp = fopen(tmp_path, "w")) == NULL) {
        fprintf(stderr, "Error opening the temporary file\n");
        return -1;
    }

    fprintf(fp, ".type fr\n");
    fprintf(fp, ".i %d\n", n_inputs);
    fprintf(fp, ".o %d\n", n_outputs);
    fprintf(fp, ".ilb ");
    for (unsigned i = 0; i < (unsigned) n_inputs; i++) {
        fprintf(fp, "%s ", km.inputs[i].name.c_str());
    }
    fprintf(fp, "\n");
    fprintf(fp, ".ob ");
    for (unsigned i = 0; i < (unsigned) n_outputs; i++) {
        fprintf(fp, "%s ", km.outputs[i].name.c_str());
    }
    fprintf(fp, "\n");
    unsigned n_marking = km.inputs[0].marking.size();
    for (unsigned j = 0; j < n_marking; j++) {
        for (unsigned i = 0; i < (unsigned) n_inputs; i++) {
            fprintf(fp, "%d", km.inputs[i].marking[j]);
        }
        fprintf(fp, " ");
        for (unsigned i = 0; i < (unsigned) n_outputs; i++) {
            fprintf(fp, "%d", km.outputs[i].marking[j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, ".e");

    fclose(fp);

    return 0;
}

// Accumulate all the signal values into the set for Espresso format
void push_back_signals(vector<signal_type> *sig, unsigned si, unsigned wi) {

    unsigned n_places = windows[wi].places.size();

    // Copy input names and markings
    for (unsigned i = 0; i < n_places; i++) {
        string name_input = "w" + to_string(wi+1) + windows[wi].places[i];
        add_signal(sig, name_input, si, wi, i);
    }

    return;
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
vector<string> find_woken_states(unsigned wi) {
    
    vector<string> woken_states;
    unsigned n_states = windows[wi].states.size();

    for (unsigned j = 0; j < n_states; j++) {

        vector<string> sources = find_source_states(windows[wi].states[j].name);
        unsigned n_sources = sources.size();

        for (unsigned k = 0; k < n_sources; k++) {
            if ( state_exist_window(sources[k], wi) == -1 &&
                 !state_exist_set(windows[wi].states[j].name, woken_states) ) {
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
            return 1;
        }
    }

    return 0;
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
int run_espresso(char* espresso_path, char* karnaugh_map) {

	char *command;

    // Build command
	command = strdup("");
	command = cat_strings(command, espresso_path);
	command = cat_char(command, ' ');
	command = cat_strings(command, (char*) ESPRESSO_OPTIONS);
	command = cat_char(command, ' ');
	command = cat_strings(command, karnaugh_map);

    // Run Espresso
    if ( system(command) == -1 ) {
		fprintf(stderr, "Error running Espresso: %s\n", command);
		return -1;
	}

    free(command);

	return 0;
}

// Clear Karnaugh map saved for the generation of the conditions
void clear_karnaugh_map() {

    unsigned n_inputs = km.inputs.size();
    unsigned n_outputs = km.outputs.size();

    for (unsigned i = 0; i < n_inputs; i++) {
        km.inputs.clear();
        km.inputs.clear();
    }

    for (unsigned i = 0; i < n_outputs; i++) {
        km.outputs.clear();
        km.outputs.clear();
    }

    return;
}

// Print signals
void print_signals(vector<signal_type> sigs) {

    unsigned n_sigs = sigs.size();

    for (unsigned i = 0; i < n_sigs; i++) {
        cout << km.inputs[i].name << ": ";
        unsigned n_marking = sigs[i].marking.size();
        for (unsigned j = 0; j < n_marking; j++) {
            cout << km.inputs[i].marking[j] << " ";
        }
        cout << "\n";
    }
    cout << "=====\n";

    return;
}
