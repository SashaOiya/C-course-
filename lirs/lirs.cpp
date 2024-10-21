#include "lirs.hpp"

int main ()
{
    using key_type = int;
    using page_type = key_type;

    size_t capacity = 0;
    std::cin >> capacity;
    if ( !std::cin.good() ) { 
        std::cout << "Error while reading the capacity of cache\n"; 

        return EXIT_FAILURE;
    }
    
    size_t elements_number = 0;
    std::cin >> elements_number;
    if ( !std::cin.good() ) { 
        std::cout << "Error while reading the number of elements\n";

        return EXIT_FAILURE;
    }

    LirsCache<key_type, page_type> cache {capacity, elements_number};

    int output = 0;
    key_type key = 0;
    for ( size_t i = 0; i < elements_number; ++i ) { 
        std::cin >> key;
        if ( std::cin.good() ) { output += cache.lookup_update(key); } 
        else { 
            std::cout << "Error while reading key\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << output << '\n';
    
    return 0;
}