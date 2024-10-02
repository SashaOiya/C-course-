#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <deque>
#include <list>

#define NDEBUG

#ifdef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

enum errors {
    NO_ERRORS = 0,
    CACHE_SIZE_ERR = 1,
    READ_FILE_ERR = 2
};

template <typename T, typename U>
class Cache_c {
    struct list_data_s {
        T data = 0; 
        size_t irr = 0;
        typename std::list<T>::iterator cache_iterator = {};
    };
private :
    std::deque<T> data_vector;
                            // data, cells
    std::unordered_map< T, std::deque<int>> data_hash_table  = {};
    std::hash<T> hash_func;
            // T may be
    std::list< T > cache_list = {}; // name
                      // data, irr
    std::unordered_map< T, list_data_s > cache_hash_table = {};

    size_t capacity = 0; // a_
    size_t elements_number = 0;

public :
    Cache_c ( size_t capacity, size_t elements_number ) : capacity( capacity ), elements_number ( elements_number ) {};

    inline errors cache_ctor () 
    {
        //data_vector.reserve ( elements_number );
        data_hash_table.reserve ( elements_number );
        assert ( cache_list.max_size() > capacity );
$
        for ( size_t i = 0, elem_number = elements_number; i < elem_number; ++i ) {
            T new_element = 0;
            std::cin >> new_element;
            std::deque<int> *element_cells_pointer = find_elem_hash_table ( new_element );

            if ( element_cells_pointer == nullptr ) {
                std::deque<int> cell = {}; 
$               data_hash_table.emplace ( new_element, cell );
                element_cells_pointer = find_elem_hash_table ( new_element );
            }
            assert ( element_cells_pointer != nullptr);
$           element_cells_pointer->push_back ( i );
            data_vector.push_back ( new_element );
        }

        return NO_ERRORS;
    }

    void output ( const int output ) 
    {
        std::cout << output << '\n';
    }

    void print_list ( Cache_c<T, U> cache )
    {
        size_t start_size = cache.cache_list.size();
        for ( int i = 0; i < start_size; ++i ) {
            std::cout << "Data cache : " << cache.cache_list.front() << '\n';
            cache.cache_list.pop_back();
        }
    }

    void print_vector ( std::vector<int> &data )
    {
        std::cout << "DATA :" << '\n';
        for (int n : data )
                std::cout << n << ' ';
            std::cout << '\n';
$
    }

    int cache_processing ()
    {
        size_t misses_total_n = filling_cache (); 
$
        size_t elem_n = data_vector.size();
        for ( size_t i = 0; i < elem_n; ++i ) {
            // get next elem
            T element = data_vector.front();  // list may be
            data_vector.pop_front ();
$
            // get irr
            std::deque<int> *element_cells_pointer = find_elem_hash_table ( element ); 
            assert ( element_cells_pointer != nullptr );
            list_data_s *irr = find_irr_hash_table (element );
            assert ( element_cells_pointer->size() >= 1 );
            if ( ( element_cells_pointer->size() == 1 ) && ( irr == nullptr ) ) {
                element_cells_pointer->pop_front();
                data_hash_table.erase ( data_hash_table.find ( element ) ); 
                
                ++misses_total_n;

                continue;
            }
            size_t element_irr = (*element_cells_pointer)[1] - (*element_cells_pointer)[0] - 1; 
            element_cells_pointer->pop_front(); 

            if ( irr == nullptr ) {
                // search max irr
                T max_irr_element = max_element ( cache_hash_table.begin(), cache_hash_table.end(), compare)->first;  // ---
                list_data_s *max_irr = find_irr_hash_table ( max_irr_element );
                assert ( max_irr != nullptr );
                if ( max_irr->irr < element_irr ) {
                    ++misses_total_n;
                    continue;
                }
                // delete its
                cache_list.erase ( max_irr->cache_iterator ); // ---
                cache_hash_table.erase( cache_hash_table.find ( max_irr_element ) ); 
                assert ( find_irr_hash_table ( element ) == nullptr ); //
$
                // insert in cache_list one
                cache_list.push_back ( element );
                // irr
                list_data_s data = { element, element_irr, --(cache_list.end()) };
                cache_hash_table.emplace ( element, data );
                assert ( find_irr_hash_table ( element ) != nullptr );

                ++misses_total_n;
            }
            else {
                cache_hash_table[element].irr = element_irr;
            }
        }

        return elements_number - misses_total_n;
    }
    
    static bool compare ( const std::pair<T, list_data_s> &a, const std::pair<T, list_data_s> &b )
    {
        return a.second.irr < b.second.irr;
    }

private:
    std::deque<int> *find_elem_hash_table ( T desired_elem )
    {
        const T hash = hash_func ( desired_elem );  ////
        typename std::unordered_map< T, std::deque<int>>::iterator elem_iterator = data_hash_table.find ( hash ); 
        if ( elem_iterator == data_hash_table.end () ) {

            return nullptr; 
        }
$
        return &(elem_iterator->second);
    } 

    list_data_s *find_irr_hash_table ( const T& desired_elem )  // const &
    {
        const T hash = hash_func ( desired_elem ); 

        typename std::unordered_map< T, list_data_s>::iterator elem_iterator = cache_hash_table.find ( hash ); 
        if ( elem_iterator == cache_hash_table.end () ) {

            return nullptr; 
        }
$
        return &(elem_iterator->second);
    } 

    inline size_t filling_cache () 
    {
        size_t misses_number = 0;

        for ( size_t i = 0; ( i < capacity ) && ( data_vector.size()); ++i ) { 
            // get element 
            T& element = data_vector.front();   ///////
            data_vector.pop_front();

            // get irr
            std::deque<int> *element_cells_pointer = find_elem_hash_table ( element ); 
            list_data_s *element_irr = find_irr_hash_table ( element );
            assert ( element_cells_pointer != nullptr ); // in find
            assert ( !(element_cells_pointer->empty()) );
            if ( ( element_cells_pointer->size() == 1 ) && ( element_irr == nullptr ) ) {
                element_cells_pointer->pop_front();
                data_hash_table.erase ( data_hash_table.find ( element ) );
                ++misses_number; 
                --i;
$
                continue;
            }
            size_t new_element_irr = (*element_cells_pointer)[1] - (*element_cells_pointer)[0] - 1; 
            element_cells_pointer->pop_front(); 
$
            // filling cache 
            if ( element_irr == nullptr ) { 
                cache_list.push_back ( element );
                list_data_s data = { element, new_element_irr, --(cache_list.end()) };
                cache_hash_table.emplace ( element, data );
                assert ( find_irr_hash_table ( element ) != nullptr );

                ++misses_number;
            }
            else { 
                cache_hash_table[element].irr = new_element_irr;
                --i;
            }
        }
        return misses_number;
    }

};