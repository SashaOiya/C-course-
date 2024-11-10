#pragma once

#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <deque>
#include <list>
#include <functional>

namespace cachei {

template <typename T, typename U>
class IdealCache {
    std::deque<T> data_storage;
                            // data, cells
    std::unordered_map< T, std::deque<int> > data_hash_table;
                      // data, irr
    std::unordered_map< T, int> cache_hash_table;
    std::list<U> cache_storage;

    std::function<U(T)> slow_get_page_ {};

    int capacity_ = 0;
    int elements_number_ = 0;

public :
    using itt = typename std::deque<T>::iterator;
    IdealCache ( int capacity, int elements_number, itt from, itt to, std::function<T(T)> slow_get_page_T )
            : capacity_( capacity ), elements_number_ ( elements_number ), slow_get_page_(slow_get_page_T)
    {
        data_hash_table.reserve ( elements_number_ );

        for ( size_t i = 0; from != to; ++from, ++i ) {
            data_storage.push_back(*from);
            data_hash_table[*from].push_back(i);
        }
    }

    bool lookup_update ( )
    {
        if ( data_storage.empty() ) throw "Empty storage";
        auto key = std::move ( data_storage.front() );
        data_storage.pop_front();

        auto &element_vector = data_hash_table[key];
        size_t element_recency = ( element_vector.size() == 1 ) ? elements_number_ + 1 : element_vector[1] - element_vector[0] - 1;
        element_vector.pop_front();

        if ( cache_hash_table.contains(key) ) {
            cache_hash_table[key] = element_recency;

            return true;
        }
        else if ( element_vector.empty() ) { return false; }

        if ( cache_storage.size() < capacity_ ) {
            cache_storage.push_back ( slow_get_page_ ( key ) );
            cache_hash_table.emplace ( key, element_recency );

            return false;
        }
        auto [max_recency_it, max_recency] = get_element_with_max_recency ();
        if ( ( max_recency <= element_recency ) ) { return false; }

        cache_hash_table.erase( *max_recency_it );
        cache_storage.erase ( max_recency_it );
        cache_storage.push_back ( slow_get_page_ ( key ) );
        cache_hash_table.emplace ( key, element_recency );

        return false;
    }

private:

    auto get_element_with_max_recency () const
    {
        int max_recency = -1;
        auto it = cache_storage.begin();
        auto max_recency_it = it;

        for (auto ite = cache_storage.end(); it != ite; ++it) {
            const int current_recency = cache_hash_table.find( *it )->second;
            if ( current_recency == elements_number_ + 1 ) {
                max_recency = elements_number_ + 1;
                return std::pair{it, max_recency};
            }
            if ( current_recency > max_recency ) {
                max_recency = current_recency;
                max_recency_it = it;
            }
        }
        return std::pair{max_recency_it, max_recency};
    }
};

}