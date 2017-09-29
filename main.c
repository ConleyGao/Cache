#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>




//define vars
//Max File Size
#define max_str_len             1000
int **tagArray;
int **lruArray;
int manySet;//how many set
int manyLine;//how many line
int missCount=0;//miscount




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




int nbits (u_int32_t x){//log2()
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

int hitway(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    int hit = -1;
    int tag = tagBits(x,C,L,K);
    if(tag == tagArray[whichSet(x,C,L,K)][getLine(x,C,L,K)]){
        hit = 1;
    }
    return hit;
}
int getLine(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    int index = setIndexLength(C,L,K);
    int sizeOfSet = sizeof(C*1000/(L*K)-1);
    int line = ((index<<sizeOfSet)>>sizeOfSet);
    return line;
}

void updateOnHit(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    int s = whichSet(x,C,L,K);
    int l = getLine(x,C,L,K);
    lruArray[s][l]= 0;//reset the lru value
    for(int set = 0; set<C*1000/(L*K)-1; set++){//others in the cache lru value ++
        for(int line = 0;line< K; line ++){
            if(set!=s&&line!=l){
                lruArray[set][line]++;
            }
        }
    }

    //counting
    missCount++;
}


/*
int setaddress(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    int lineNum = x<<(31-offsetLength(L)-setIndexLength(C,L,K));//get rid of tag
    lineNum = lineNum >> (31-setIndexLength(C,L,K));//shifting set address to the right
    int a=0x7fffffff>>(31-setIndexLength(C,L,K));
    lineNum=lineNum&a;//get rid of 2s complement
    return lineNum;
}
*/

void updateOnMiss(u_int32_t tag, u_int32_t x, u_int32_t C,u_int32_t L,u_int32_t K ){//tag, address, C, L, K
    missCount++;//counting
    int setNum = whichSet(x,C,L,K);//waiting for whichset()
    //get set address
    //int line=setaddress(x,C,L,K)-L*setNum+1;//getting which line, L start with 1
    int line =  MissTag(setNum, tag, K);//updatedata
    MissLru(setNum,line, manySet,manyLine);//updatedata

    //debug
    printf("address %d, tag %d, in array %d line:%d\n", x, tag, setNum,line);
    printf("[0][0]: %d\n", lruArray[0][0]);

}

void MissLru(int set, int line, int manySet, int manyLine){
    for(int j=0; j<manySet; j++){//set# sweep
        for(int i=0; i<manyLine; i++){//line# sweep
            if((j!=set)&&(i!=line)){//lruArray++ if this line not been called
                lruArray[j][i]++; 
            }
        }
    }
}


int MissTag(int set, int tag, int K){//pushing tag into tag array and return which line it pushed into
    int r=0;//return which line it took in
    int temp=0;//for testing
    int lru=0;//lru comparing
    int a=0;//storing biggest lru of j
    int check=0;
    for(int j=0;j<K;j++){
        temp=tagArray[set][j];
        if(!temp){//if empty
            tagArray[set][j]=tag;
            lruArray[set][j]=0;
            r=j;
            check=!check;
        } else{
            if(lru<lruArray[set][j]) {
                a=j;
                lru = lruArray[set][j];
            }
        }
    }
    if(!check){
        tagArray[set][a]=tag;
        lruArray[set][a]=0;
        r=a;
    }
    return r;
}

int main(int argc, char *argv[]) {
    //argv takes [0]main.c [1]K, [2]L,[3]C [4]traceFile
   // int K = int(argv[1]) ;//line per set
    //int L = int(argv[2]);//line size
    //int C = int(argv[3])*1024;//cache size in Byte
    //sprintf(argv[4],"C:\\Users\\haoga\\OneDrive\\com.sys\\sampleTrace.txt");
/*
    int K=4;
    int L=1;
    int C=1*1024;
*/
    //char *tracefile = argv[4];


    //bit shift
    int offset  =nbits(L);




    //cache structure
    int set=C/(L*K)-1;//how many set in cash
    manySet=set;
    manyLine=K;
    tagArray=(int**)malloc((set+1)*max_str_len);//[set][line]
    for(int j=0;j<set+1;j++){
        *(tagArray+j)=(int*)malloc(K*sizeof(int));//each set has K line
    }

    lruArray=(int**)malloc((set+1)*max_str_len);//[set][line]
    for(int j=0;j<set+1;j++){
        *(lruArray+j)=(int*)malloc(K*sizeof(int));//each set has K line
    }


    //trace
    char hexa [max_str_len];
    u_int32_t decimal ;


    //Open file
    FILE *file;
    //file = fopen(tracefile,"r");
    file = fopen("sample.txt","r");//Todd's test
   // printf("%s\n",argv[4]);
    if (file == NULL){
        printf("unable to open the file");
        exit(0);
    }


    //scan traces and access cache
    while(fscanf(file,"%s",&hexa[0])!=EOF){
        printf("hex : %s\n",hexa);
        u_int32_t decimal =(u_int32_t)strtol(hexa, NULL, 16);
        printf("decimal : %u\n",decimal);
        //updateOnMiss(tagBits(decimal,C,L,K),decimal,C,L,K);//todd's test
    }
    printf("miss count: %d", missCount);//print mis count


    free(tagArray);
    free(lruArray);
    return 0;
}

