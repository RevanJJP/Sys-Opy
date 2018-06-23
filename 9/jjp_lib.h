#ifndef MYLIB_JJP_TIME_H
#define MYLIB_JJP_TIME_H

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

void print_time_result(struct timeval start, struct timeval end);
void print_result(struct rusage susage, struct rusage eusage);

void ERROR(int error_code, char* error_meesage);

void print_time_msec();

#endif