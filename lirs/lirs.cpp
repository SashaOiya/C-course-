#include "lirs.hpp"

#define NDEBUG

int main ()
{
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
    Cache_c<key_type, page_type> cache {capacity, elements_number};

    int output = cache.process_data ();
    std::cout << output << '\n';
    
    return 0;
}