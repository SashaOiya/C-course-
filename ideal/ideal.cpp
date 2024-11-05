#include <iostream>
#include "ideal.hpp"

int slow_get_page_int ( int key )
{
    return key;
}

int main ()
{
    using key_type = int;
    using page_type = key_type;

    int capacity = 0;
    std::cin >> capacity;
    if ( !std::cin.good() || capacity < 0 ) {
        std::cout << "Error while reading the capacity of cache\n";

        return EXIT_FAILURE;
    }

    int elements_number = 0;
    std::cin >> elements_number;
    if ( !std::cin.good() || elements_number < 0 ) {
        std::cout << "Error while reading the number of elements\n";

        return EXIT_FAILURE;
    }

    std::deque<key_type> key_storage = {};
    for ( int i = 0; i < elements_number; ++i ) {
        key_type key = 0;
        std::cin >> key;
        if ( !std::cin.good() ) { key_storage.push_back ( key ); }
        else {
            std::cout << "Error while reading key\n";
            return EXIT_FAILURE;
        }
    }

    cachei::IdealCache<key_type, page_type> cache { capacity, elements_number, key_storage.begin(), (key_storage.end()), slow_get_page_int };
    auto begin_itt = key_storage.begin(), end_itt = key_storage.end();

    size_t hits = 0;
    for ( int itt_counter = 0; begin_itt + itt_counter != end_itt; ++itt_counter ) {
        hits += cache.lookup_update ( begin_itt[itt_counter] );
    }
    std::cout << hits << '\n';

    return 0;
}