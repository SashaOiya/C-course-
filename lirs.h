#pragma once 

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <ctime>
#include <fstream>

//#define DEBUG

#ifdef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

const int SINGLE_CACHE_SIZE = 1;

enum data_type {
    NO_TYPE         = 0,
    LIR             = 1,
    HIR_RESIDENT    = 2,
    HIR_NO_RESIDENT = 3
};

enum errors {
    NO_ERRORS = 0,
    HIR_LIST_SIZE_ERR = 1,
    READ_FILE_ERR = 2
};

template <typename T>
struct list_data_s {
    T data = 0; 
    data_type type = NO_TYPE;
    bool place_in_cache_list = false;
    typename std::list<T>::iterator cache_iterator = {};
    typename std::list<T>::iterator hir_iterator = {};
};

template <typename T>
class Cache_c {
private:
    std::list<T> cache_list = {};   // hir_resident + hir_no_resident + lir
    std::list<T> hir_list    = {};  // only hir_resident
    std::unordered_map< T, list_data_s<T> > cache_hash_table  = {}; 
    size_t capacity = 0;
    size_t lir_size = 0;
    size_t hir_size = 0; 
    size_t elements_number = 0;
    size_t currennt_lir_n = 0;  // to control the size and trim the cache_list
    std::hash<T> hash_func; 

public:
    int cache_ctor ( Cache_c<T> &cache, const char *file_name ) 
    {
        std::ifstream file ( file_name );
        if ( !file ) {
            std::cout<< "FILE NOT FOUNDED\n";

            return -1; 
        }
        file >> cache.capacity >> cache.elements_number;
        cache.get_hir_lir_size( cache );
        cache.cache_hash_table.reserve ( cache.elements_number );

        int output = 0;
        for (  int i = 0; i < cache.elements_number; ++i ) {
            int element = 0; // not only int
            file >> element;

$           list_data_s <T>*hash_elem_pointer = cache.find_elem_hash_table ( cache, element ); 
            output += cache.cache_processing( cache, hash_elem_pointer, element );
        }
        file.close();

        return cache.elements_number - output;
    }

    void print_list_typo ( Cache_c<T> cache )
    {
        size_t start_size = cache.cache_list.size();
        for ( int i = 0; i < start_size; ++i ) {
            std::cout << "Data cache : " << cache.cache_list.back() << " node_type : " << cache.find_elem_hash_table( cache, cache.cache_list.back() )->type << '\n';
            cache.cache_list.pop_back();
        }
        start_size = cache.hir_list.size();
        for ( int i = 0; i < start_size; ++i ) {
            std::cout << "Data hir : " << cache.hir_list.back() << " node_type : " << cache.find_elem_hash_table ( cache, cache.hir_list.back() )->type << '\n';
            cache.hir_list.pop_back();
        }
    }

private:
    void get_hir_lir_size ( Cache_c<T> &cache )
    {
        // define ?
        if ( cache.capacity >= 10 ) { // const
            cache.hir_size = cache.capacity * 2 / 10;
            cache.lir_size = cache.capacity - cache.hir_size;
        }
        else if ( cache.capacity <= 2 ) {
            cache.lir_size = cache.capacity / 2;
            cache.hir_size = cache.capacity - cache.lir_size;
        } 
        else {
            cache.hir_size = cache.capacity / 3;
            cache.lir_size = cache.capacity - cache.hir_size;
        }
 $  }

