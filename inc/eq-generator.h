#ifndef __EQ_GENERATOR_H__
#define __EQ_GENERATOR_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

int generate_wakeup_conditions();
int generate_marking_conditions();
int generate_marking_window(unsigned);
vector<string> find_woken_states(unsigned);
vector<string> find_source_states(string);
int state_exist_set(string, vector<string>);
int state_exist_window(string, unsigned);
void push_back_signals(vector<signal_type>*, unsigned, unsigned);
int is_marked(unsigned, state_type);
void add_signal(vector<signal_type>*, string, unsigned, unsigned, unsigned);
int signal_exist(string, vector<signal_type>);
int run_espresso(char*, char*);
void clear_karnaugh_map();
void print_signals(vector<signal_type>);
int write_karnaugh_map(char*);

#endif
