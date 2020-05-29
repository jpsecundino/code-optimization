#include "sorting.h"
#include <stdlib.h>

void swap(int *a, int *b){ 

    int temp = *a; 

    *a = *b; 

    *b = temp; 

} 

void bubblesort(int *v, int n){ 

    if (n < 1)return; 

 

    for (int i=0; i<n; i++) 

        if (v[i] > v[i+1]) 

            swap(&v[i], &v[i+1]);  

    bubblesort(v, n-1); 

} 

void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergesort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergesort(arr, l, m); 
        mergesort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

void heapsort(int a[], int n) {
   int i = n / 2, dad, son, t;
   while(1) {
      if (i > 0) {
          i--;
          t = a[i];
      } else {
          n--;
          if (n <= 0) return;
          t = a[n];
          a[n] = a[0];
      }
      dad = i;
      son = i * 2 + 1;
      while (son < n) {
          if ((son + 1 < n)  &&  (a[son + 1] > a[son]))
              son++;
          if (a[son] > t) {
             a[dad] = a[son];
             dad = son;
             son = dad * 2 + 1;
          } else {
             break;
          }
      }
      a[dad] = t;
   }
}

void radixsort(int vetor[], int size) {
    int i;
    int *b;
    int maior = vetor[0];
    int exp = 1;

    b = (int *)calloc(size, sizeof(int));

    for (i = 0; i < size; i++) {
        if (vetor[i] > maior)
    	    maior = vetor[i];
    }

    while (maior/exp > 0) {
        int bucket[10] = { 0 };
    	for (i = 0; i < size; i++)
    	    bucket[(vetor[i] / exp) % 10]++;
    	for (i = 1; i < 10; i++)
    	    bucket[i] += bucket[i - 1];
    	for (i = size - 1; i >= 0; i--)
    	    b[--bucket[(vetor[i] / exp) % 10]] = vetor[i];
    	for (i = 0; i < size; i++)
    	    vetor[i] = b[i];
    	exp *= 10;
    }

    free(b);
}
