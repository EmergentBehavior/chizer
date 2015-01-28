#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *message;
    double value;
    double p;
} threshold;

threshold criteria[]={
    {"is definitely encrpyted",219.025255,0.95}, 
    {"must be encrpyted",293.247835,0.05}, 
    {"maybe encrpyted",310.457388,0.01}, 
    {"is not encrpyted",316.919385,0.05}, 
    {"is not encrpyted",-1}  
};

#define HIST_SIZE 256
#define BUFFER_SIZE 65535

#define DEBUG 1

typedef struct  {
    long data[HIST_SIZE];
    long count;
} histogram;

double calcChi(histogram *hist){
    double chi=0;
    double expected=(double)hist->count/HIST_SIZE;
    int i=0;
    for(i=0;i<HIST_SIZE;i++){
        double diff=hist->data[i]-expected;
        chi+=(diff*diff)/expected;
    }
    return chi;
}


histogram *getHist(char *fname,histogram *hist){
    FILE *fp=NULL;

    if( (fp=fopen(fname,"rb"))==NULL){
        fprintf(stderr,"cannot open %s\n",fname);
        exit(EXIT_FAILURE);
    }
    int i=0;
    for(i=0;i<HIST_SIZE;i++){
        hist->data[i]=0;
    }
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read=0;
    hist->count=0;
    while( (bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp)) !=0){
        hist->count+=bytes_read;
        int i=0;
        for(i=0;i<bytes_read;i++){
          hist->data[buffer[i]]++;
        }
    }
    fclose(fp);
    return hist;
}

int judge(double chi){
    int i=0;
    for(i=0;i<4;i++){
        if(chi<=criteria[i].value){
            return i;
        }
    }
    return i;
}

int judgeRank(char *fname){
    histogram hist;
    getHist(fname,&hist);
    double chi=calcChi(&hist);
    int rank=judge(chi);
#ifdef DEBUG
    printf("file size=%ld bytes\n",hist.count);
    printf("Chi square distribution=%lf\n",chi);
//    printf("%s %s.\n",fname,criteria[rank].message);
    printf("significance level:%lf\n",criteria[rank].p);
#endif
    return rank;
}

int isEncrypted(char *fname){
  return judgeRank(fname)<=1;
}


int main(int argc,char *argv[]){
    if(isEncrypted(argv[1])){
        printf("%s is encrypted.\n",argv[1]);
    } else{
        printf("%s is NOT encrypted.\n",argv[1]);
    }
    return EXIT_SUCCESS;
}
