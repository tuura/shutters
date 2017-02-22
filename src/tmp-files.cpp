#if defined(__linux) || defined(__APPLE__)
	#include "tmp-files.h"
#else
	#include "Path\tmp-files.h"
#endif

int create_tmp_filename(char* tmp_name){

    #if defined(__linux) || defined(__APPLE__)
        int fd;

        // Create and open temporary file inside tmp std dir
	    if ( (fd = mkstemp(tmp_name)) == -1 ) {
		    fprintf(stderr,"Error on the creation of the tmp file: %s.\n", tmp_name);
		    return -1;
	    }

        // Close file just opened
        close(fd);
    #else
	    char tmp_string[L_tmpnam];

	    // Get std dir for temp files
	    if ( GetTempPath(FILENAME_MAX,tmp_name) == 0 ) {
		    fprintf(stderr, "Error getting temporary directory path.\n");
		    return -1;
	    }

	    // creating unique fileName
	    if ( tmpnam(tmp_string) == NULL ) {
		    fprintf(stderr, "Error on the generation of a unique temp file name.\n");
		    return -1;
	    }

	    // concat temp file name
	    strcat(tmp_name,tmp_string);
    #endif

	return 0;
}

// removing temporary files from the HDD
int rm_tmp_file(char* tmp_name) {
	char *command;

	if ( file_exist(tmp_name) ) {
        #if defined(__linux) || defined(__APPLE__)
	        command = strdup("rm -f ");
        #else
        	command = strdup("del ");
        #endif
		command = cat_strings(command, tmp_name);
		if ( system(command) == -1 ) {
			fprintf(stderr,"Error on removing %s.\n", tmp_name);
			return -1;
		}
		free(command);
	}

	return 0;
}

int copy_file(char* src, char* dst) {

    FILE *fps = NULL, *fpd = NULL;
    char c;

    if ( (fps = fopen(src, "r")) == NULL ) {
        fprintf(stderr, "Error opening the temporary file\n");
        return -1;
    }

    if ( (fpd = fopen(dst, "w")) == NULL ) {
        fprintf(stderr, "Error opening the temporary file\n");
        return -1;
    }

    while ( (c = fgetc(fps)) != EOF ) {
        fprintf(fpd,"%c", c);
    }

    fclose(fps);
    fclose(fpd);

    return 0;
}

// Create all temporary files
void create_tmp_files() {

    create_tmp_filename(pla_path);
    create_tmp_filename(pla_path_back);
    create_tmp_filename(pla_path_copy);
    create_tmp_filename(espresso_result_path);
    create_tmp_filename(espresso_copy_path);
    create_tmp_filename(abc_script_path);
    create_tmp_filename(abc_equations_path);
    create_tmp_filename(abc_result_path);

    return;
}

// Remove all temporary files
void remove_tmp_files() {

    rm_tmp_file(pla_path);
    rm_tmp_file(pla_path_back);
    rm_tmp_file(pla_path_copy);
    rm_tmp_file(espresso_result_path);
    rm_tmp_file(espresso_copy_path);
    rm_tmp_file(abc_script_path);
    rm_tmp_file(abc_equations_path);
    rm_tmp_file(abc_result_path);

    return;
}

// Check file existance
int file_exist(char *tmp_name) {
	FILE *fp = fopen(tmp_name, "r");
	if ( fp!=NULL ) fclose (fp);
	return (fp!=NULL);
}

// Concatenates two strings creating the right portion in the memory
char* cat_strings(char *str1, char *str2) {

	char *newStr;

	newStr = (char *) malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
	sprintf(newStr, "%s%s", str1, str2);
	
	free(str1);

	return newStr;
}

// Concatenate one string with a character
char* cat_char(char *str1, char c) {

	char *newStr;

	newStr = (char *) malloc(sizeof(char) * (strlen(str1) + 2));
	sprintf(newStr, "%s%c", str1, c);
	
	free(str1);

	return newStr;
}

// Print file to stdout (for debugging)

void print_file(char *file_name) {

    FILE *fp;
    char c;

    if ( (fp = fopen(file_name, "r")) == NULL ) {
        fprintf(stderr, "Error opening %s.\n", file_name);
        return;
    }

    printf("\n=====================\n");
    while ( (c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    printf("\n=====================\n");

    fclose(fp);

    return;
}
