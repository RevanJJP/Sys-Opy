#define _XOPEN_SOURCE 500
#define PATH_MAX 1024
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

void error(char* function_name, char* error_meesage, int error_code);
void search_stat(char* path, char sign, struct tm* date);
int time_difference(struct tm date_file, struct tm date);
void print_info(char* path, struct stat info);

int main(int argc, char** argv) {
    if(argc!=4) error("main", "Wrong number of arguments", 1);

    char* path = argv[1];
    char* input_date = argv[3];
    char sign='\0';
    if(strlen(argv[2])==1 && (argv[2][0]=='>' || argv[2][0]=='<' || argv[2][0]=='=')) sign = argv[2][0];
    else error("main", "Wrong sign argument", 2);

    struct tm* date = malloc(sizeof(struct tm));
    strptime(input_date, "%m-%d-%Y", date);

    printf("\n%s\n", argv[1]);

    printf("SIZE\t\tPERMISSIONS\tLAST MODIFIED\t\tPATH\n");
    search_stat(realpath(path, NULL), sign, date);
    return 0;
}

void error(char* function_name, char* error_meesage, int error_code) {
    printf("\nERROR %d in function '%s':\n%s.\n", error_code, function_name, error_meesage);
    exit(error_code);
}

void search_stat(char* path, char sign, struct tm* date) {
    if(path==NULL) error("search_stat", "Wrong path", 1);
    
    DIR* dir=opendir(path);
    if(dir==NULL) error("search_stat", "Wrong directory", 2);

    char path_full[PATH_MAX+1];
    struct stat info;
    struct dirent* stream;

    while((stream=readdir(dir))!=NULL)  {
        if(strcmp(stream->d_name, ".")==0 || strcmp(stream->d_name, "..")==0) continue;


        strcpy(path_full,path);
        if(strlen(path)+strlen(stream->d_name)+1 > PATH_MAX) error("search_stat", "Too long path", 3);
        strcat(path_full, "/");
        strcat(path_full, stream->d_name);

        lstat(path_full, &info);

        if(S_ISREG(info.st_mode))  {
            if(sign=='=' && time_difference(*localtime(&info.st_mtime), *date)==0) print_info(path_full, info);
            if(sign=='<' && time_difference(*localtime(&info.st_mtime), *date)==-1) print_info(path_full, info);
            if(sign=='>' && time_difference(*localtime(&info.st_mtime), *date)==1) print_info(path_full, info);

        } else if(S_ISDIR(info.st_mode)) search_stat(path_full, sign, date);
    }
    closedir(dir);
}

void print_info(char* path, struct stat info){
    printf("%lldB\t\t",(long long int) info.st_size);
    printf((S_ISDIR(info.st_mode)) ? "d" : "-");
    printf((info.st_mode & S_IRUSR) ? "r" : "-");
    printf((info.st_mode & S_IWUSR) ? "w" : "-");
    printf((info.st_mode & S_IXUSR) ? "x" : "-");
    printf((info.st_mode & S_IRGRP) ? "r" : "-");
    printf((info.st_mode & S_IWGRP) ? "w" : "-");
    printf((info.st_mode & S_IXGRP) ? "x" : "-");
    printf((info.st_mode & S_IROTH) ? "r" : "-");
    printf((info.st_mode & S_IWOTH) ? "w" : "-");
    printf((info.st_mode & S_IXOTH) ? "x" : "-");

    char buffer[PATH_MAX];
    strftime(buffer, PATH_MAX, "%m-%d-%Y %H:%M:%S", localtime(&info.st_mtime));

    printf("\t%s\t%s\n", buffer, realpath(path, NULL));
}

int time_difference(struct tm date_file, struct tm date) {
    if(date_file.tm_year==date.tm_year && date_file.tm_yday==date.tm_yday) return 0;
    else if (date_file.tm_year > date.tm_year || (date_file.tm_year==date.tm_year && date_file.tm_yday>date.tm_yday)) return 1;
    else return -1;
}