#include "interpreter.h"

int lineread(FILE* file, char*** args) {
    int i=0, j=0, pipe_flag=0;
    char* word=NULL;
    while(feof(file)==0) {
        pipe_flag=0;
        args[i]=malloc(max_words*sizeof(char*));
        j=0;
        do {
            args[i][j]=malloc(max_word_size*sizeof(char));
            word=malloc(max_word_size*sizeof(char));
            if(fscanf(file, "%s", word)!=1) {
                free(word);
                return i;
            }
            if(word[0]=='|' && strlen(word)==1) {
                pipe_flag=1;
                free(word);
                fgetc(file); //take the space after pipe '|'
                break;
            }
            else if (word!=NULL){
                args[i][j]=word;
                free(word);
                j++;
            }
        } while(fgetc(file)==' ');
        args[i][j]=NULL;
        i++;
        if(pipe_flag==0) {
            args[i]=malloc(max_words*sizeof(char*));
            args[i][0]=malloc(max_word_size*sizeof(char));
            args[i][0]=NULL;
            pipe_flag=0;
            i++;
        }
    }
    return i;
}