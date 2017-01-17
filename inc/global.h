#ifndef __GLOBAL_H__
#define __GLOBAL_H__

using namespace std;

// Data types
typedef struct struct_tag {
    string name;
    vector<unsigned> marking;
} state_type;

typedef struct struct_tag2 {
    vector<string> places;
    vector<state_type> states;
} window_type;

typedef struct struct_tag3 {
    vector<string> source;
    vector<string> arc;
    vector<string> dest;
    string initial;
} net_type;

typedef struct struct_tag4 {
    string         name;
    vector<int>    marking;
} signal_type;

typedef struct struct_tag5 {
    vector<signal_type> inputs;
    vector<signal_type> outputs;
    vector<string>      states_in;
    vector<string>      states_out;
    int left;
    int right;
} espresso_format;

// Global variables
#if defined(__linux) || defined(__APPLE__)
    extern char pla_path[];
    extern char pla_path_back[];
    extern char pla_path_copy[];
    extern char espresso_result_path[];
    extern char espresso_copy_path[];
    extern char abc_script_path[];
    extern char abc_equations_path[];
    extern char abc_result_path[];
#else
	extern char pla_path[FILENAME_MAX];
	extern char pla_path_back[FILENAME_MAX];
	extern char pla_path_copy[FILENAME_MAX];
    extern char espresso_copy_path[FILENAME_MAX];
    extern char espresso_result_path[FILENAME_MAX];
    extern char abc_script_path[FILENAME_MAX];
    extern char abc_equations_path[FILENAME_MAX];
    extern char abc_result_path[FILENAME_MAX];
#endif
extern string espresso_path;
extern string abc_path;
extern string marking_path;
extern net_type network;
extern vector<window_type> windows;
extern vector<espresso_format> kmw;
extern vector<string> states;
extern int positive_mode;
extern int win;

#endif
