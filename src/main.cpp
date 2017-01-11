#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "Path\config.h"
#endif

// Global variables
#if defined(__linux) || defined(__APPLE__)
    string espresso_path("espresso");
	char pla_path[] = "/tmp/pla.XXXXXX";
	char pla_path_back[] = "/tmp/pla_back.XXXXXX";
	char pla_path_copy[] = "/tmp/pla_copy.XXXXXX";
    char espresso_result_path[] = "/tmp/esp_res.XXXXXX";
    char abc_script_path[] = "/tmp/abc_script.XXXXXX";
    char abc_equations_path[] = "/tmp/abc_eqs.XXXXXX";
    char abc_result_path[] = "/tmp/abc_result.XXXXXX";
#else
    string espresso_path("espresso.exe");
	char pla_path[FILENAME_MAX];
	char pla_path_back[FILENAME_MAX];
	char pla_path_copy[FILENAME_MAX];
    char espresso_result_path[FILENAME_MAX];
    char abc_script_path[FILENAME_MAX];
    char abc_equations_path[FILENAME_MAX];
    char abc_result_path[FILENAME_MAX];
#endif
string abc_path;
string marking_path;
net_type network;
vector<window_type> windows;
vector<espresso_format> kmw;
vector<string> states;
int positive_mode = 0;

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
