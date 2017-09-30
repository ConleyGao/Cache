#include <stdio.h>
#include <stdlib.h>

#include <string.h>




//define vars
//Max File Size
#define max_str_len             255
unsigned int **tagArray;
int **lruArray;
//int manySet;//how many set
//int manyLine;//how many line
int n_hit = 0;//hit count
int n_access = 0;//access count

int nbits (u_int32_t x){//log2()
    int n = x-1;
    int bits=0;
    while(n>0){
        n=n>>1;
        bits+=1;
    }
    return bits;
}

int setIndexLength(u_int32_t C,u_int32_t L,u_int32_t K){
    u_int32_t nSet = C/L/K;
    return nbits (nSet);
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
    return nbits(L);
}


int tagBits(u_int32_t x ,u_int32_t C,u_int32_t L,u_int32_t K){
    u_int32_t  addr=x;
    addr =addr >> (setIndexLength(C,L,K)+offsetLength(L));
    return addr;
}


//TODO   return -1 if no tag match, return which line if hit
//   if hit return  which line hit
//   else return -1
u_int32_t hitway(u_int32_t tag,u_int32_t set,u_int32_t K) {
    n_access++;//counting access time _
    u_int32_t result=(u_int32_t )-1;//return value
    for (u_int32_t i = 0; i < K; i++) {
        if (lruArray[set][i] >= 0) {
            if (tagArray[set][i] == tag) {
                n_hit = n_hit + 1;
                result=i;
            }
        }
}
    return result;
}
//updating lru array if not empty, for both hit and miss
    void increLRU(u_int32_t set, u_int32_t K) {
        for (int j = 0; j < K; j++) {
            if ((lruArray[set][j]) != -1);
            lruArray[set][j]++;
        }
    }
// TODO  only need to update lruarray
    void updateOnHit(u_int32_t set, u_int32_t line, u_int32_t K) {//if it's a hit, update the LRU value
        increLRU(set, K);//all LRU value add one
        lruArray[set][line] = 0;// set the LRUvalue of the hit line to 0

    }

// TODO  swich index that has highest LRU  with new addrs update tag and set it's LRU =0
    void updateOnMiss(u_int32_t tag, u_int32_t set, u_int32_t K) {//tag, address, K
        int i = 0, max = 0, index = 0;//i for loop, max for max lru, index is o/p line #
        increLRU(set, K);//update lru first, won't be affected by tag update
        while (i < K) {
            if (lruArray[set][i] == -1) {//if empty
                tagArray[set][i] = tag;
                lruArray[set][i] = 0;
                return;
            } else if (max < lruArray[set][i]) {//find max lru
                max = lruArray[set][i];
                index = i;
            }
            i++;
        }
        //update
        tagArray[set][index] = tag;
        lruArray[set][index] = 0;
    }

    int main(int argc, char *argv[]) {
        //argv takes [0]main.c [1]K, [2]L,[3]C [4]traceFile
        u_int32_t K = (u_int32_t) argv[1];//line per set
        u_int32_t L = (u_int32_t) argv[2];//line size
        u_int32_t C = (u_int32_t) argv[3] * 1024;//cache size in Byte, KB=1025 bytes

        printf("Trace=%s, K=%d, L=%d, C=%d, ", argv[4], K, L, C);//printing needed stuff

        //vars
        char hexa[max_str_len];
        int i, j;

        //initializing cache structure
        int n_set = C / (L * K);//how many set in cash

        tagArray = (unsigned int **) malloc((n_set) * sizeof(unsigned int *));//[set][line]
        for (j = 0; j < n_set + 1; j++) {
            tagArray[j] = (unsigned int *) malloc(K * sizeof(unsigned int));//each set has K line
        }

        lruArray = (int **) malloc((n_set) * sizeof(int *));//[set][line]
        for (j = 0; j < n_set + 1; j++) {
            lruArray[j] = (int *) malloc(K * sizeof(int));//each set has K line
        }
        //initialize lruArray to -1
        for (i = 0; i < n_set; i++)
            for (j = 0; j < K; j++)
                lruArray[i][j] = -1;

        //Open file
        FILE *file = fopen(argv[4], "r");//Todd's test
        // printf("%s\n",argv[4]);
        if (file == NULL) {
            printf("unable to open the file");
            exit(0);
        }


        //scan traces and access cache
        while (fscanf(file, "%s", &hexa[0]) != EOF) {

            //show data
            //printf("hex : %s\n", hexa);
            //convert Hex string to int
            u_int32_t decimal = (u_int32_t) strtol(hexa, NULL, 16);

            //printf("decimal : %u\n", decimal);


            //whichSet
            u_int32_t wSet = (u_int32_t) whichSet(decimal, C, L, K);
            //tagBits
            u_int32_t tBits = (u_int32_t) tagBits(decimal, C, L, K);
            //hit ?
            u_int32_t hit = (u_int32_t) hitway(tBits, wSet, K);

            // updates
            if (hit == -1) {
                updateOnMiss(tBits, wSet, K);
            } else {
                updateOnHit(wSet, hit, K);
            }

        }
        double missrate = 1-(n_hit/n_access);// miss rate = miss/access
        printf("miss rate = %f\n", missrate);

        //free memory
        free(tagArray);
        free(lruArray);
        return 0;
    }

