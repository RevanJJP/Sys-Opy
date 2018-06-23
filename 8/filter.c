#include "jjp_lib.h"
#include <sys/_pthread/_pthread_t.h>
#include <pthread.h>
#include <memory.h>
#include <math.h>
#include <sys/times.h>
#include <zconf.h>

typedef struct filter_data {
    int** picture;
    int W;
    int W_start;
    int W_end;
    int H;
    int H_start; 
    int H_end;
    float** filter;
    int C;
    int **output;
} filter_data;

void* filtering(void* data);
int** multithread_filtering(int threads, int** picture, int W, int H, float** filter, int C);

int main(int argc, char** argv) {
    if(argc!=5) ERROR(1, "Wrong arguments");
    int W=0, H=0, C=0, depth=0;
    int threads=atoi(argv[1]);

    FILE* picture_file=fopen(argv[2], "r+");
    if(picture_file==NULL)  ERROR(1, "Can't open the picture file");
    FILE* filter_file=fopen(argv[3], "r+");
    if(filter_file==NULL)  ERROR(1, "Can't open the filter file");
    FILE* output_file=fopen(argv[4], "w+");
    if(output_file==NULL)  ERROR(1, "Can't open the ouput file");
    

    char format[4];
    if(fscanf(picture_file, "%s", format) < 1) ERROR(1, "format error");
    if(strcmp(format, "P2")!=0) ERROR(1, "wrong format");


    if(fscanf(picture_file, "%d", &W) < 1) ERROR(1, "reading width error");
    if(fscanf(picture_file, "%d", &H) < 1) ERROR(1, "reading heigth error");
    if(fscanf(picture_file, "%d", &depth) < 1) ERROR(1, "reading depth error");


    int** picture=calloc(H, sizeof(int*));

    for(int i=0; i<H; i++) {
        picture[i]=calloc(W, sizeof(int));
        for(int j=0; j<W; j++) {
            if(fscanf(picture_file, "%d ", &picture[i][j]) < 1) ERROR(1, "reading pixel error");
        }
    }
    if(fscanf(filter_file, "%d", &C) < 1) ERROR(1, "reading filter size error");
    float** filter=calloc(C, sizeof(int*));
    
    for(int i=0; i<C; i++) {
        filter[i]=calloc(C, sizeof(int));
        for(int j=0; j<C; j++) {
            if(fscanf(filter_file, "%f", &filter[i][j]) < 1) ERROR(1, "reading filter data error");
            if(filter[i][j]>1.0) filter[i][j]=1.0;
            if(filter[i][j]<0.0) filter[i][j]=0.0;
        }
    }

    int** output=multithread_filtering(threads, picture, W, H, filter, C);
    
    fprintf(output_file, "P2\n");
    char* buf = malloc(sizeof(int));
    sprintf(buf, "%d %d\n", W, H);
    fprintf(output_file, buf);
    fprintf(output_file, "255\n");
    for(int i=0; i<H; i++) {
        for(int j=0; j<W; j++) {
            sprintf(buf, "%d ", output[i][j]);
            fprintf(output_file, buf);
        }
        fprintf(output_file, "\n");
    }

    free(picture);
    free(filter);
    free(output);

    return 0;
}

int** multithread_filtering(int threads, int** picture, int W, int H, float** filter, int C) {
    struct rusage susage, eusage;
    getrusage(RUSAGE_SELF, &susage);
    
    pthread_t* thread_list = calloc(sizeof(pthread_t), (size_t)threads);
    int** output = calloc(H, sizeof(int*));
    for(int i=0; i<H; i++) {
        output[i] = calloc(W, sizeof(int));
    }

    filter_data data;
    data.picture=picture;
    data.H=H;
    data.W=W;
    data.W_start=0;
    data.W_end=W;
    data.output=output;
    data.filter=filter;
    data.C=C;

    filter_data* tab=calloc(threads, sizeof(filter_data));

    int new_H = H/threads;
    int i=0;
    for(i=0; i<(threads-1); i++) {
        data.H_start=i*new_H;
        data.H_end=(i+1)*new_H;
        tab[i]=data;
        if (pthread_create(&thread_list[i], NULL, filtering, (void*)&tab[i]) != 0) ERROR(1, "Creating thread error");
    }
    data.H_start=i*new_H;
    data.H_end=H;
    tab[i]=data;
    if (pthread_create(&thread_list[i], NULL, filtering, (void*)&tab[i]) != 0) ERROR(1, "Creating thread error");
    
    for(int i=0; i<threads; i++) {
        if (pthread_join(thread_list[i], NULL) != 0) ERROR(1, "Waiting for thread error");
    }
    
    getrusage(RUSAGE_SELF, &eusage);

    print_result(susage, eusage);
        return output;
}


void* filtering(void* data) {
    filter_data *d=(filter_data*)data;
    int c=(int)ceil(d->C/2.0);
    for(int x=d->H_start; x<d->H_end; x++) {
        for(int y=d->W_start; y<d->W_end; y++) {
            double s=0;
            for(int i=0; i<d->C; i++) {
                for(int j=0; j<d->C; j++) { 
                    int a=x-c+i;
                    int b=y-c+j;
                    
                    if(a<0) a=0;
                    else
                    if(a>=d->H) a=d->H-1;
                    
                    if(b<0) b=0;
                    else
                    if(b>=d->W) b=d->W-1;
                    
                    s+=(double)d->picture[a][b]*d->filter[i][j];
                }
            }
            d->output[x][y]=(int)round(s);
        }
    }
    pthread_exit((void*) 0);
}