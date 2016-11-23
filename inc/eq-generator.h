#ifndef __EQ_GENERATOR_H__
#define __EQ_GENERATOR_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

int generate_wakeup_conditions();
int generate_marking_conditions();
int generate_window_conditions();
int generate_window_condition(unsigned);
int generate_marking_condition(unsigned);
int state_exist_set(string, vector<string>);
int state_exist_window(string, unsigned);
int state_exist_km(string);
int is_marked(unsigned, state_type);
int signal_exist(string, vector<signal_type>);
int run_espresso_on_km(string);
int run_espresso(char*, char*, char*);
int write_karnaugh_map(char*);
int count_inputs(vector<string>*);
int count_outputs(vector<string>*);
int print_signal_values(FILE*, vector<signal_type>, int);

char* replace_zeros_ones(char*);

void push_back_signals(vector<signal_type>*, unsigned, unsigned);
void add_signal(vector<signal_type>*, string, unsigned, unsigned, unsigned);
void write_inputs_km(FILE*, int, unsigned);
void write_outputs_km(FILE*, int, unsigned);
void limit_input(vector<string>, unsigned);
void write_marking(FILE*, vector<string>, int, vector<string>, int);
void print_signals(vector<signal_type>);

string name_place_in_window(unsigned, unsigned);

vector<string> find_all_woken_states(unsigned);
vector<string> find_woken_states_between_windows(unsigned, unsigned);
vector<string> find_source_states(string);
vector<string> read_equations(char*);

vector<signal_type> push_back_output_window(vector<string>, unsigned, unsigned);
vector<signal_type> push_back_inputs_window(vector<string>, unsigned);

#endif
