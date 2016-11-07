#ifndef __CONFIG_H__
#define __CONFIG_H__

// Libraries to include
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Files to include
#if defined(__linux) || defined(__APPLE__)
    #include "global.h"
    #include "par-parser.h"
    #include "mark-parser.h" 
#else
    #include "PATH\global.h"
    #include "PATH\par-parser.h"
    #include "PATH\mark-parser.h"
#endif

// Key-words defines
#define TRANSITIONS  ".transitions"
#define INITIAL      ".initial"
#define WINDOW       ".window"
#define STATE        ".state"

#endif
