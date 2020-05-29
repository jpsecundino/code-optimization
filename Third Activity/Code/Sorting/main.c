#include<stdio.h>
#include<stdlib.h>
#include "sorting.h"

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define LARGEST_CACHE_SZ (3 * MB)
#define NUM_SORTS 4
#define VEC_SIZE 200000
static unsigned char dummy_buffer[LARGEST_CACHE_SZ];

void cache_clean()
{
    unsigned long long i;
    
    for (i=0; i<LARGEST_CACHE_SZ; i++)
        dummy_buffer[i] += 1;
}

int fill(int *v, int size){
   
    for (int i = 0; i < size; i++)
    {
        v[i] = rand()%VEC_SIZE;
    }
    
}

int isordered(int *v, int size){
    for (int i = 1; i < size; i++)
    {
        if(v[i] < v[i-1]){
            return 0;
        }

        printf("%d ", v[i]);
    }

    return 1;
    
}
int main(int argc, char *argv[]) {
    
    srand(2);
    
    int *v = malloc(sizeof(int)*VEC_SIZE);

        
    fill(v, VEC_SIZE);
    cache_clean();
    radixsort(v, VEC_SIZE);//O(n)
    printf("radix");
    
    fill(v, VEC_SIZE);
    cache_clean();
    heapsort(v, VEC_SIZE);//O(nlogn)
    printf("heap");

    fill(v, VEC_SIZE);
    cache_clean();
    mergesort(v, 0,VEC_SIZE);//O(nlogn)
    printf("merge");

    fill(v, VEC_SIZE);
    cache_clean();
    bubblesort(v, VEC_SIZE);//O(n²)
    printf("bubble");


    free(v);  
 
    return 0;
}