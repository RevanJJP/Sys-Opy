#include "iosys.h"

void error(char* function_name, char* error_meesage, int error_code) {
    printf("\nERROR %d in function '%s':\n%s.\n", error_code, function_name, error_meesage);
    exit(error_code);
}
void warning(char* function_name, char* warning_meesage, int warning_code) {
    printf("\nWARNING %d in function '%s':\n%s.\n", warning_code, function_name, warning_meesage);
}

void generate(char* path, int rows, int row_size)   {
    FILE* file = fopen(path, "w");
    if(file==NULL)  error("generate", "Can't open the file", 1);

    FILE* generator = fopen("/dev/urandom", "r");
    if(generator==NULL) error("generate", "Can't read generator file", 2);

    for(int i=0; i<rows; i++)   {
        char* buffer = malloc(row_size*sizeof(char));
        
        if(fread(buffer, sizeof(char), (size_t) row_size, generator) != row_size) error("generate", "Can't read generator file", 3);
        
        for(int j=0; j<row_size; j++) buffer[j] = 32+abs(buffer[j])%95;
        
        if(fwrite(buffer, sizeof(char), (size_t) row_size, file) != row_size) error("generate", "Can't write to generator file", 4);

        free(buffer);
    }

    if(fclose(generator)!=0) error("generate", "Can't close generator file", 5);
    if(fclose(file)!=0) error("generate", "Can't close the file", 6);
}

void sort_sys(char* path, int rows, int row_size)  {
    int file = open(path, O_RDWR); //descriptor
    if(file<0)  error("sort_sys", "Can't open the file", 1);

    long offset = row_size * sizeof(char);

    for(int i=0; i<rows; i++)   {
        if(lseek(file, i*offset, SEEK_SET)<0) error("sort_sys", "Can't set position in the file", 2);
        
        char* buffer = malloc(row_size*sizeof(char));
        if(read(file, buffer, (size_t) row_size*sizeof(char)) != row_size) break; //error("sort_sys", "Can't read the file", 3);

        for(int j=0; j<i; j++)  {
            if(lseek(file, j*offset, SEEK_SET)<0) error("sort_sys", "Can't set position in the file", 4);
            char* insert_buffer = malloc(row_size * sizeof(char));

            if(read(file, insert_buffer, (size_t) row_size*sizeof(char)) != row_size) error("sort_sys", "Can't read the file", 5);

            if(buffer[0]<insert_buffer[0])  {
                if(lseek(file, j*offset, SEEK_SET)<0) error("sort_sys", "Can't set position in the file", 6);
                if(write(file, buffer, (size_t) row_size*sizeof(char)) != row_size) error("sort_sys", "Can't write to the file", 7);

                if(lseek(file, i*offset, SEEK_SET)<0) error("sort_sys", "Can't set position in the file", 8);
                if(write(file, insert_buffer, (size_t) row_size*sizeof(char)) != row_size) error("sort_sys", "Can't write to the file", 9);
            
                char* temp = buffer;
                buffer = insert_buffer;
                insert_buffer = temp;
            }
            free(insert_buffer);
        }
        free(buffer);
    }
    if(close(file)!=0) error("sort_sys", "Can't close the file", 10);
}

void sort_lib(char* path, int rows, int row_size)  {
    FILE* file = fopen(path, "r+");
    if(file==NULL)  error("sort_lib", "Can't open the file", 1);

    long offset = row_size * sizeof(char);

    for(int i=0; i<rows; i++)   {
        if(fseek(file, i*offset, 0)!=0) error("sort_lib", "Can't set position in the file", 2);
        
        char* buffer = malloc(row_size*sizeof(char));
        if(fread(buffer, sizeof(char), (size_t) row_size, file) != row_size) error("sort_lib", "Can't read the file", 3);

        for(int j=0; j<i; j++)  {
            if(fseek(file, j*offset, 0)!=0) error("sort_lib", "Can't set position in the file", 4);
            char* insert_buffer = malloc(row_size * sizeof(char));

            if(fread(insert_buffer, sizeof(char), (size_t) row_size, file) != row_size) break; //error("sort_lib", "Can't read the file", 5);

            if(buffer[0]<insert_buffer[0])  {
                if(fseek(file, j*offset, 0)!=0) error("sort_lib", "Can't set position in the file", 6);
                if(fwrite(buffer, sizeof(char), (size_t) row_size, file) != row_size) error("sort_lib", "Can't write to the file", 7);

                if(fseek(file, i*offset, 0)!=0) error("sort_lib", "Can't set position in the file", 8);
                if(fwrite(insert_buffer, sizeof(char), (size_t) row_size, file) != row_size) error("sort_lib", "Can't write to the file", 9);
            
                char* temp = buffer;
                buffer = insert_buffer;
                insert_buffer = temp;
            }
            free(insert_buffer);
        }
        free(buffer);
    }
    if(fclose(file)!=0) error("sort_lib", "Can't close the file", 10);
}

void copy_sys(char* source_path, char* destination_path, int rows, int row_size)   {
    int file_s = open(source_path, O_RDONLY);
    if(file_s<0)  error("copy_sys", "Can't open source file", 1);
    int file_d = open(destination_path, O_WRONLY | O_CREAT, S_IRWXU | O_TRUNC);
    if(file_d<0)  error("copy_sys", "Can't open destination file", 2);

    for(int i=0; i<rows; i++)   {
        char* buffer = malloc(row_size*sizeof(char));
        
        size_t size = read(file_s, buffer, (size_t) row_size*sizeof(char)); 
        if(size!=row_size)  {
            warning("copy_sys", "Error while writing. Not all data was copied", 1);
            write(file_d, buffer, (size_t) size*sizeof(char) );
            break;
        } else if (write(file_d, buffer, (size_t) row_size*sizeof(char) ) != row_size) error("copy_sys", "Can't write to destination file", 3);
    
        free(buffer);
    }
    if(close(file_s)!=0) error("copy_sys", "Can't close source file", 4);
    if(close(file_d)!=0) error("copy_sys", "Can't close destination file", 5);
}

void copy_lib(char* source_path, char* destination_path, int rows, int row_size)   {
    FILE* file_s = fopen(source_path, "r");
    if(file_s==NULL)  error("copy_lib", "Can't open source file", 1);
    FILE* file_d = fopen(destination_path, "w");
    if(file_d==NULL)  error("copy_lib", "Can't open destination file", 2);

    for(int i=0; i<rows; i++)   {
        char* buffer = malloc(row_size*sizeof(char));
        
        size_t size = fread(buffer, sizeof(char), (size_t) row_size, file_s); 
        if(size!=row_size)  {
            warning("copy_lib", "Error while writing. Not all data was copied", 1);
            fwrite(buffer, sizeof(char), (size_t) size, file_d);
            break;
        } else if (fwrite(buffer, sizeof(char), (size_t) row_size, file_d) != row_size) error("copy_lib", "Can't write to destination file", 3);
    
        free(buffer);
    }
    if(fclose(file_s)!=0) error("copy_lib", "Can't close source file", 4);
    if(fclose(file_d)!=0) error("copy_lib", "Can't close destination file", 5);
}