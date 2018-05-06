#ifndef MYLIB_JJP_TIME_H
#define MYLIB_JJP_TIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>


void print_time_result(struct timeval start, struct timeval end);
void print_result(struct rusage susage, struct rusage eusage);

void error(char* function_name, char* error_meesage, int error_code);

void warning(char* function_name, char* warning_meesage, int warning_code);

#endif