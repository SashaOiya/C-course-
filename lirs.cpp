#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <cmath>
#include <stdio.h>

#ifndef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

enum data_type {
    NO_TYPE         = 0,
    LIR             = 1,
    HIR_RESIDENT    = 2,
    HIR_NO_RESIDENT = 3
};

enum errors {
    NO_ERRORS = 0,
    HIR_LIST_SIZE_ERR = 1
};

struct list_data_s {
    int data = 0; // not only int
    data_type type = NO_TYPE;
    bool place_in_cache_list = false;
#define INF -1
    int location = INF; // ? 
    size_t IRR = INF;
    //int R = INF;
};

struct cache_s { 
        std::list<list_data_s *> cache_list = {};
        std::list<list_data_s *> hir_list    = {};
        std::unordered_map< int, list_data_s *> cache_hash_table  = {}; // not only int
        size_t lir_size = 0;
        size_t hir_size = 0;
        size_t capacity = 0;
        std::hash<int> hash_int; // int ?
}; 

errors Get_Elem ( cache_s *cache );
void Cache_Ctor ( cache_s *cache );

void Print_List_Typo ( cache_s *cache ); //

int main ()
{
    cache_s cache = {};

    Cache_Ctor ( &cache );

    Get_Elem ( &cache );
    // 
    Print_List_Typo ( &cache );

    printf ( "Successfully\n" );

    return 0;
}

void Cache_Ctor ( cache_s *cache ) // return errors
{
    assert ( cache != nullptr );

    std::cin >> cache->lir_size >> cache->hir_size >> cache->capacity;
    //cache->capacity = cache->lir_size + cache->hir_size;

    // ctor lir_hir_list, hir_list, lir_hir_hash_table
}

list_data_s *Find_Elem_Hash_Table ( cache_s *cache, int desired_elem );
void Clear_Cache_List ( cache_s *cache );
void Clear_Hir_List ( cache_s *cache );

errors Get_Elem ( cache_s *cache ) 
{
    assert ( cache != nullptr ); 

    for (  int i = 0; i < cache->capacity; ++i ) {
        int element = 0; // not only int
        std::cin >> element;

$       list_data_s *hash_elem_pointer = Find_Elem_Hash_Table ( cache, element ); // rename

        if ( hash_elem_pointer == nullptr ) {
$           list_data_s *temp_list_elem = (list_data_s *)calloc ( sizeof (list_data_s) * 1, 1 ); 
            // errors
            // not forget clean

            temp_list_elem->data = element;
            temp_list_elem->type = HIR_RESIDENT;
            //temp_list_elem->IRR = INF;
            temp_list_elem->place_in_cache_list = true; // whithout clear cache list

            cache->cache_list.push_front ( temp_list_elem );

            cache->hir_list.push_front ( temp_list_elem );
            cache->cache_hash_table.try_emplace ( temp_list_elem->data, temp_list_elem );
            Clear_Hir_List ( cache );
            //Clear_Cache_List ( cache );

            temp_list_elem->location = ( cache->cache_list.size() == 0 ) ? INF : cache->cache_list.size() ;
        }
        else {
            const data_type elem_type = hash_elem_pointer->type;

            assert ( elem_type != NO_TYPE );

            if ( elem_type == LIR ) {    // weird
$               hash_elem_pointer->IRR =(hash_elem_pointer->location == INF) ? INF : fabs(cache->cache_list.size()-hash_elem_pointer->location );
                // replace to front 
                cache->cache_list.remove ( hash_elem_pointer ); // class in cache_s
                cache->cache_list.push_front ( hash_elem_pointer );
                // clear cache_list to LIR
                Clear_Cache_List ( cache );
                hash_elem_pointer->location = cache->cache_list.size();
            }  
            else if ( elem_type == HIR_RESIDENT ) {
                if ( hash_elem_pointer->place_in_cache_list == false ) {
$                   hash_elem_pointer->IRR = INF;
                    hash_elem_pointer->place_in_cache_list = true;

                    cache->cache_list.push_front ( hash_elem_pointer );
                    hash_elem_pointer->location = cache->cache_list.size();
                    cache->hir_list.remove ( hash_elem_pointer ); // class in cache_s
                    cache->hir_list.push_front ( hash_elem_pointer );
                }
                else {
                    hash_elem_pointer->IRR=(hash_elem_pointer->location == INF) ? INF : fabs(cache->cache_list.size()-hash_elem_pointer->location ); 
                    if ( ( hash_elem_pointer->IRR < cache->cache_list.size() ) && ( hash_elem_pointer->location != INF )  ) {
$                       hash_elem_pointer->type = LIR;
                        cache->hir_list.remove ( hash_elem_pointer );

                        if ( cache->cache_list.size() > 1 ) { /// aaaaaaaaaaa
                            cache->cache_list.remove ( hash_elem_pointer );
                            cache->cache_list.push_front ( hash_elem_pointer );

                             // class in cache_s
                            cache->hir_list.push_front ( cache->cache_list.back() );
                            cache->cache_list.back()->type = HIR_RESIDENT;
                        }

                        Clear_Cache_List ( cache );

                        hash_elem_pointer->place_in_cache_list = cache->cache_list.size();
                    }
                }
            }
            else if ( elem_type == HIR_NO_RESIDENT ) {
 $              hash_elem_pointer->IRR=(hash_elem_pointer->location == INF) ? INF : fabs(cache->cache_list.size()-hash_elem_pointer->location ); 
                hash_elem_pointer->type = LIR;
                hash_elem_pointer->place_in_cache_list = true; // added

                if ( cache->cache_list.size() > 1 ) {
                    cache->cache_list.remove ( hash_elem_pointer );
                    cache->cache_list.push_front ( hash_elem_pointer );

                    cache->cache_list.back()->type = HIR_RESIDENT;
                    cache->hir_list.push_front ( cache->cache_list.back() );
                }
                Clear_Hir_List ( cache );
                Clear_Cache_List ( cache );

$               hash_elem_pointer->location = cache->cache_list.size();
            }
        }
    }

    return NO_ERRORS;
}

