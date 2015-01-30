#include <stdio.h>
#include <string.h>
#include <tap.h>
#include "chizer.h"

#define BUFFER_SIZE 65535

void compareChunkChi(){
    int i=0;
    char *fname1="result.dat";
    char *fname2="data/faked_chi.dat";

    FILE *fp1=NULL,*fp2=NULL;
    fp1=fopen(fname1,"rb");
    ok(fp1!=NULL,"file %s is found or not",fname1);

    fp2=fopen(fname2,"rb");
    ok(fp2!=NULL,"file %s is found or not",fname2);

    if(fp1==NULL || fp2==NULL){
        return;
    }

    int bytes_read1=0;
    int bytes_read2=0;
    unsigned char buffer1[BUFFER_SIZE];
    unsigned char buffer2[BUFFER_SIZE];

    int isOK=1;

    while(isOK && (bytes_read1 = fread(buffer1, sizeof(unsigned char), BUFFER_SIZE, fp1)) !=0){
        bytes_read2 = fread(buffer2, sizeof(unsigned char), BUFFER_SIZE, fp2);
        isOK=isOK & (bytes_read1==bytes_read2);
        
        for(i=0;i<bytes_read1;i++){
            isOK=isOK & (buffer1[i]==buffer2[i]);
        }
    }
    ok(isOK,"correctness test of each chunks distribution");
}

int main () {
    char *buffer="hataraitara makekana to omotteiru. I believe I'm loss if I work. This is a test sentence. Be Storj with you!\n";
    long hist[256];
    int i=0;
    for(i=0;i<256;i++) hist[i]=0;

    getHistogram((unsigned char*)buffer,hist,strlen(buffer));
    ok(hist[' '] == 19,"simple sentence histogram test #space");
    ok(hist['e'] == 10,"simple sentence histogram test #e");
    ok(hist['z'] == 0, "simple sentence histogram test #z");
    ok(hist['\n'] == 1,"simple sentence histogram test #CR");

    double chi=calcChi(hist,strlen(buffer));
    ok( 1955.43<chi && chi<1955.45,"chi test for a simple sentence");

    chi=getFileChi("data/ac59ab5a282afd3de22062c7d62b5367");
    ok( 236.71<chi && chi<236.73,"chi test for 30MB file");
    
    int isEncrypted=isFileEncrypted("data/ac59ab5a282afd3de22062c7d62b5367");
    ok( isEncrypted,"whole file encryption detector test for 30MB encrypted file");

    dies_ok({isFileEncrypted("nofile");},"no exist file for isFileEncrypted function");
    dies_ok({isChunksEncrypted("nofile");},"no exist file for isChunksEncrypted function");

    isEncrypted=isFileEncrypted("data/Storj - Decentralizing Cloud Storage-vl3bUzfn2lg.mp4.gz");
    ok( !isEncrypted,"while file encryption detector test for gzipped Storj movie file");

    isEncrypted=isChunksEncrypted("data/ac59ab5a282afd3de22062c7d62b5367");
    ok( isEncrypted,"chunk encryption detector test for 30MB encrypted file");

    isEncrypted=isChunksEncrypted("data/Storj - Decentralizing Cloud Storage-vl3bUzfn2lg.mp4.gz");
    ok( !isEncrypted,"chunk encryption detector test for Storj movie file");

    isEncrypted=isChunksEncrypted("data/faked.dat");
    ok( !isEncrypted,"chunk encryption detector test for 30MB encrypted file + Storj sound file");

    compareChunkChi();

    done_testing();
}
