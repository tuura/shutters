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
#include <unistd.h>

// Files to include
#if defined(__linux) || defined(__APPLE__)
    #include "global.h"
    #include "par-parser.h"
    #include "mark-parser.h"
    #include "tmp-files.h"
    #include "eq-generator.h"
    #include "eq-refactoring.h"
#else
    #include <windows.h>
    #include "PATH\global.h"
    #include "PATH\par-parser.h"
    #include "PATH\mark-parser.h"
    #include "PATH\tmp-files.h"
    #include "PATH\eq-generator.h"
    #include "PATH\eq-refactoring.h"
#endif

// Key-words defines
#define ESPRESSO_OPTIONS    "-D so -o eqntott"
//#define ESPRESSO_OPTIONS    "-D qm -o eqntott"
#define TRANSITIONS         ".transitions"
#define INITIAL             ".initial"
#define WINDOW              ".window"
#define STATE               ".state"

#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif
