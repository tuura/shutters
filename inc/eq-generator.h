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
vector<string> find_common_states(unsigned, unsigned);
vector<string> find_woken_states(vector<string>, unsigned, unsigned);
vector<string> find_source_states(string);
int state_exist_set(string, vector<string>);
int state_exist_window(string, unsigned);
int run_espresso(char*, char*);

#endif
