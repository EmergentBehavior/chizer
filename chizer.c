/*
 Copyright (c) 2015, utamaro
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>

#define HIST_SIZE 256
#define BUFFER_SIZE 65536

#define THRESHOLD_FILE 293.247835 //p=0.05

#define CHUNK_SIZE 32 //from article
#define THRESHOLD_CHUNK 512 //from article
#define THRESHOLD_SUSPECTS 5 //from article


void getHistogram(unsigned char *buffer, long* hist,int bytes){
    int i=0;
    for(i=0;i<bytes;i++){
      hist[buffer[i]]++;
    }
}

double calcChi(long *hist,int bytes){
    int i=0;
    double chi=0;
    double expected=(double)bytes/HIST_SIZE;
    for(i=0;i<HIST_SIZE;i++){
        double diff=hist[i]-expected;
        chi+=(diff*diff);
    }
    chi/=expected;
    return chi;
}

double getFileChi(char *fname){
    FILE *fp=NULL;
    int i=0;
    long hist[HIST_SIZE];
    for(i=0;i<HIST_SIZE;i++) hist[i]=0;

    if( (fp=fopen(fname,"rb"))==NULL){
        fprintf(stderr,"cannot open %s\n",fname);
        exit(EXIT_FAILURE);
    }
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read=0;
    int bytes=0;
    while( (bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp)) !=0){
        bytes+=bytes_read;
        getHistogram(buffer,hist,bytes_read);
    }
    double chi=calcChi(hist,bytes);
#ifdef DEBUG
    printf("\tChi square distribution=%lf\n",chi);
#endif
    return chi;
}

int isFileEncrypted(char *fname){
    return  getFileChi(fname)< THRESHOLD_FILE;
}


int isChunksEncrypted(char *fname){
    int i=0;
    int suspects=0;
    FILE *fp=NULL;

    if( (fp=fopen(fname,"rb"))==NULL){
        fprintf(stderr,"cannot open %s\n",fname);
        exit(EXIT_FAILURE);
    }
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read=0;
    long hist[HIST_SIZE];

#ifdef DEBUG
    FILE *fout=fopen("result.dat","wb");
    long no=0;
#endif
    while( (bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp)) !=0){
        for(i=0;i<bytes_read;i+=CHUNK_SIZE){
            long bytes=bytes_read-i>CHUNK_SIZE?CHUNK_SIZE:bytes_read-i;
            int j=0;
            for(j=0;j<HIST_SIZE;j++) hist[j]=0;
            getHistogram(buffer+i,hist,bytes);
            double chi=calcChi(hist,bytes);
#ifdef DEBUG
            fprintf(fout,"%ld,%lf,%s\n",no,chi,(chi<THRESHOLD_CHUNK? "o":"x"));
            no++;
#endif
            if(chi>=THRESHOLD_CHUNK) suspects++;
        }
    }
    fclose(fp);
#ifdef DEBUG
    fclose(fout);
    printf("\tsuspecious blocks:%d\n",suspects);
#endif
    return suspects<THRESHOLD_SUSPECTS;
}

#ifdef CHI_MAIN 
int main(int argc,char *argv[]){
    if(isFileEncrypted(argv[1])){
        printf(";-) \tWhole File is encrypted.\n");
    } else{
        printf(":-( \tWhole File is NOT encrypted.\n");
    }

    if(isChunksEncrypted(argv[1])){
        printf(";-) \tchunks are encrypted.\n");
    } else{
        printf(":-( \tchunks are NOT encrypted.\n");
    }
    return 0;
}
#endif
