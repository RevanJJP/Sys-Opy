#ifndef Z2_IOSYS_H
#define Z2_IOSYS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <sys/wait.h>

#define max_lines 8
#define max_words 32
#define max_word_size 32

void error(char* function_name, char* error_message, int error_code);
void warning(char* function_name, char* warning_meesage, int warning_code);

int lineread(FILE* file, char*** args);

#endif