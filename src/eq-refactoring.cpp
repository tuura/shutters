#if defined(__linux) || defined(__APPLE__)
	#include "eq-refactoring.h"
#else
	#include "Path\eq-refactoring.h"
#endif

// Refactorize Espresso generated equations using ABC
int refactorize_equations(vector<string> equations) {

    vector<string> equations_ref;
    unsigned n_eqs;

    if ( write_abc_script() ) {
        fprintf(stderr, "Error writing script for ABC.\n");
        return -1;
    }

    if ( write_abc_equations(equations) ) {
        fprintf(stderr, "Error writing equations for ABC.\n");
        return -1;
    }

    if ( run_abc() ) {
        fprintf(stderr, "Error running ABC.\n");
        return -1;
    }

    equations_ref = read_equations_abc();

    n_eqs = equations_ref.size();
    for(unsigned i = 0; i < n_eqs; i++) {
        cout << equations_ref[i] << "\n";
    }

    return 0;
}

// Script for refactoring equations with ABC
int write_abc_script() {

    FILE *fp = NULL;

    if ( (fp = fopen(abc_script_path, "w")) == NULL ) {
        return -1;
    }

    fprintf(fp, "read_eqn %s\n", abc_equations_path);
    fprintf(fp, "strash\n");
    fprintf(fp, "refactor\n");
    fprintf(fp, "collapse\n");
    fprintf(fp, "write_eqn %s", abc_result_path);

    fclose(fp);

    return 0;
}

// Write file in Synopsys format for ABC
int write_abc_equations(vector<string> eqs) {

    FILE *fp = NULL;
    unsigned n_eqs = eqs.size();
    vector<string> inputs;
    vector<string> outputs;
    int n_inputs = count_inputs(&inputs);
    int n_outputs = count_outputs(&outputs);

    if ( (fp = fopen(abc_equations_path, "w")) == NULL ) {
        return -1;
    }

    // inputs
    fprintf(fp, "INORDER =");
    for (unsigned i = 0; i < (unsigned) n_inputs; i++) {
        fprintf(fp, " %s", inputs[i].c_str());
    }
    fprintf(fp, ";\n");

    // outputs
    fprintf(fp, "OUTORDER = ");
    for (unsigned i = 0; i < (unsigned) n_outputs; i++) {
        fprintf(fp, " %s", outputs[i].c_str());
    }
    fprintf(fp, ";\n");

    // equations
    for (unsigned i = 0; i < n_eqs; i++) {
        fprintf(fp, "%s\n", eqs[i].c_str());
    }

    fclose(fp);

    return 0;
}

// Run ABC
int run_abc(){

	char *command = NULL;

	command = strdup("");
	command = cat_strings(command, (char*)abc_path.c_str());
	command = cat_strings(command, (char*)" < ");
	command = cat_strings(command, abc_script_path);
    #if defined(__linux) || defined(__APPLE__)
	    command = cat_strings(command, (char*)" 1>/dev/null 2>&1");
    #else
	    command = cat_strings(command, (char*)" >nul 2>&1");
    #endif

    if( system(command) == -1 ) {
        fprintf(stderr, "Error on running ABC.\n");
        return -1;
    }

    free(command);

	return 0;
}

// Read ABC generated equations
vector<string> read_equations_abc() {
	FILE *fp = NULL;
	char c;
    vector<string> eqs;
	
	fp = fopen(abc_result_path, "r");

    // throw away first line
    while( (c = fgetc(fp)) != '\n');
	// throw away input and output lines
	for(int i = 0; i < 2; i++){
		while( (c = fgetc(fp)) != ';');
	}

    while ( (c = fgetc(fp)) != EOF ) {

        if ( c == ' ' || c == '\n' ) {
            continue;
        }
        
        char *eq;
        eq = strdup("");
        eq = cat_char(eq, c);
        while((c = fgetc(fp)) != ';'){
            eq = cat_char(eq, c);
	    }
        eq = cat_char(eq, ';');

        eqs.push_back(eq);
    }

	fclose(fp);

	return eqs;
}
