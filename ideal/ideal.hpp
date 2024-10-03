#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <deque>
#include <vector>
#include <list>

#define NDEBUG

#ifdef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

const int INF = -1; 

enum errors {
    NO_ERRORS = 0,
    CACHE_SIZE_ERR = 1,
    READ_FILE_ERR = 2
};

template <typename T, typename U>
class Cache_c {
    std::deque<T> data_vector = {};
                            // data, cells
    std::unordered_map< T, std::deque<int> > data_hash_table  = {};
    std::vector<T> cache_list = {}; // name
                      // data, irr
    std::unordered_map< T, int> cache_hash_table = {}; 

    size_t capacity = 0; // a_
    size_t elements_number = 0;

public :
    Cache_c ( size_t capacity, size_t elements_number ) : capacity( capacity ), elements_number ( elements_number ) {};

    errors reading_data () 
    {
        data_hash_table.reserve ( elements_number );
        assert ( cache_list.max_size() > capacity );
$
        for ( size_t i = 0, elem_number = elements_number; i < elem_number; ++i ) {
            T new_element = 0;
            std::cin >> new_element;
            std::deque<int> *element_pointer = search_in_data_hash_table ( new_element );

            if ( element_pointer == nullptr ) {
                std::deque<int> cell = {}; 
$               data_hash_table.emplace ( new_element, cell );
                element_pointer = search_in_data_hash_table ( new_element );
            }
            assert ( element_pointer != nullptr);

$           element_pointer->push_back ( i );
            data_vector.push_back ( new_element );
        }

        return NO_ERRORS;
    }

    void output ( const int output ) 
    {
        std::cout << output << '\n';
    }

    int cache_processing ()
    {
        size_t misses_total_n = 0;
$
        for ( size_t i = 0, elem_n = data_vector.size(); ( i < elem_n ) && ( data_vector.size() ); ++i ) {
            // get next elem
            T element = data_vector.front();  // list may be
            data_vector.pop_front();
$
            // get irr
            std::deque<int> *element_vector_pointer = search_in_data_hash_table ( element ); 
            assert ( element_vector_pointer != nullptr );
            int irr = search_in_cache_hash_table (element );
            assert ( element_vector_pointer->size() >= 1 );
            size_t element_recency = 0;
            if ( ( element_vector_pointer->size() == 1 ) && ( irr == -1 ) ) {
                element_vector_pointer->pop_front();
                data_hash_table.erase ( data_hash_table.find ( element ) ); 
                
                ++misses_total_n;

                continue;
            }
            else if ( element_vector_pointer->size() == 1 ) { element_recency = INF; }  
            else {  
                element_recency = (*element_vector_pointer)[1] - (*element_vector_pointer)[0] - 1; //
                element_vector_pointer->pop_front(); //
            }

            if ( ( irr == -1 ) && ( cache_list.size() < capacity ) ) { 
$               cache_list.push_back ( element );
                cache_hash_table.emplace ( element, element_recency );
                assert ( search_in_cache_hash_table ( element ) != -1 );

                ++misses_total_n;
            }
            else if ( irr == -1 ) {
                // search max irr
                ++misses_total_n;

                T max_irr_element = get_element_with_max_recency ();
$               size_t max_recency = search_in_cache_hash_table ( max_irr_element );
                if ( max_recency < element_recency ) {
                    continue;
                }
                // delete its
$               cache_list.erase ( std::remove( cache_list.begin(), cache_list.end(), max_irr_element ), cache_list.end() );
                cache_hash_table.erase( cache_hash_table.find ( max_irr_element ) ); 
                assert ( search_in_cache_hash_table ( element ) == -1 ); //
$
                // insert in cache_list one
                cache_list.push_back ( element );
                cache_hash_table.emplace ( element, element_recency );
                assert ( search_in_cache_hash_table ( element ) != -1 );
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
$
        return &(elem_iterator->second);
    } 

    int search_in_cache_hash_table ( T desired_elem )
    {
        typename std::unordered_map< T, int>::iterator elem_iterator = cache_hash_table.find ( desired_elem ); 
        if ( elem_iterator == cache_hash_table.end () ) { return INF; }
$
        return elem_iterator->second;
    } 

    T get_element_with_max_recency () 
    {
        int max_recency = 0;
        T max_recency_element = 0;
        typename std::vector<T>::iterator element_iterator = cache_list.begin();

        for ( size_t i = 0, size = cache_hash_table.size(); i < size; ++i ) {
            T current_element = *element_iterator;
            int current_recency = search_in_cache_hash_table ( current_element );
            if ( current_recency == INF ) {
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