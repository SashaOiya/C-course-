#include "ideal.hpp"

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

    std::deque<key_type> key_storage = {}; // vector
    for ( size_t i = 0; i < elements_number; ++i ) {
        key_type key = 0;
        std::cin >> key;
        key_storage.push_back ( key );
    }

    IdealCache<key_type, page_type> cache { capacity, elements_number, key_storage.begin(), (key_storage.end()) };
    size_t hits = 0;
    auto begin_itt = key_storage.begin();

    for ( int itt_counter = 0; begin_itt + itt_counter != key_storage.end(); ++itt_counter ) {
        hits += cache.lookup_update ( begin_itt[itt_counter] );
    }
    std::cout << hits << '\n';

    return 0;
}