#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>




//define vars
//Max File Size
#define max_str_len             255
int **tagArray;
int **lruArray;
//int manySet;//how many set
//int manyLine;//how many line
int n_hit = 0;
int n_miss = 0;

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

// TODO  this should not be need , same as hitway()
int getLine(u_int32_t C,u_int32_t L,u_int32_t K){
    int index = setIndexLength(C,L,K);
    int sizeOfSet = sizeof(C*1000/(L*K)-1);
    int line = ((index<<sizeOfSet)>>sizeOfSet);
    return line;
}

//TODO   return -1 if no tag match, return which line if hit
//   if hit return  which line hit
//   else return -1
int hitway(u_int32_t tag,u_int32_t set,u_int32_t K){

    for (int i=0;i<K;i++ ){
        if(tagArray[set][i]==tag && lruArray[set][i]!=-1) {
            n_hit=n_hit+1;
            return i;
        }
    }
        n_miss = n_miss+1;
    return -1;
}
void increLRU(u_int32_t set,u_int32_t K){
    for (int j=0;j<K;j++){
       if( (lruArray[set][j])!= -1);
        lruArray[set][j]++;
}
}
// TODO  only need to update lruarray
void updateOnHit(u_int32_t set ,u_int32_t line,u_int32_t K){
    increLRU(set,K);
    lruArray[set][line] = 0;

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
// TODO  swich index that has highest LRU  with new addrs update tag and set it's LRU =0
    void updateOnMiss(u_int32_t tag, u_int32_t set,u_int32_t K ){//tag, address, C, L, K
        int i=0 , max=0, index=0;
        increLRU(set,K);
        while(i<K){
            if (lruArray[set][i]==-1){
                tagArray[set][i] =tag;
                lruArray[set][i] =0;
                return;
            }
          else if (max<lruArray[set][i] ){
               max = lruArray[set][i];
               index = i;
           }
        }
        tagArray[set][index] =tag;
        lruArray[set][index] =0;
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


   /*  for test
    int K=4;
    int L=1;
    int C=1*1024;
*/

    //vars
    char hexa [max_str_len];
    u_int32_t decimal ;
    int i,j;

    //cache structure
    int n_set=C/(L*K);//how many set in cash

    tagArray=(int**)malloc((n_set)*sizeof(int *));//[set][line]
    for(int j=0;j<n_set+1;j++){
        tagArray[j]=(int*)malloc(K*sizeof(int));//each set has K line
    }

    lruArray=(int**)malloc((n_set)*sizeof(int *));//[set][line]
    for( j=0;j<n_set+1;j++){
        lruArray[j]=(int*)malloc(K*sizeof(int));//each set has K line
    }
    //initialize lruArray to -1
    for (i = 0; i <  n_set; i++)
        for (j = 0; j < K; j++)
            lruArray[i][j] = -1;




    //Open file
    FILE *file = fopen("sample.txt","r");//Todd's test
   // printf("%s\n",argv[4]);
    if (file == NULL){
        printf("unable to open the file");
        exit(0);
    }


    //scan traces and access cache
    while(fscanf(file,"%s",&hexa[0])!=EOF) {

        //show data
        printf("hex : %s\n", hexa);
        //convert Hex string to int
        u_int32_t decimal = (u_int32_t) strtol(hexa, NULL, 16);
        printf("decimal : %u\n", decimal);


        //whichSet
        int wSet = whichSet(decimal, C, L, K);
        //tagBits
        int tBits = tagBits(decimal, C, L, K);

        //hit ?
        int hit = hitway(tBits,wSet,K);

        // updates
          if (hit == -1){
            updateOnMiss(tBits,wSet,K);
        }
          else{
            updateOnHit(wSet,hit,K );
        }

        //TODO calculate miss rate


        //updateOnMiss(tagBits(decimal,C,L,K),decimal,C,L,K);//todd's test
    }
    printf("miss count: %d", missCount);//print mis count


    free(tagArray);
    free(lruArray);
    return 0;
}

