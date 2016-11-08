#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

// Global variables
#if defined(__linux) || defined(__APPLE__)
	string espresso_path("espresso");
	char tmp_path[] = "/tmp/karnaugh-map.XXXXXX";
#else
	string espresso_path("espresso.exe");
	char tmp_path[FILENAME_MAX];
#endif
string marking_path;
net_type network;
vector<window_type> windows;
espresso_format km;

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

    // Print data structure
    // print_structure();

    // Generate Boolean equations
    generate_wakeup_conditions();

    cout << "Program terminated.\n";

    return 0;

}