    int cache_processing ( Cache_c<T> &cache, list_data_s<T> *hash_elem_pointer, const T element ) 
    {
        if ( hash_elem_pointer == nullptr ) {
            cache.cache_list.push_back ( element ); 
            cache.hir_list.push_back ( element ); 
$           list_data_s<T> temp_list_elem = {element, HIR_RESIDENT, true, --(cache.cache_list.end()), --(cache.hir_list.end())  };
            cache.cache_hash_table.try_emplace ( element, temp_list_elem );
            cache.clear_hir_list( cache ); 

            return 1; 
        }
        else {
            const data_type &elem_type = hash_elem_pointer->type;
            const T &hash_elem = hash_elem_pointer->data;
            assert ( elem_type != NO_TYPE );

            if ( elem_type == HIR_RESIDENT ) {
                if ( hash_elem_pointer->place_in_cache_list == false ) {
$                  hash_elem_pointer->place_in_cache_list = true;

                    cache.cache_list.push_back ( hash_elem );
                    hash_elem_pointer->cache_iterator = --( cache.cache_list.end() );

                    cache.hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                    cache.hir_list.push_back ( hash_elem );
                    hash_elem_pointer->hir_iterator = --( cache.hir_list.end() );
                }
                else {
$                   hash_elem_pointer->type = LIR;
                    ++(cache.currennt_lir_n);

                    cache.hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                    cache.checking_lir_size ( cache, hash_elem ); 
                    hash_elem_pointer->place_in_cache_list = cache.cache_list.size();
                }
            }
            else if ( elem_type == LIR ) {  
$               cache.cache_list.erase ( hash_elem_pointer->cache_iterator ); 
                cache.cache_list.push_back ( hash_elem );
                hash_elem_pointer->cache_iterator = --( cache.cache_list.end() );
                cache.clear_cache_list ( cache );
            }  
            else if ( elem_type == HIR_NO_RESIDENT ) {
$               hash_elem_pointer->type = LIR;
                ++(cache.currennt_lir_n);
                hash_elem_pointer->place_in_cache_list = true;

                cache.checking_lir_size ( cache, hash_elem );
                cache.clear_hir_list( cache );
            }
        }
        return 0;
    }

    void checking_lir_size ( Cache_c<T> &cache, const T element ) // bad name
    {
        if ( cache.cache_list.size() > SINGLE_CACHE_SIZE ) { 
            list_data_s<T> *data_pointer = cache.find_elem_hash_table ( cache, element );
            cache.cache_list.erase ( data_pointer->cache_iterator ); 
            cache.cache_list.push_back ( element );
            data_pointer->cache_iterator = --( cache.cache_list.end() );

            assert ( cache.currennt_lir_n <= cache.lir_size + SINGLE_CACHE_SIZE );
            if ( cache.currennt_lir_n == cache.lir_size + SINGLE_CACHE_SIZE ) {
$               (cache.find_elem_hash_table ( cache, cache.cache_list.front() ))->type = HIR_RESIDENT; 
                --(cache.currennt_lir_n);
                cache.hir_list.push_back ( cache.cache_list.front() );
                (cache.find_elem_hash_table ( cache, cache.cache_list.front() ))->hir_iterator = --( cache.hir_list.end() );
            }
        }
        cache.clear_cache_list( cache );
    }

    list_data_s<T> *find_elem_hash_table ( Cache_c<T> &cache, const T desired_elem )
    {
        const T hash = cache.hash_func ( desired_elem ); 
        typename std::unordered_map <T, list_data_s<T> >::iterator elem_iterator = cache.cache_hash_table.find ( hash ); 
        if ( elem_iterator == cache.cache_hash_table.end () ) {
$
            return nullptr; 
        }

        return &(elem_iterator->second);
    } 

    void clear_cache_list ( Cache_c<T> &cache )
    {
        list_data_s<T> *hash_elem_pointer = cache.find_elem_hash_table ( cache, cache.cache_list.front() );
        assert ( hash_elem_pointer != nullptr );
        for ( ;( hash_elem_pointer->type ) != LIR; hash_elem_pointer = cache.find_elem_hash_table(cache, cache.cache_list.front()) ) {
$           hash_elem_pointer->place_in_cache_list = false;

            if ( hash_elem_pointer->type == HIR_NO_RESIDENT ) {
                hash_elem_pointer->type = NO_TYPE;
                cache.cache_hash_table.erase( cache.cache_hash_table.find ( hash_elem_pointer->data ) );
            }
            cache.cache_list.erase ( hash_elem_pointer->cache_iterator );
        }
    }

    void clear_hir_list ( Cache_c<T> &cache )
    {
$       assert ( cache.hir_list.size () <= cache.hir_size + SINGLE_CACHE_SIZE );

        if ( cache.hir_list.size () == cache.hir_size + SINGLE_CACHE_SIZE ) {
            list_data_s<T> *no_resident_hir_pointer = cache.find_elem_hash_table( cache, cache.hir_list.front() );
            cache.hir_list.pop_front ();
            assert ( no_resident_hir_pointer != nullptr );
            
            if ( no_resident_hir_pointer->place_in_cache_list == true ) {
$               no_resident_hir_pointer->type = HIR_NO_RESIDENT;
            }
            else {
$               no_resident_hir_pointer->type = NO_TYPE;
                cache.cache_hash_table.erase( cache.cache_hash_table.find ( no_resident_hir_pointer->data ) );
            }
        }
    }
};