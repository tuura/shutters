#ifndef __PAR_PARSER_H__
#define __PAR_PARSER_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

int parse_arg(int, char**);
int parse_espresso(int, int, char**);
int parse_abc(int, int, char**);
int parse_help(int, int, char**);
int parse_version(int, int, char**);
void print_help(char**);
void print_version(char**);
void print_usage(char**);

#endif
