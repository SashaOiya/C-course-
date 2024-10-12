#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <deque>
#include <vector>
#include <list>

// name private members as m_field or field_

// + slow_get_page
// + lookup_update

template <typename T, typename U>
class Cache_c {
    std::deque<T> data_vector;
                            // data, cells
    std::unordered_map< T, std::deque<int> > data_hash_table;
    std::vector<T> cache_storage;     
                      // data, irr
    std::unordered_map< T, int> cache_hash_table;

    size_t capacity = 0; // a_
    size_t elements_number = 0;

public :
    // template<typename It> // typename -> std::input_iterator
    Cache_c ( size_t capacity, size_t elements_number/*, It from, It to*/)
        : capacity( capacity ), elements_number ( elements_number )/*, data_vector(from, to)*/ {}

    void reading_data () // divide into a part that reads data from stdin and a part that changes the state of the cache
    {
        data_hash_table.reserve ( elements_number );

        for ( size_t i = 0; i < elements_number; ++i ) {
            T new_element = 0;
            std::cin >> new_element;
            data_vector.push_back ( new_element );

            data_hash_table[new_element].push_back(i);
        }
    }

    const int MAX_IRR = elements_number + 1; // remove

    int cache_processing () // lookup_update in loop, loop in main
    {
        size_t misses_total_n = 0;

        for ( auto itt = data_vector.begin(); itt != data_vector.end(); ++itt ) {
            // get next elem
            T element = std::move(*itt);
            data_vector.pop_front();

            // get irr
            auto irr = cache_hash_table.find (element );
            if ( irr == cache_hash_table.end() ) { ++misses_total_n; }
            auto &element_vector = data_hash_table[element];

            if ( element_vector.size() == 1 && irr == cache_hash_table.end() ) {
                element_vector.pop_front();
                data_hash_table.erase ( element );

                continue;
            }

            size_t element_recency = ( element_vector.size() == 1 ) ? MAX_IRR : element_vector[1] - element_vector[0] - 1;
            element_vector.pop_front();

            if (irr == cache_hash_table.end()) {
                if (cache_storage.size() < capacity) {
                    cache_storage.push_back ( element );
                    cache_hash_table.emplace ( element, element_recency );
                    //assert ( cache_hash_table.contains ( element ) );
                } else {
                    auto [max_irr_it, max_recency] = get_element_with_max_recency ();
                    if ( ( max_recency <= element_recency ) ) {
                        continue;
                    }
                    cache_storage.erase ( std::remove( cache_storage.begin(), cache_storage.end(), *max_irr_it ),
                                          cache_storage.end() );
                    cache_hash_table.erase( *max_irr_it );
                    cache_storage.push_back ( element );
                    cache_hash_table.emplace ( element, element_recency );
                }
            } else {
                cache_hash_table[element] = element_recency;
            }
        }

        return elements_number - misses_total_n;
    }

private:
    auto get_element_with_max_recency ()
    {
        int max_recency = 0;
        auto it = cache_storage.begin();
        auto max_recency_it = it;

        for (auto ite = cache_storage.end(); it != ite; ++it) {
            int current_recency = cache_hash_table[*it];
            if ( current_recency == MAX_IRR ) {
                return std::pair{it, MAX_IRR};
            }
            else if ( current_recency > max_recency ) {
                max_recency = current_recency;
                max_recency_it = it;
            }
        }
        return std::pair{max_recency_it, max_recency};
    }
};
