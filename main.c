
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//define vars
//Max File Size
#define max_str_len             1000

int nbits (u_int32_t x){
    int n = x-1;
    int bits=0;
    while(n>0){
        n=n>>1;
        bits+=1;
    }
    return bits;
}

int whichSet(u_int32_t x,u_int32_t C,u_int32_t L,u_int32_t K ){
    int offset =setIndexLength(C,L,K);
    u_int32_t  addr=x>>offset;
    int mask =0;
    int i = 0;
    while(i<offset){
        mask =mask<<1 | 1;
        i++;
    }
    printf("%x\n",mask);
    return  addr & mask;
}
int offsetLength(u_int32_t L){
    return nbits (L);
}

int setIndexLength(u_int32_t C,u_int32_t L,u_int32_t K){
    u_int32_t nSet = C/L/K;
    return nbits (nSet);
}
int tagBits(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    u_int32_t  addr=x;
    addr =addr >> (setIndexLength(C,L,K)+offsetLength(L));
    return addr;
}

int main(int argc, char *argv[]) {
    //argv takes [0]main.c [1]K, [2]L,[3]C [4]traceFile
    //int K = int(argv[1]) ;
    //int L = int(argv[2]);
    //int C = int(argv[3])*1000;

    //trace
    char hexa [max_str_len];
    u_int32_t decimal ;
    printf("test");
    //Open file
    FILE *file;
    file = fopen("C:\\Users\\haoga\\CLionProjects\\Cache\\sampleTrace.txt","r");

    if (file == NULL){
        printf("unable to open the file");
        exit(0);
    }

    //scan traces and access cache
    while(fscanf(file,"%s",&hexa[0])!=EOF){
        printf("hex : %s\n",hexa);
        decimal =(u_int32_t)strtol(hexa, NULL, 16);
        printf("decimal : %u\n",decimal)
                
    }


    return 0;
}