#if defined(__linux) || defined(__APPLE__)
	#include "par-parser.h"
#else
	#include "Path\par-parser.h"
#endif

// Parse argument of the program
int parse_arg(int argc, char *argv[]) {

    int cur = 1;

    if ( argc < 2 ) {
        print_usage(argv);
        return -1;
    } else {
        marking_path = argv[1];
    }

    while (cur < argc) {

        if ( parse_espresso(cur, argc, argv) )  return -1;
        if ( parse_abc(cur, argc, argv) )       return -1;
        if ( parse_help(cur, argc, argv) )      return -1;
        if ( parse_version(cur, argc, argv) )   return -1;
        if ( parse_positive(cur, argc, argv) )  return -1;

        cur++;
    }

    return 0;
}

// Parse -e Espresso Path
int parse_espresso(int i, int argc, char **argv) {

    if ( !strcmp(argv[i], "-e") ) {
		if ( i+1 < argc ) {
            espresso_path = argv[i+1];
		} else {
			printf("After -e, Espresso logic minimizer path must be introduced.\n");
			return -1;
        }
	}

    return 0;
}

// Parse -a ABC Path
int parse_abc(int i, int argc, char **argv) {

    if ( !strcmp(argv[i], "-a") ) {
		if ( i+1 < argc ) {
            abc_path = argv[i+1];
		} else {
			printf("After -a, ABC path must be introduced.\n");
			return -1;
        }
	}

    return 0;
}

// Parse -p or --positive
int parse_positive(int i, int argc, char **argv) {

    if ( !strcmp(argv[i], "-p") || !strcmp(argv[i], "--positive") ) {
        positive_mode = 1;
	}

    return 0;
}

// Parse -h or --help
int parse_help(int i, int argc, char **argv) {

    if ( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") ) {
		print_help(argv);
        return -1;
	}

    return 0;
}

// Parse -v or --version
int parse_version(int i, int argc, char **argv) {

    if ( !strcmp(argv[i], "-v") || !strcmp(argv[i], "--version") ) {
        print_version(argv);
        return -1;
	}

    return 0;
}

// Print help
void print_help(char **argv) {

    printf("\n%s can generate wake-up window and marking condition for the ", argv[0]);
    printf("project named PROCESS WINDOW. Following the usage of the tool:\n\n");
    print_usage(argv);
    print_version(argv);

    return;
}

// Print usage
void print_usage(char **argv) {

    printf("Usage:\t%s marking_file_path\n", argv[0]);
    printf("Options available:\n");
    printf("\t-a ABC_path:\t\tspecify ABC path\n");
    printf("\t-e Espresso_path:\tspecify Espresso logic minimizer path\n");
    printf("\t-h or --help:\t\tprint help of the tool\n");
    printf("\t-p or --positive:\tforce equations to contain only positive literals\n");
    printf("\t-v or --version:\tprint tool version\n");

    return;
}

// Print version
void print_version(char **argv) {

    printf("%s v0.2\n", argv[0]);

    return;
}
