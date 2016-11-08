#ifndef __EQ_GENERATOR_H__
#define __EQ_GENERATOR_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

int generate_wakeup_conditions();
int run_espresso(char*, char*);

#endif
