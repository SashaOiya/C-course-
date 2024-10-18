#pragma once 

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <fstream>
#include <iterator>

const int kSingleCacheSize = 1;
const int kLastElement     = 1;

enum data_type {
    no_type              = 0,
    lir_type             = 1,
    hir_resident_type    = 2,
    hir_no_resident_type = 3
};

template <typename T>
struct list_data_s {
    T data; 
    data_type type = no_type;
    bool place_in_cache_list = false;
    typename std::list<T>::iterator cache_iterator = {};
    typename std::list<T>::iterator hir_iterator = {};
};

template <typename T, typename U>
class LirsCache {
    size_t capacity = 0;
    size_t elements_number = 0;
    std::list<T> cache_list  = {};   // hir_resident + hir_no_resident + lir
    std::list<T> hir_list    = {};  // only hir_resident
    std::unordered_map< T, list_data_s<T> > cache_hash_table  = {}; 
    size_t lir_size = 0;
    size_t hir_size = 0; 
    size_t currennt_lir_n = 0;  // to control the size and trim the cache_list

public:
    LirsCache ( size_t capacity, size_t elements_number ) : capacity( capacity ), elements_number ( elements_number ) 
    {
        get_hir_lir_size();
        cache_hash_table.reserve ( elements_number );
    }

    bool lookup_update ( const T key ) 
    {
        auto hash_elem_pointer = search_elem_in_cache ( key ); 

        if ( !hash_elem_pointer ) {
            cache_list.push_back ( key ); 
            hir_list.push_back ( key ); 
            list_data_s<T> temp_list_elem = {key, hir_resident_type, true, std::prev(cache_list.end()), std::prev(hir_list.end())  };
            cache_hash_table.try_emplace ( key, temp_list_elem );
            clear_hir_list(); 

            return false; 
        }
        else {
            const data_type &elem_type = hash_elem_pointer->type;
            const T &hash_elem = hash_elem_pointer->data;

            if ( elem_type == hir_resident_type ) {
                if ( hash_elem_pointer->place_in_cache_list == false ) {
                    hash_elem_pointer->place_in_cache_list = true;

                    cache_list.push_back ( hash_elem );
                    hash_elem_pointer->cache_iterator = std::prev( cache_list.end(), kLastElement );

                    hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                    hir_list.push_back ( hash_elem );
                    hash_elem_pointer->hir_iterator = std::prev( hir_list.end(), kLastElement );
                }
                else {
                    hash_elem_pointer->type = lir_type;
                    ++(currennt_lir_n);

                    hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                    lir_size_control ( hash_elem ); 
                    hash_elem_pointer->place_in_cache_list = cache_list.size();
                }
            }
            else if ( elem_type == lir_type ) {  
                cache_list.erase ( hash_elem_pointer->cache_iterator ); 
                cache_list.push_back ( hash_elem );
                hash_elem_pointer->cache_iterator = std::prev( cache_list.end(), kLastElement );
                clear_cache_list ();
            }  
            else if ( elem_type == hir_no_resident_type ) {
                hash_elem_pointer->type = lir_type;
                ++(currennt_lir_n);
                hash_elem_pointer->place_in_cache_list = true;

                lir_size_control ( hash_elem );
                clear_hir_list();
            }
        }
        return true;
    }

private:
    void get_hir_lir_size ()
    {
        if ( capacity >= 10 ) { 
            hir_size = capacity / 5;
            lir_size = capacity - hir_size;
        }
        else if ( capacity <= 2 ) {
            lir_size = capacity / 2;
            hir_size = capacity - lir_size;
        } 
        else {
            hir_size = capacity / 3;
            lir_size = capacity - hir_size;
        }
    }

    void lir_size_control ( const T element ) 
    {
        if ( cache_list.size() > kSingleCacheSize ) { 
            list_data_s<T> *data_pointer = search_elem_in_cache ( element );
            cache_list.erase ( data_pointer->cache_iterator ); 
            cache_list.push_back ( element );
            data_pointer->cache_iterator = std::prev( cache_list.end(), kLastElement  );

            if ( currennt_lir_n == lir_size + kSingleCacheSize ) {
                search_elem_in_cache ( cache_list.front() )->type = hir_resident_type; 
                --(currennt_lir_n);
                hir_list.push_back ( cache_list.front() );
                search_elem_in_cache ( cache_list.front() )->hir_iterator = std::prev( hir_list.end(), kLastElement );
            }
        }
        clear_cache_list();
    }

    list_data_s<T> *search_elem_in_cache ( const T desired_elem )
    {
        auto elem_iterator = cache_hash_table.find ( desired_elem ); 
        if ( elem_iterator == cache_hash_table.end () ) {
 
            return nullptr; 
        }

        return &(elem_iterator->second);
    } 

    void clear_cache_list ()
    {
        list_data_s<T> *hash_elem_pointer = search_elem_in_cache ( cache_list.front() );
        for ( ;( hash_elem_pointer->type ) != lir_type; hash_elem_pointer = search_elem_in_cache ( cache_list.front()) ) {
            hash_elem_pointer->place_in_cache_list = false;

            if ( hash_elem_pointer->type == hir_no_resident_type ) {
                hash_elem_pointer->type = no_type;
                cache_hash_table.erase( cache_hash_table.find ( hash_elem_pointer->data ) );
            }
            cache_list.erase ( hash_elem_pointer->cache_iterator );
        }
    }

    void clear_hir_list ()
    {
        if ( hir_list.size () == hir_size + kSingleCacheSize ) {
            list_data_s<T> *no_resident_hir_pointer = search_elem_in_cache ( hir_list.front() );
            hir_list.pop_front ();
            
            if ( no_resident_hir_pointer->place_in_cache_list ) {
                no_resident_hir_pointer->type = hir_no_resident_type;
            }
            else {
                no_resident_hir_pointer->type = no_type;
                cache_hash_table.erase( cache_hash_table.find ( no_resident_hir_pointer->data ) );
            }
        }
    }
};