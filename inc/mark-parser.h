#ifndef __MARK_PARSER_H__
#define __MARK_PARSER_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

int parse_marking();
int read_transitions(ifstream&);
int read_windows(ifstream&);
window_type read_window(ifstream&);
int read_state(ifstream&, window_type*, state_type*);
int place_exist(vector<string>, string);
unsigned place_index(vector<string>, string);
void print_structure();
void print_transitions();
void print_windows();

#endif
