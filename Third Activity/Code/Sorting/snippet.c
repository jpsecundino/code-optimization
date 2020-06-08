...
    fill(v, VEC_SIZE); // fills an array with values between 0 and array.size
    cache_clean(); // cleans the cache before running sorting algorithm
    radixsort(v, VEC_SIZE);//O(n)
    printf("radix");
    
    fill(v, VEC_SIZE); // fills an array with values between 0 and array.size
    cache_clean(); // cleans the cache before running sorting algorithm
    heapsort(v, VEC_SIZE);//O(nlogn)
    printf("heap");

    fill(v, VEC_SIZE); // fills an array with values between 0 and array.size
    cache_clean(); // cleans the cache before running sorting algorithm
    mergesort(v, 0,VEC_SIZE);//O(nlogn)
    printf("merge");

    fill(v, VEC_SIZE); // fills an array with values between 0 and array.size
    cache_clean(); // cleans the cache before running sorting algorithm
    bubblesort(v, VEC_SIZE);//O(n²)
    printf("bubble");
...