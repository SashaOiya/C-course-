#include "lirs.h"

#define NDEBUG

int main ( int argc, char* argv[] )
{
    unsigned int start_time =  clock();

    Cache_c<int> cache;
    int output = cache.cache_ctor ( cache, (argc > 1) ? argv[1] : nullptr );
    
    unsigned int end_time = clock();

    std::cout << "OUTPU : " << output << '\n';
    printf ( "Successfully. Time = %d s \n", ( end_time - start_time ) / 1000 );

    return 0;
}