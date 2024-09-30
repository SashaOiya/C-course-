#include "lirs.hpp"

#define NDEBUG

int main ( int argc, char* argv[] )
{
    //unsigned int start_time =  clock();
    Cache_c<int> cache;
    int output = cache.cache_ctor ( cache, (argc > 1) ? argv[1] : "" );
    
    //unsigned int end_time = clock();
    //printf ( "Successfully. Time = %d s \n", ( end_time - start_time ) / 1000 );

    std::cout << output << '\n';

    return 0;
}