#include "lirs.hpp"

#define NDEBUG

int main ( int argc, char* argv[] )
{
    //unsigned int start_time =  clock();
    Cache_c<int> cache;
    int output = 0;
    if ( ( output = cache.cache_ctor ( cache, (argc > 1) ? argv[1] : "" ) ) < 0 ) {

        return EXIT_FAILURE;
    }
    cache.output ( cache, output, argv[1] );
    
    //unsigned int end_time = clock();
    //printf ( "Successfully. Time = %d s \n", ( end_time - start_time ) / 1000 );

    return 0;
}