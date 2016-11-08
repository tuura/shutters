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
void push_back_inputs(unsigned, unsigned);
int is_marked(unsigned, state_type);
void add_input(string, unsigned, unsigned, unsigned);
int input_exist(string);
int run_espresso(char*, char*);
void clear_karnaugh_map();

#endif
