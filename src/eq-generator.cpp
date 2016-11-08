#if defined(__linux) || defined(__APPLE__)
	#include "eq-generator.h"
#else
	#include "Path\eq-generator.h"
#endif

// Generate wakeup marking and window conditions
int generate_wakeup_conditions() {

    if ( generate_marking_conditions() ) {
        fprintf(stderr, "Error on the generation of the marking conditions.\n");
        return -1;
    }

    // generate_windows_conditions();

    return 0;
}

// Generate wakeup marking conditions of all the windows
int generate_marking_conditions() {

    unsigned n_windows = windows.size();

    for (unsigned i = 0; i < n_windows; i++) {
        if ( generate_marking_window(i) ) {
            fprintf(stderr, "Error on generating marking conditions\
                             for the Window %d\n", i+1);
            return -1;
        }
    }

    return 0;
}

// Generate wakeup marking conditions of one windows
int generate_marking_window(unsigned wi) {

    vector<string>  input_names;
    vector<int>     input_val;
    vector<string>  output_names;
    vector<int>     output_val;
    unsigned n_windows = windows.size();

    for (unsigned i = 0; i < n_windows; i++) {
        if ( wi != i ) {
            vector<string> common_states = find_common_states(wi, i);
            vector<string> woken_states = find_woken_states(common_states, wi, i);

            // debug
            cout << "COMMON: " << wi << " - " << i << "\n";
            for (unsigned k = 0; k < common_states.size(); k++) {
                cout << common_states[k] << " ";
            }
            cout << "\n";

            cout << "WOKEN: \n";
            for (unsigned k = 0; k < woken_states.size(); k++) {
                cout << woken_states[k] << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}

// Find common states which have incoming arcs of states from window i not in 
// the common set
vector<string> find_woken_states(vector<string> common_states, unsigned wi, unsigned i) {
    
    vector<string> woken_states;
    unsigned n_common_states = common_states.size();

    for (unsigned j = 0;  j < n_common_states; j++) {

        vector<string> source_states = find_source_states(common_states[j]);
        unsigned n_sources = source_states.size();

        for (unsigned k = 0; k < n_sources; k++) {
            if ( !state_exist_set(source_states[k], common_states) &&\
                 state_exist_window(source_states[k], i)           &&\
                 !state_exist_set(common_states[j], woken_states)) {
                woken_states.push_back(common_states[j]);
            }
        }

    }

    return woken_states;
}

int state_exist_set(string state_name, vector<string> set) {
    unsigned ncs = set.size();

    for (unsigned i = 0; i < ncs; i++) {
        if ( !state_name.compare(set[i]) ) {
            return 1;
        }
    }

    return 0;
}

int state_exist_window(string state_name, unsigned wi) {
    unsigned nsw = windows[wi].states.size();

    for (unsigned i = 0; i < nsw; i++) {
        if ( !state_name.compare(windows[wi].states[i].name) ) {
            return 1;
        }
    }

    return 0;
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

// Find common states between two windows
vector<string> find_common_states(unsigned si, unsigned di) {

    vector<string> intersections;
    unsigned s_states = windows[si].states.size();
    unsigned d_states = windows[di].states.size();
    
    for (unsigned i = 0; i < s_states; i++) {
        for (unsigned j = 0; j < d_states; j++) {
            if ( !windows[si].states[i].name.compare(windows[di].states[j].name) ) {
                intersections.push_back(windows[si].states[i].name);
            }
        }
    }

    return intersections;
}

// Run Espresso logic minimizer over a karnaugh map
int run_espresso(char *espresso_path, char *karnaugh_map) {

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
