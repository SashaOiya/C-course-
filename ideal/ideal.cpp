#include "ideal.hpp"

int main ( int argc, char* argv[] ) 
{
    //unsigned int start_time = clock();
    using key_type = int;
    using page_type = key_type;

    size_t capacity = 0;
    std::cin >> capacity;
    if ( !std::cin.good() ) {
        std::cerr << "Error while reading the capacity of cache\n";

        return EXIT_FAILURE;
    }
    
    size_t elements_number = 0;
    std::cin >> elements_number;
    if ( !std::cin.good() ) {
        std::cerr << "Error while reading the number of elements\n";

        return EXIT_FAILURE;
    }
    Cache_c<key_type, page_type> cache { capacity, elements_number };

    if ( cache.cache_ctor () != NO_ERRORS ) {
        std::cout << "Cache ctor error \n";

        return EXIT_FAILURE;
    }
    size_t output = cache.cache_processing ();
    cache.output ( output );

    //unsigned int end_time = clock();
    //unsigned int search_time = end_time - start_time;
    //printf ( "Successfully. Time = %d s \n", search_time / 1000 );
$
    return 0;
}