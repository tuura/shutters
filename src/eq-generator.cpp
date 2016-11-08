#if defined(__linux) || defined(__APPLE__)
	#include "eq-generator.h"
#else
	#include "Path\eq-generator.h"
#endif

// Parse file which contains the network and the marking
int generate_wakeup_conditions() {

    return 0;
}

// Run Espresso logic minimizer over a karnaugh map
int run_espresso(char *espresso_path, char *karnaugh_map) {

	char *command;

    // Build command
	command = strdup("");
	command = cat_strings(command, espresso_path);
	command = cat_char(command, ' ');
	command = cat_strings(command, (char*) ESPRESSO_OPTIONS);
	command = cat_char(command, ' ');
	command = cat_strings(command, karnaugh_map);

    // Run Espresso
    if( system(command) == -1 ) {
		fprintf(stderr, "Error running Espresso: %s\n", command);
		return -1;
	}

    free(command);

	return 0;
}
