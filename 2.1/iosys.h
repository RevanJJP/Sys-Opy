#ifndef Z2_IOSYS_H
#define Z2_IOSYS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void error(char* function_name, char* error_message, int error_code);
void warning(char* function_name, char* warning_meesage, int warning_code);

void generate(char* path, int rows, int row_size);

void sort_sys(char* path, int rows, int row_size);
void sort_lib(char* path, int rows, int row_size);

void copy_sys(char* source_path, char* destination_path, int rows, int row_size);
void copy_lib(char* source_path, char* destination_path, int rows, int row_size);

#endif