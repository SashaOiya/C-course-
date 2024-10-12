#pragma once

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <deque>
#include <list>

// + slow_get_page

template <typename T, typename U>
class Cache_c {
    std::deque<T> data_storage;
                            // data, cells
    std::unordered_map< T, std::deque<int> > data_hash_table;
                      // data, irr
    std::unordered_map< T, int> cache_hash_table;
    std::list<T> cache_storage;      

    size_t capacity_ = 0; 
    size_t elements_number_ = 0;

public :
    //template <typename It> // typename -> std::input_iterator
    Cache_c ( size_t capacity, size_t elements_number/*, It from, It to*/ ) 
            : capacity_( capacity ), elements_number_ ( elements_number )/*, data_vector(from, to)*/
    {
        data_hash_table.reserve ( elements_number_ );

        for ( size_t i = 0; i < elements_number_; ++i ) {
            T new_element = 0;
            std::cin >> new_element;
            data_storage.push_back ( new_element );
            data_hash_table[new_element].push_back(i);
        }
    }

    //void reading_data () // divide into a part that reads data from stdin and a part that changes the state of the cache
    size_t processing () 
    {
        size_t hits = 0;
        auto begin_itt = data_storage.begin();

        for ( int itt_counter = 0; begin_itt + itt_counter != data_storage.end(); ++itt_counter ) {
            hits += ( lookup_update ( begin_itt[itt_counter] ) ? 1 : 0 );
        }

        return hits;
    }

private:
    const int MAX_IRR = elements_number_ + 1; // remove

    bool lookup_update ( T &key ) 
    {
        auto &element_vector = data_hash_table[key]; 
        size_t element_recency = ( element_vector.size() == 1 ) ? MAX_IRR : element_vector[1] - element_vector[0] - 1;
        data_hash_table[key].pop_front();

        if ( cache_hash_table.find( key ) != cache_hash_table.end() ) { 
            cache_hash_table[key] = element_recency;

            return true; 
        }
        else if ( (data_hash_table.find(key)->second).size() == 0 ) { return false; }

        if ( cache_storage.size() < capacity_ ) {
            cache_storage.push_back ( key );
            cache_hash_table.emplace ( key, element_recency );

            return false;
        } 
        auto [max_irr_it, max_recency] = get_element_with_max_recency ();
        if ( ( max_recency <= element_recency ) ) { return false; }

        cache_storage.erase ( max_irr_it );
        cache_hash_table.erase( *max_irr_it );
        cache_storage.push_back ( key );
        cache_hash_table.emplace ( key, element_recency );

        return false;
    }

    auto get_element_with_max_recency ()
    {
        int max_recency = -1;
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
