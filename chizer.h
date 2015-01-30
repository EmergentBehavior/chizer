void getHistogram(unsigned char *buffer, long* hist,int bytes);
double calcChi(long *hist,int bytes);
double getFileChi(char *fname);
int isFileEncrypted(char *fname);
int isChunksEncrypted(char *fname);
