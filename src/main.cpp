#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

// Global variables
#if defined(__linux) || defined(__APPLE__)
	string espresso_path ("espresso");
#else
	string espresso_path ("espresso.exe");
#endif
string marking_path;
net_type network;
vector<window_type> windows;

// Main function
int main(int argc, char** argv) {

    // Parse arguments
	if( parse_arg(argc, argv) ) {
		return -1;
	}

    // Parse marking file
    if ( parse_marking() ) {
        fprintf(stderr, "Error reading the marking file.\n");
        return -1;
    }

    print_windows();

    return 0;

}
