#if defined(__linux) || defined(__APPLE__)
	#include "par-parser.h"
#else
	#include "Path\par-parser.h"
#endif

// Parse file which contains the network and the marking
int parse_marking() {

    ifstream file;

    file.open(marking_path.c_str());
    if ( !file.is_open() ) {
        fprintf(stderr, "Error opening the file.\n");
        return -1;
    }

    read_transitions(file);

    read_windows(file);

    file.close();
    return 0;
}

// Read network transitions
int read_transitions(ifstream& file) {

    string word;

    // Read until ".transitions" key-word
    while ( file >> word && word.compare(TRANSITIONS) );

    while ( file >> word ) {

        if ( !word.compare(INITIAL) ) {
            file >> network.initial;
            break;
        }

        network.source.push_back(word);
        file >> word;
        network.arc.push_back(word);
        file >> word;
        network.dest.push_back(word);

    }

    return 0;
}

// Read windows
int read_windows(ifstream& file) {

    string word;

    // Read until ".window" key-word
    while ( file >> word && word.compare(WINDOW) );

    while ( file >> word ) {

        window_type w = read_window(file);
        windows.push_back(w);

    }

    return 0;
}

// Read single window
window_type read_window(ifstream& file) {

    window_type w;
    string word;
    int last_state = 0;

    // Read until ".state" key-word
    while ( file >> word && word.compare(STATE) );

    while (file >> word) {

        if ( !word.compare(WINDOW) ) {
            break;
        }

        state_type s;
        s.name = word;
        last_state = read_state(file, &w, &s);
        w.states.push_back(s);

        if ( last_state ) break;

    }

    return w;
}

// Read state and marking
// return -1 if last state is a new window is present afterwards
int read_state(ifstream& file, window_type *w, state_type *s) {

    string word;

    while ( file >> word && word.compare(STATE) ) {

        if ( !word.compare(WINDOW) ) {
            return -1;
        }

        if ( !place_exist((*w).places, word) ) {
            (*w).places.push_back(word);
            (*s).marking.push_back( (*w).places.size()-1 );
        } else {
            (*s).marking.push_back( place_index( (*w).places, word) );
        }
    }

    return 0;
}

// Is the place already in the set?
int place_exist(vector<string> places, string word) {

    for(unsigned i = 0; i < places.size(); i++) {
        if ( !places[i].compare(word) ) {
            return 1;
        }
    }

    return 0;
}

// Where (index) is the place in the set?
unsigned place_index(vector<string> places, string word) {

    unsigned i;

    for(i = 0; i < places.size(); i++) {
        if ( !places[i].compare(word) ) {
            return i;
        }
    }

    return i;
}

// Print the window structure
void print_windows() {

    cout << "Windows read by the file " << marking_path << ":\n";

    for(unsigned i = 0; i < windows.size(); i++) {

        cout << "\n" << "Window: " << i+1 << "\n";

        cout << "Places: ";
        for(unsigned j = 0; j < windows[i].places.size(); j++) {
            cout << windows[i].places[j] << " ";
        }

        cout << "\n" << "States: " << "\n";
        for(unsigned j = 0; j < windows[i].states.size(); j++) {

            cout << "\t" << windows[i].states[j].name << " -> ";
            for(unsigned k = 0; k < windows[i].states[j].marking.size(); k++) {
                cout << windows[i].places[ windows[i].states[j].marking[k] ] << " ";
            }
            cout << "\n";

        }
    }

    cout << "\n";

    return;
}
