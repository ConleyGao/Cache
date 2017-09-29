
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//define vars
//Max File Size
#define max_str_len             1000
int **tagArray;
int **lruArray;


  char* hexTobinary(char *hexa, char *binarynum){
    int i =0;
    int nbytes = 0;
    while(hexa[i]){
        switch (hexa[i])
        {
            case '0':
                nbytes += sprintf(binarynum+nbytes,"0000" );
                break;
            case '1':
                nbytes += sprintf(binarynum+nbytes,"0001" );
                break;
            case '2':
                nbytes += sprintf(binarynum+nbytes,"0010" );
                break;
            case '3':
                nbytes += sprintf(binarynum+nbytes,"0011" );
                break;
            case '4':
                nbytes += sprintf(binarynum+nbytes,"0100" );
                break;
            case '5':
                nbytes += sprintf(binarynum+nbytes,"0101" );
                break;
            case '6':
                nbytes += sprintf(binarynum+nbytes,"0110" );
                break;
            case '7':
                nbytes += sprintf(binarynum+nbytes,"0111" );
                break;
            case '8':
                nbytes += sprintf(binarynum+nbytes,"1000" );
                break;
            case '9':
                nbytes += sprintf(binarynum+nbytes,"1001" );
                break;
            case 'A':
                nbytes += sprintf(binarynum+nbytes,"1010" );
                break;
            case 'B':
                nbytes += sprintf(binarynum+nbytes,"1011" );
                break;
            case 'C':
                nbytes += sprintf(binarynum+nbytes,"1100" );
                break;
            case 'D':
                nbytes += sprintf(binarynum+nbytes,"1101" );
                break;
            case 'E':
                nbytes += sprintf(binarynum+nbytes,"1110" );
                break;
            case 'F':
                nbytes += sprintf(binarynum+nbytes,"1111" );
                break;
            case 'a':
                nbytes += sprintf(binarynum+nbytes,"1010" );
                break;
            case 'b':
                nbytes += sprintf(binarynum+nbytes,"1011" );
                break;
            case 'c':
                nbytes += sprintf(binarynum+nbytes,"1100" );
                break;
            case 'd':
                nbytes += sprintf(binarynum+nbytes,"1101" );
                break;
            case 'e':
                nbytes += sprintf(binarynum+nbytes,"1110" );
                break;
            case 'f':
                nbytes += sprintf(binarynum+nbytes,"1111" );
                break;
            default:
                printf("\n Invalid hex input %c ", hexa[i]);
                return 0;
        }
        i++;
    }
    binarynum[nbytes] = '\0';
    return binarynum;
}
int power2 (u_int32_t x){
    unsigned int bits;
    while(x>0){
        x=x>>1;
        bits+=1;
    }
    return bits;
}

int whichSet(u_int32_t x,u_int32_t C,u_int32_t L,u_int32_t K ){
    u_int32_t  addr=x;
    addr>>offsetLength(L);
    int mask =0;
    for (int i =0, i<setIndexLength(C,L,K),mask
    while(i<setIndexLength(C,L,K)){

    }
}

int offsetLength(u_int32_t L){
   return power2 (L);
}

int setIndexLength(u_int32_t C,u_int32_t L,u_int32_t K){
    u_int32_t nSet = C/L/K;
    return power2 (nSet);
}

int tagBits(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    u_int32_t  addr=x;
    addr>> (setIndexLength(C,L,K)+offsetLength(L));
    return addr;
}

void updateOnMiss(u_int32_t tag, u_int32_t x, u_int32_t C,u_int32_t L,u_int32_t K ){
    int setNum= whichSet();//waiting for whichset()

}
void MissLru(){}

void MissTag(int set, int line){

}
int main(int argc, char *argv[]) {
    argv takes [0]main.c [1]K, [2]L,[3]C [4]traceFile
    int K = int(argv[1]);
    int L = int(argv[2]);
    int C = int(argv[3]);//shabi, this is KB
    sprintf(argv[4],"C:\\Users\\haoga\\OneDrive\\com.sys\\sampleTrace.txt");
    char *tracefile = argv[4];

    //bit shift
    int offset  =;


    //cache structure
    int set=C*1000/(L*K)-1;//how many set in cash
    tagArray=(int**)malloc((set+1)*max_str_len);//[set][line]
    for(int j=0;j<set+1;j++){
        *(tagArray+j)=(int*)malloc(K*sizeof(int));//each set has K line
    }

    lruArray=int**)malloc((set+1)*max_str_len);//[set][line]
    for(int j=0;j<set+1;j++){
        *(lruArray+j)=(int*)malloc(K*sizeof(int));//each set has K line
    }

    char hexa [max_str_len];
    //char binarynum[max_str_len];

    //Open file
    FILE *file;
    file = fopen(tracefile,"r");
    printf("%s\n",argv[4]);
    if (file == NULL){
        printf("unable to open the file");
        exit(0);
    }

    //scan traces and access cache
    while(fscanf(file,"%s",&hexa[0])!=EOF){
        printf("hex : %s\n",hexa);
        hexTobinary(hexa,binarynum);
        printf("binary :%s\n",binarynum);
    }


    return 0;
}