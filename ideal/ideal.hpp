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

template <typename T, typename U>
class Cache_c {
    std::deque<T> data_vector = {};
                            // data, cells
    std::unordered_map< T, std::deque<int> > data_hash_table  = {};
    std::vector<T> cache_storage = {}; 
                      // data, irr
    std::unordered_map< T, int> cache_hash_table = {}; 

    size_t capacity = 0; // a_
    size_t elements_number = 0;
    std::hash<T> hash_func;

public :
    Cache_c ( size_t capacity, size_t elements_number ) : capacity( capacity ), elements_number ( elements_number ) {};

    void reading_data () 
    {
        data_hash_table.reserve ( elements_number );
        assert ( cache_storage.max_size() > capacity );

        for ( size_t i = 0; i < elements_number; ++i ) {
            T new_element = 0;
            std::cin >> new_element;
            std::deque<int> *element_pointer = search_in_data_hash_table ( new_element );

            if ( element_pointer == nullptr ) {
                std::deque<int> cell = {}; 
                data_hash_table.emplace ( new_element, cell );
                element_pointer = search_in_data_hash_table ( new_element );
            }
            assert ( element_pointer != nullptr);

            element_pointer->push_back ( i );
            data_vector.push_back ( new_element );
        }
    }

    void output ( const int output ) 
    {
        std::cout << output << '\n';
    }

    const int MAX_IRR = elements_number + 1;

    int cache_processing ()
    {
        size_t misses_total_n = 0;

        for ( size_t i = 0, elem_n = data_vector.size(); ( i < elem_n ) && ( data_vector.size() ); ++i ) {
            // get next elem
            T element = data_vector.front(); 
            data_vector.pop_front();

            // get irr
            std::deque<int> *element_vector_pointer = search_in_data_hash_table ( element ); 
            auto irr = search_in_cache_hash_table (element );
            size_t element_recency = 0; //
            if ( irr == cache_hash_table.end() ) { ++misses_total_n; }

            if (  ( element_vector_pointer->size() == 1 ) && ( irr == cache_hash_table.end() ) ) {
                element_vector_pointer->pop_front();
                data_hash_table.erase ( data_hash_table.find ( element ) ); 

                continue;
            }
            element_recency = ( element_vector_pointer->size() == 1 ) ? MAX_IRR:(*element_vector_pointer)[1] - (*element_vector_pointer)[0] - 1; 
            element_vector_pointer->pop_front();

            if ( ( irr == cache_hash_table.end() ) && ( cache_storage.size() < capacity ) ) { 
                cache_storage.push_back ( element );
                cache_hash_table.emplace ( element, element_recency );
                assert ( search_in_cache_hash_table ( element ) != cache_hash_table.end() );
            }
            else if ( irr == cache_hash_table.end() ) {
                T max_irr_element = get_element_with_max_recency ();
                size_t max_recency = search_in_cache_hash_table( max_irr_element )->second;
                if ( ( max_recency <= element_recency ) ) {
                    continue;
                }
                // delete its
                cache_storage.erase ( std::remove( cache_storage.begin(), cache_storage.end(), max_irr_element ), cache_storage.end() );
                cache_hash_table.erase( cache_hash_table.find ( max_irr_element ) ); 

                // insert in cache_list one
                cache_storage.push_back ( element );
                cache_hash_table.emplace ( element, element_recency );
            }
            else {
                cache_hash_table[element] = element_recency;
            }
        }

        return elements_number - misses_total_n;
    }

private:

    std::deque<int> *search_in_data_hash_table ( T desired_elem )
    {
        typename std::unordered_map< T, std::deque<int>>::iterator elem_iterator = data_hash_table.find ( desired_elem ); 
        if ( elem_iterator == data_hash_table.end () ) { return nullptr; }

        return &(elem_iterator->second);
    } 

    auto search_in_cache_hash_table ( T desired_elem )
    {
        typename std::unordered_map< T, int>::iterator elem_iterator = cache_hash_table.find ( desired_elem ); 
 
        return elem_iterator;
    } 

    T get_element_with_max_recency () 
    {
        int max_recency = 0;
        T max_recency_element = 0;
        typename std::vector<T>::iterator element_iterator = cache_storage.begin();

        for ( size_t i = 0, size = cache_storage.size(); i < size; ++i ) {
            T current_element = *element_iterator;
            int current_recency = search_in_cache_hash_table( current_element )->second;
            if ( current_recency == MAX_IRR ) {
                return current_element;
            }
            else if ( current_recency > max_recency ) {
                max_recency_element = current_element;
            }
            ++element_iterator;
        }
        return max_recency_element;
    }
};