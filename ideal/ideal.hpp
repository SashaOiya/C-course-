#pragma once

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <deque>
#include <list>

template <typename T, typename U>
class IdealCache {
    std::deque<T> data_storage;
                            // data, cells
    std::unordered_map< T, std::deque<int> > data_hash_table;
                      // data, irr
    std::unordered_map< T, int> cache_hash_table;
    std::list<T> cache_storage;      

    size_t capacity_ = 0; 
    size_t elements_number_ = 0;

public :
    using itt = typename std::deque<T>::iterator;
    IdealCache ( size_t capacity, size_t elements_number, itt from, itt to ) 
            : capacity_( capacity ), elements_number_ ( elements_number )
    {
        data_hash_table.reserve ( elements_number_ );

        for ( size_t i = 0; from != to; ++from, ++i ) {
            std::fill_n(std::back_inserter(data_storage), 1, *from );
            data_hash_table[*from].push_back(i);
        }
    }

    bool lookup_update ( const T &key ) 
    {
        auto &element_vector = data_hash_table[key]; 
        size_t element_recency = ( element_vector.size() == 1 ) ? elements_number_ + 1 : element_vector[1] - element_vector[0] - 1;
        data_hash_table[key].pop_front();

        if ( cache_hash_table.find( key ) != cache_hash_table.end() ) { 
            cache_hash_table[key] = element_recency;

            return true; 
        }
        else if ( (data_hash_table.find(key)->second).empty() ) { return false; }

        if ( cache_storage.size() < capacity_ ) {
            cache_storage.push_back ( key );
            cache_hash_table.emplace ( key, element_recency );

            return false;
        } 
        auto [max_recency_it, max_recency] = get_element_with_max_recency ();
        if ( ( max_recency <= element_recency ) ) { return false; }

        cache_hash_table.erase( *max_recency_it );
        cache_storage.erase ( max_recency_it );
        cache_storage.push_back ( key );
        cache_hash_table.emplace ( key, element_recency );

        return false;
    }

private:

    auto get_element_with_max_recency ()
    {
        int max_recency = -1;
        auto it = cache_storage.begin();
        auto max_recency_it = it; 
  
        for (auto ite = cache_storage.end(); it != ite; ++it) {
            int current_recency = cache_hash_table[*it];
            if ( current_recency == elements_number_ + 1 ) { 
                max_recency = elements_number_ + 1;
                return std::pair{it, max_recency}; 
            }
            else if ( current_recency > max_recency ) { 
                max_recency = current_recency;
                max_recency_it = it;
            }
        }
        return std::pair{max_recency_it, max_recency};
    }
};