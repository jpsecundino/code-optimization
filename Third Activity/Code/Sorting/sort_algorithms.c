#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#define ROW_SIZE 10
#define COLUMN_SIZE 10
#define DEBUG false

int getMax(int array[], int n) {
    
    int max = array[0];
    
    int i;
    
    for (i = 1; i < n; i++) 
    {
        if (array[i] > max)
            max = array[i];
    }

    return max;
} 

//Function for Count sort
void countSort(int array[], int n, int dig) {
    int output[n]; 
    int i, count[10] = { 0 };
    // Store count of occurrences in count[]
    for (i = 0; i < n; i++){
        count[(array[i] / dig) % 10]++; 
    }

    // Change count[i] so that count[i] now contains actual 
    //  position of this digit in output[] 
    for (i = 1; i < 10; i++) count[i] += count[i - 1]; // Build the output array for (i = n - 1; i >= 0; i--) {
        output[count[(array[i] / dig) % 10] - 1] = array[i];
        count[(array[i] / dig) % 10]--;
    } 

    // Copy the output array to arr[], so that arr[] now 
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++) array[i] = output[i]; } // The main function to that sorts arr[] of size n using Radix Sort void radixsort(int array[], int n) { //Get the largest number to know the maximum number of digits int m = getMax(array, n); int dig; //Counting sort is performed for every digit for (dig = 1; m / dig > 0; dig *= 10)
    
    countSort(array, n, dig);
}

//Function to Print Array
void print(int arr[], int n) {
    int i;

    for (i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
} 

void radixsort(int array[], int n) {
    //Get the largest number to know the maximum number of digits
    int m = getMax(array, n);
    int dig;
    
    //Counting sort is performed for every digit
    for (dig = 1; m / dig > 0; dig *= 10){
        countSort(array, n, dig);
    }
}
///////////////////////////////////////////
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    // Create temp arrays
    int L[n1], R[n2];
    // Copy data to temp array
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    // Merge the temp arrays
    i = 0;
    j = 0;
    k = l;
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
    // Copy the remaining elements of L[]
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R[]
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Finding mid element
        int m = l+(r-l)/2;
        // Recursively sorting both the halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        
        // Merge the array
        merge(arr, l, m, r);
    }
}
int main(int argc, char *argv[]) {
    
    switch (strtol(argv[1], NULL, 10))
    {
        case 0:
            radixsort(v);//O(n)
            break;
        case 1:
            heapsort(v);//O(nlogn)
            break;
        case 2:
            mergesort(M1, M2);//O(nlogn)
            break;
        case 3:
            bubblesort(M1, M2);//O(n²)
            break;
        default:
            break;
    }

    return 0;
}