#pragma once 

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <fstream>
#include <iterator>

namespace cachel {

template <typename T, typename U>
class LirsCache {
    enum class data_type {
        no_type              = 0,
        lir_type             = 1,
        hir_resident_type    = 2,
        hir_no_resident_type = 3
    };

    struct list_data_s {
        T data; 
        data_type type = data_type::no_type;
        bool place_in_cache_list = false;
        typename std::list<T>::iterator cache_iterator = {};
        typename std::list<T>::iterator hir_iterator = {};
    };

    int capacity = 0;
    size_t elements_number = 0;
    std::list<T> cache_list  = {};   // hir_resident + hir_no_resident + lir
    std::list<T> hir_list    = {};  // only hir_resident
    std::unordered_map< T, list_data_s > cache_hash_table  = {}; 
    size_t lir_size = 0;
    size_t hir_size = 0; 
    size_t currennt_lir_n = 0;  // to control the size and trim the cache_list

    static const int kSingleCacheSize = 1;
    static const int kLastElement     = 1;


public:
    LirsCache ( int capacity, int elements_number ) : capacity( capacity ), elements_number ( elements_number ) 
    {
        adjust_hir_lir_size();
        cache_hash_table.reserve ( elements_number );
    }

    bool lookup_update ( const T key ) 
    {
        auto hash_elem_pointer = search_elem_in_cache ( key ); 

        if ( !hash_elem_pointer ) {
            cache_list.push_back ( key ); 
            hir_list.push_back ( key ); 
            list_data_s temp_list_elem = {key, data_type::hir_resident_type, true, std::prev(cache_list.end()), std::prev(hir_list.end())  };
            cache_hash_table.try_emplace ( key, temp_list_elem );
            clear_hir_list(); 

            return false; 
        }
        
        processing_cache_element ( hash_elem_pointer );
        
        return true;
    }

private:
    static const int large_size = 10;
    static const int middle_size = 3;
    static const int small_size = 2;

    void adjust_hir_lir_size ()
    {
        if ( capacity >= large_size ) { 
            hir_size = capacity / ( large_size / small_size );
            lir_size = capacity - hir_size;
            return ;
        }
        else if ( capacity <= small_size ) {
            lir_size = capacity / small_size;
            hir_size = capacity - lir_size;
            return ;
        } 
        hir_size = capacity / middle_size;
        lir_size = capacity - hir_size;
    }

    void processing_cache_element ( list_data_s *hash_elem_pointer )
    {
        const data_type &elem_type = hash_elem_pointer->type;
        const T &hash_elem = hash_elem_pointer->data;

        if ( elem_type == data_type::hir_resident_type ) {
            if ( hash_elem_pointer->place_in_cache_list == false ) {
                hash_elem_pointer->place_in_cache_list = true;

                cache_list.push_back ( hash_elem );
                hash_elem_pointer->cache_iterator = std::prev( cache_list.end(), kLastElement );

                hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                hir_list.push_back ( hash_elem );
                hash_elem_pointer->hir_iterator = std::prev( hir_list.end(), kLastElement );
            }
            else {
                hash_elem_pointer->type = data_type::lir_type;
                ++(currennt_lir_n);

                hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                lir_size_control ( hash_elem ); 
                hash_elem_pointer->place_in_cache_list = cache_list.size();
            }
        }
        else if ( elem_type == data_type::lir_type ) {  
            cache_list.erase ( hash_elem_pointer->cache_iterator ); 
            cache_list.push_back ( hash_elem );
            hash_elem_pointer->cache_iterator = std::prev( cache_list.end(), kLastElement );
            clear_cache_list ();
        }  
        else if ( elem_type == data_type::hir_no_resident_type ) {
            hash_elem_pointer->type = data_type::lir_type;
            ++(currennt_lir_n);
            hash_elem_pointer->place_in_cache_list = true;

            lir_size_control ( hash_elem );
            clear_hir_list();
        }
    }

    void lir_size_control ( const T element ) 
    {
        if ( cache_list.size() > kSingleCacheSize ) { 
            list_data_s *data_pointer = search_elem_in_cache ( element );
            cache_list.erase ( data_pointer->cache_iterator ); 
            cache_list.push_back ( element );
            data_pointer->cache_iterator = std::prev( cache_list.end(), kLastElement  );

            if ( currennt_lir_n == lir_size + kSingleCacheSize ) {
                search_elem_in_cache ( cache_list.front() )->type = data_type::hir_resident_type; 
                --(currennt_lir_n);
                hir_list.push_back ( cache_list.front() );
                search_elem_in_cache ( cache_list.front() )->hir_iterator = std::prev( hir_list.end(), kLastElement );
            }
        }
        clear_cache_list();
    }

    list_data_s *search_elem_in_cache ( const T desired_elem )
    {
        auto elem_iterator = cache_hash_table.find ( desired_elem ); 
        if ( elem_iterator == cache_hash_table.end () ) {
 
            return nullptr; 
        }

        return &(elem_iterator->second);
    } 

    void clear_cache_list ()
    {
        list_data_s *hash_elem_pointer = search_elem_in_cache ( cache_list.front() );
        for ( ;( hash_elem_pointer->type ) != data_type::lir_type; hash_elem_pointer = search_elem_in_cache ( cache_list.front()) ) {
            hash_elem_pointer->place_in_cache_list = false;

            if ( hash_elem_pointer->type == data_type::hir_no_resident_type ) {
                hash_elem_pointer->type = data_type::no_type;
                cache_hash_table.erase( cache_hash_table.find ( hash_elem_pointer->data ) );
            }
            cache_list.erase ( hash_elem_pointer->cache_iterator );
        }
    }

    void clear_hir_list ()
    {
        if ( hir_list.size () == hir_size + kSingleCacheSize ) {
            list_data_s *no_resident_hir_pointer = search_elem_in_cache ( hir_list.front() );
            hir_list.pop_front ();

            if ( no_resident_hir_pointer->place_in_cache_list ) {
                no_resident_hir_pointer->type = data_type::hir_no_resident_type;
            }
            else {
                no_resident_hir_pointer->type = data_type::no_type;
                cache_hash_table.erase( cache_hash_table.find ( no_resident_hir_pointer->data ) );
            }
        }
    }
};

}