#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define LARGEST_CACHE_SZ (3 * MB)

static unsigned char dummy_buffer[LARGEST_CACHE_SZ];

void cache_clean()
{
    unsigned long long i;
    
    for (i=0; i<LARGEST_CACHE_SZ; i++)
        dummy_buffer[i] += 1;
}