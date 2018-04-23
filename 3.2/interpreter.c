#include "interpreter.h"

void error(char* function_name, char* error_meesage, int error_code) {
    printf("\nERROR %d in function '%s':\n%s.\n", error_code, function_name, error_meesage);
    exit(error_code);
}

void warning(char* function_name, char* warning_meesage, int warning_code) {
    printf("\nWARNING %d in function '%s':\n%s.\n", warning_code, function_name, warning_meesage);
}

int lineread(FILE* file, char*** args) {
    int i=0, j=0;
    while(feof(file)==0) {
        args[i]=malloc(max_words*sizeof(char*));
        j=0;
        do {
            args[i][j]=malloc(max_word_size*sizeof(char));
            fscanf(file, "%s", args[i][j]);
            j++;
        } while(fgetc(file)==' ');
        args[i][j]=NULL;
        i++;
    }
    if(args[i-1][0][0]=='\0') {
        free(args[i-1]);
        i--;
    }
    return i;
}