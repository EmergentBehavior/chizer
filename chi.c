#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//#define DEBUG 1
#define HIST_SIZE 256
#define BUFFER_SIZE 32 //from article

#define THRESHOLD_FILE 293.247835 //p=0.05
#define THRESHOLD_CHUNK 512 //from article

typedef struct {
    char *message;
    double value;
    double p;
} threshold;


typedef struct  {
    long data[HIST_SIZE];
    long bytes;
} histogram;

double calcChi(histogram *hist){
    double chi=0;
    double expected=(double)hist->bytes/HIST_SIZE;
    int i=0;
    for(i=0;i<HIST_SIZE;i++){
        double diff=hist->data[i]-expected;
        chi+=(diff*diff)/expected;
    }
    return chi;
}

histogram *getHistogram(unsigned char *buffer,histogram *hist){
    int i=0;
    for(i=0;i<HIST_SIZE;i++){
        hist->data[i]=0;
    }
    for(i=0;i<hist->bytes;i++){
      hist->data[buffer[i]]++;
    }
    return hist;
}

int isChunkEncrypted(unsigned char *buffer,int bytes){
    histogram hist;
    hist.bytes=bytes;
    getHistogram(buffer,&hist);
    double chi=calcChi(&hist);

#ifdef DEBUG
    printf("\tchunk size=%ld bytes\n",hist.bytes);
    printf("\tChi square distribution=%lf\n",chi);
#endif
    return chi<THRESHOLD_CHUNK;
}

int isFileEncrypted(char *fname){
    FILE *fp=NULL;

    if( (fp=fopen(fname,"rb"))==NULL){
        fprintf(stderr,"cannot open %s\n",fname);
        exit(EXIT_FAILURE);
    }
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read=0;
    int isEncrypted=1;
#ifdef DEBUG
   long loc=0;
#endif
    while( (bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp)) !=0){
#ifdef DEBUG
        printf("\tfile location:%ld-%ld\n",loc,loc+bytes_read-1);
#endif
        int e=isChunkEncrypted(buffer,bytes_read);
#ifdef DEBUG
        loc+=bytes_read;
        if(e){
            printf(";-)\tchunk is encrpyted\n\n");
        }else{
            printf(":-(\tchunk is NOT encrpyted\n\n");
        }
#endif
        isEncrypted =isEncrypted && e;
    }
    fclose(fp);
    return isEncrypted;
}

int main(int argc,char *argv[]){
    if(isFileEncrypted(argv[1])){
        printf(";-) \tencrypted.\n");
    } else{
        printf(":-( \tNOT encrypted.\n");
    }
    return EXIT_SUCCESS;
}
