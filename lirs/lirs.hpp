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
const int INVALID_VALUE = -1;

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

template <typename T, typename U>
class Cache_c {
private:
    size_t capacity = 0;
    size_t elements_number = 0;
    std::list<T> cache_list = {};   // hir_resident + hir_no_resident + lir
    std::list<T> hir_list    = {};  // only hir_resident
    std::unordered_map< T, list_data_s<T> > cache_hash_table  = {}; 
    size_t lir_size = 0;
    size_t hir_size = 0; 
    size_t currennt_lir_n = 0;  // to control the size and trim the cache_list
    std::hash<T> hash_func; 

public:
    Cache_c ( size_t capacity, size_t elements_number ) : capacity( capacity ), elements_number ( elements_number ) {};
    int cache_ctor () 
    {
        get_hir_lir_size();
        cache_hash_table.reserve ( elements_number );

        int output = 0;
        for ( size_t i = 0; i < elements_number; ++i ) {
            T element = 0; 
            std::cin >> element;

$           list_data_s <T>*hash_elem_pointer = find_elem_hash_table ( element ); 
            output += cache_processing( hash_elem_pointer, element );
        }

        return elements_number - output;
    }

    void output ( const int output ) 
    {
    std::cout << output << '\n';
    }

    void print_list ( Cache_c<T, U> cache )
    {
        size_t start_size = cache.cache_list.size();
        for ( int i = 0; i < start_size; ++i ) {
            std::cout << "Data cache : " << cache.cache_list.back() << " node_type : " << cache.find_elem_hash_table( cache.cache_list.back() )->type << '\n';
            cache.cache_list.pop_back();
        }
        start_size = cache.hir_list.size();
        for ( int i = 0; i < start_size; ++i ) {
            std::cout << "Data hir : " << cache.hir_list.back() << " node_type : " << cache.find_elem_hash_table ( cache.hir_list.back() )->type << '\n';
            cache.hir_list.pop_back();
        }
    }

private:
    void get_hir_lir_size ()
    {
        assert ( capacity >= 0 );
        if ( capacity >= 10 ) { 
            hir_size = capacity * 2 / 10;
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
 $  }

    int cache_processing ( list_data_s<T> *hash_elem_pointer, const T element ) 
    {
        if ( hash_elem_pointer == nullptr ) {
            cache_list.push_back ( element ); 
            hir_list.push_back ( element ); 
$           list_data_s<T> temp_list_elem = {element, HIR_RESIDENT, true, --(cache_list.end()), --(hir_list.end())  };
            cache_hash_table.try_emplace ( element, temp_list_elem );
            clear_hir_list(); 

            return 1; 
        }
        else {
            const data_type &elem_type = hash_elem_pointer->type;
            const T &hash_elem = hash_elem_pointer->data;
            assert ( elem_type != NO_TYPE );

            if ( elem_type == HIR_RESIDENT ) {
                if ( hash_elem_pointer->place_in_cache_list == false ) {
$                  hash_elem_pointer->place_in_cache_list = true;

                    cache_list.push_back ( hash_elem );
                    hash_elem_pointer->cache_iterator = --( cache_list.end() );

                    hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                    hir_list.push_back ( hash_elem );
                    hash_elem_pointer->hir_iterator = --( hir_list.end() );
                }
                else {
$                   hash_elem_pointer->type = LIR;
                    ++(currennt_lir_n);

                    hir_list.erase ( hash_elem_pointer->hir_iterator ); 
                    lir_size_control ( hash_elem ); 
                    hash_elem_pointer->place_in_cache_list = cache_list.size();
                }
            }
            else if ( elem_type == LIR ) {  
$               cache_list.erase ( hash_elem_pointer->cache_iterator ); 
                cache_list.push_back ( hash_elem );
                hash_elem_pointer->cache_iterator = --( cache_list.end() );
                clear_cache_list ();
            }  
            else if ( elem_type == HIR_NO_RESIDENT ) {
$               hash_elem_pointer->type = LIR;
                ++(currennt_lir_n);
                hash_elem_pointer->place_in_cache_list = true;

                lir_size_control ( hash_elem );
                clear_hir_list();
            }
        }
        return 0;
    }

    void lir_size_control ( const T element ) 
    {
        if ( cache_list.size() > SINGLE_CACHE_SIZE ) { 
            list_data_s<T> *data_pointer = find_elem_hash_table ( element );
            cache_list.erase ( data_pointer->cache_iterator ); 
            cache_list.push_back ( element );
            data_pointer->cache_iterator = --( cache_list.end() );

            assert ( currennt_lir_n <= lir_size + SINGLE_CACHE_SIZE );
            if ( currennt_lir_n == lir_size + SINGLE_CACHE_SIZE ) {
$               find_elem_hash_table( cache_list.front() )->type = HIR_RESIDENT; 
                --(currennt_lir_n);
                hir_list.push_back ( cache_list.front() );
                find_elem_hash_table( cache_list.front() )->hir_iterator = --( hir_list.end() );
            }
        }
        clear_cache_list();
    }

    list_data_s<T> *find_elem_hash_table ( const T desired_elem )
    {
        const T hash = hash_func ( desired_elem ); 
        typename std::unordered_map <T, list_data_s<T> >::iterator elem_iterator = cache_hash_table.find ( hash ); 
        if ( elem_iterator == cache_hash_table.end () ) {
$
            return nullptr; 
        }

        return &(elem_iterator->second);
    } 

    void clear_cache_list ()
    {
        list_data_s<T> *hash_elem_pointer = find_elem_hash_table ( cache_list.front() );
        assert ( hash_elem_pointer != nullptr );
        for ( ;( hash_elem_pointer->type ) != LIR; hash_elem_pointer = find_elem_hash_table( cache_list.front()) ) {
$           hash_elem_pointer->place_in_cache_list = false;

            if ( hash_elem_pointer->type == HIR_NO_RESIDENT ) {
                hash_elem_pointer->type = NO_TYPE;
                cache_hash_table.erase( cache_hash_table.find ( hash_elem_pointer->data ) );
            }
            cache_list.erase ( hash_elem_pointer->cache_iterator );
        }
    }

    void clear_hir_list ()
    {
$       assert ( hir_list.size () <= hir_size + SINGLE_CACHE_SIZE );

        if ( hir_list.size () == hir_size + SINGLE_CACHE_SIZE ) {
            list_data_s<T> *no_resident_hir_pointer = find_elem_hash_table( hir_list.front() );
            hir_list.pop_front ();
            assert ( no_resident_hir_pointer != nullptr );
            
            if ( no_resident_hir_pointer->place_in_cache_list == true ) {
$               no_resident_hir_pointer->type = HIR_NO_RESIDENT;
            }
            else {
$               no_resident_hir_pointer->type = NO_TYPE;
                cache_hash_table.erase( cache_hash_table.find ( no_resident_hir_pointer->data ) );
            }
        }
    }
};