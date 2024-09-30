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

template <typename T>
class Cache_c {
private :
    std::vector<T> data_vector = {};
                            // data, cells
    std::unordered_map< T, std::vector<int>> data_hash_table  = {};
    std::hash<T> hash_func;

    std::vector<T> cache_list = {}; // name
                      // data, irr
    std::unordered_map< T, int> cache_hash_table = {}; 

    size_t capacity = 0; // a_
    size_t emements_number = 0;

public :
    inline errors cache_ctor ( Cache_c<T> &cache, std::string file_name ) 
    {
        std::ifstream file ( file_name );
        if ( !file ) {
            std::cout<< "FILE NOT FOUNDED\n";

            return READ_FILE_ERR; 
        }

        file >> cache.capacity >> cache.emements_number;
        cache.data_vector.reserve ( cache.emements_number );
        cache.data_hash_table.reserve ( cache.emements_number );
        assert ( cache.cache_list.max_size() > cache.capacity );
$
        for ( int i = 0, elem_number = cache.emements_number; i < elem_number; ++i ) {
            T new_element = 0;
            file >> new_element;
            std::vector<int> *element_pointer = cache.find_elem_hash_table ( cache, new_element );

            if ( element_pointer == nullptr ) {
                std::vector<int> cell = {}; 
$               cache.data_hash_table.emplace ( new_element, cell );
                element_pointer = cache.find_elem_hash_table ( cache, new_element );
            }
            assert ( element_pointer != nullptr);

$           element_pointer->push_back ( i );
            cache.data_vector.push_back ( new_element );
        }
        file.close();

        return NO_ERRORS;
    }

    void output ( Cache_c<T> cache, int output, std::string input_file_name ) 
    {
        std::ofstream out ( "../../test/comparison.txt", std::ios::app ); 
        if ( !out ) {

            return ; 
        }
        out << "IDEAL. Test " << input_file_name << " OUT: " << output << std::endl;
        out.close();
    }

    void print_list ( Cache_c<T> cache )
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

    int cache_processing ( Cache_c<T> &cache )
    {
        size_t misses_total_n = cache.filling_cache ( cache ); 
$
        size_t elem_n = cache.data_vector.size();
        for ( size_t i = 0; i < elem_n; ++i ) {
            // get next elem
            T element = cache.data_vector.front();  // list may be
            cache.data_vector.erase ( cache.data_vector.begin() );
$
            // get irr
            std::vector<int> *element_vector_pointer = cache.find_elem_hash_table ( cache, element ); 
            assert ( element_vector_pointer != nullptr );
            int irr = cache.find_irr_hash_table ( cache, element );
            assert ( element_vector_pointer->size() >= 1 );
            if ( ( element_vector_pointer->size() == 1 ) && ( irr == -1 ) ) {
                element_vector_pointer->erase( element_vector_pointer->begin() );
                cache.data_hash_table.erase ( cache.data_hash_table.find ( element ) ); 
                
                ++misses_total_n;

                continue;
            }
            size_t element_irr = (*element_vector_pointer)[1] - (*element_vector_pointer)[0] - 1; //
            element_vector_pointer->erase( element_vector_pointer->begin() ); //

            if ( irr == -1 ) {
                // search max irr
                T max_irr_element = max_element(cache.cache_hash_table.begin(), cache.cache_hash_table.end(), compare)->first; 
                size_t max_irr = cache.find_irr_hash_table ( cache, max_irr_element );
                if ( max_irr < element_irr ) {
                    ++misses_total_n;
                    continue;
                }
                // delete its
                cache.cache_list.erase ( std::remove(cache.cache_list.begin(), cache.cache_list.end(), max_irr_element ), cache.cache_list.end() );
                cache.cache_hash_table.erase( cache.cache_hash_table.find ( max_irr_element ) ); 
                assert ( cache.find_irr_hash_table( cache, element ) == -1 ); //
$
                // insert in cache_list one
                cache.cache_list.push_back ( element );
                cache.cache_hash_table.emplace ( element, element_irr );
                assert ( cache.find_irr_hash_table ( cache, element ) != -1 );

                ++misses_total_n;
            }
            else {
                cache.cache_hash_table[element] = element_irr;
            }
        }

        return cache.emements_number - misses_total_n;
    }
    
    static bool compare ( const std::pair<T, int> &a, const std::pair<T, int> &b )
    {
        return a.second < b.second;
    }

private:
    std::vector<int> *find_elem_hash_table ( Cache_c<T> &cache, T desired_elem )
    {
        const T hash = cache.hash_func ( desired_elem ); 
        typename std::unordered_map< T, std::vector<int>>::iterator elem_iterator = cache.data_hash_table.find ( hash ); 
        if ( elem_iterator == cache.data_hash_table.end () ) {

            return nullptr; 
        }
$
        return &(elem_iterator->second);
    } 

    inline int find_irr_hash_table ( Cache_c<T> &cache, T desired_elem )
    {
        const T hash = cache.hash_func ( desired_elem ); 

        typename std::unordered_map< T, int>::iterator elem_iterator = cache.cache_hash_table.find ( hash ); 
        if ( elem_iterator == cache.cache_hash_table.end () ) {

            return -1; 
        }
$
        return elem_iterator->second;
    } 

    inline size_t filling_cache ( Cache_c<T> &cache ) 
    {
        size_t misses_number = 0;

        for ( size_t i = 0; ( i < cache.capacity ) && ( cache.data_vector.size()); ++i ) { 
            // get element 
            T element = cache.data_vector.front();  
            cache.data_vector.erase ( cache.data_vector.begin() );

            // get irr
            std::vector<int> *element_pointer = cache.find_elem_hash_table ( cache, element ); 
            int element_irr = cache.find_irr_hash_table( cache, element );
            assert ( element_pointer != nullptr );
            assert ( element_pointer->size() >= 1 );
            if ( ( element_pointer->size() == 1 ) && ( element_irr == -1 ) ) {
                element_pointer->erase( element_pointer->begin() );
                cache.data_hash_table.erase ( cache.data_hash_table.find ( element ) );
                ++misses_number; 
                --i;
$
                continue;
            }
            size_t new_element_irr = (*element_pointer)[1] - (*element_pointer)[0] - 1; 
            element_pointer->erase( element_pointer->begin() ); 
$
            // filling cache 
            if ( element_irr == -1 ) { 
                cache.cache_list.push_back ( element );
                cache.cache_hash_table.emplace ( element, new_element_irr );
                assert ( cache.find_irr_hash_table ( cache, element ) != -1 );

                ++misses_number;
            }
            else { 
                cache.cache_hash_table[element] = new_element_irr;
                --i;
            }
        }
        return misses_number;
    }

};