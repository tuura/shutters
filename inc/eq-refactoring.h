#ifndef __EQ_REFACTORING_H__
#define __EQ_REFACTORING_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

int refactorize_equations(vector<string>);
int write_abc_script();
int write_abc_equations(vector<string>);
int run_abc();

vector<string> read_equations_abc();

#endif
