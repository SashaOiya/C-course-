#include "ideal.hpp"

int main ( int argc, char* argv[] ) 
{
    //unsigned int start_time = clock();
    Cache_c<int> cache = {};

    if ( cache.cache_ctor ( cache, (argc > 1) ? argv[1] : "" ) != NO_ERRORS ) {
        std::cout << "Cache ctor error \n";

        return EXIT_FAILURE;
    }
    size_t output = cache.cache_processing ( cache );
    std::cout << output << '\n';

    //unsigned int end_time = clock();
    //unsigned int search_time = end_time - start_time;
    //printf ( "Successfully. Time = %d s \n", search_time / 1000 );
$
    return 0;
}