list_data_s *Find_Elem_Hash_Table ( cache_s *cache, int desired_elem ) // list_data_s *
{
    assert ( cache != nullptr );

    const int hash = cache->hash_int(desired_elem); // int
    //std::cout << ( hash );

    std::unordered_map<int, list_data_s *>::iterator elem_iterator = cache->cache_hash_table.find ( hash ); // int
    if ( elem_iterator == cache->cache_hash_table.end () ) {
        printf ( "No founded\n" ); //

        return nullptr; // something else
    }

    return elem_iterator->second;
} 

void Clear_Cache_List ( cache_s *cache )
{
    assert ( cache != nullptr );

    for ( int deleted_size = 0; ( cache->cache_list.back()->type ) != LIR; ) {
$       cache->cache_list.back()->location = INF;
        cache->cache_list.back()->place_in_cache_list = false;

        if ( cache->cache_list.back()->type == HIR_NO_RESIDENT ) {
            cache->cache_list.back()->type = NO_TYPE;
            cache->cache_hash_table.erase( cache->cache_hash_table.find ( cache->cache_list.back()->data ) );
        }
        else {
            assert ( cache->cache_list.back()->type == HIR_RESIDENT );
        }
        // free ()
        cache->cache_list.pop_back();

        ++deleted_size; // why

        //assert ( deleted_size < cache->cache_list.size() ); ///// add other asserts
    }

    //cache->current_cache_list_size -= deleted_size;
}

void Clear_Hir_List ( cache_s *cache )
{
    assert ( cache != nullptr );
$
    size_t const current_hir_list_size = cache->hir_list.size (); 

    if ( current_hir_list_size > cache->hir_size + 1 ) {
$       std::cout << "Hir list size error\n";

        return ;//HIR_LIST_SIZE_ERR;
    }
    else if ( current_hir_list_size == cache->hir_size + 1 ) {
 $      list_data_s *no_resident_hir = cache->hir_list.back();
        cache->hir_list.pop_back ();
                //errors
        list_data_s *no_resident_hir_pointer = Find_Elem_Hash_Table ( cache, no_resident_hir->data );
        assert ( no_resident_hir_pointer != nullptr );
        if ( no_resident_hir_pointer->place_in_cache_list == true ) {
$           no_resident_hir_pointer->type = HIR_NO_RESIDENT;
        }
        else {
$           no_resident_hir_pointer->type = NO_TYPE;
            no_resident_hir_pointer->location = INF;
            cache->cache_hash_table.erase( cache->cache_hash_table.find ( no_resident_hir_pointer->data ) );
            // free ( prev_hir_pointer )
        }
    }
}

void Print_List_Typo ( cache_s *cache )
{
    assert ( cache != nullptr );

    size_t start_size = cache->cache_list.size();

    for ( int i = 0; i < start_size; ++i ) {
        std::cout << "Data cache : " << cache->cache_list.front()->data << " node_type : " << cache->cache_list.front()->type << '\n';
        cache->cache_list.pop_front();
    }
    start_size = cache->hir_list.size();
    for ( int i = 0; i < start_size; ++i ) {
        std::cout << "Data hir : " << cache->hir_list.front()->data << " node_type : " << cache->hir_list.front()->type << '\n';
        cache->hir_list.pop_front();
    }
}