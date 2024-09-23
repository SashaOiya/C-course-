#include <iostream>
#include <map>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>
//
#include <utility>
#include <algorithm>

//#define DEBUG

#ifdef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

struct list_data_s {
    int data = 0; // not only int
    std::vector<int> cells = {};
    size_t current_irr = 0;
};

struct cache_s {
    std::vector<int> data_vector= {};
    std::list<list_data_s *> cache_list = {};
    std::unordered_map< int, list_data_s *> data_hash_table  = {};
    std::unordered_map< int, list_data_s *> cache_hash_table  = {};
          // irr, 
    std::multimap<int, list_data_s *> cache_map = {};
    std::hash<int> hash_int;
    size_t capacity = 0;
    size_t emements_number = 0;
};

void Print_Vector ( std::vector<int> &data );
void Ctor ( cache_s *cache );
int Filling_Cache ( cache_s *cache );

bool compare(const std::pair<int, int>&a, const std::pair<int, int>&b)
{
   return a.first<b.first;
}

int main () 
{
    cache_s cache = {};

    Ctor ( &cache );

    Filling_Cache ( &cache );

    /*for (const auto& p : cache.cache_map )
        std::cout << p.first <<'\n'; */

    return 0;
}

list_data_s *Find_Elem_Hash_Table ( cache_s *cache, std::unordered_map< int, list_data_s *> hash_table, int desired_elem );

void Ctor ( cache_s *cache ) 
{
    assert ( cache != nullptr );

    std::cin>> cache->capacity;

    std::cin>> cache->emements_number;
    cache->data_vector.reserve ( cache->emements_number );
$
    for ( int i = 0; i < cache->emements_number; ++i ) {
        int new_element = 0;
        std::cin>> new_element;
        list_data_s *element_pointer = Find_Elem_Hash_Table ( cache, cache->data_hash_table, new_element );
        if ( element_pointer == nullptr ) {
            element_pointer = (list_data_s *)calloc ( sizeof (list_data_s) * 1, 1 ); 
            // free
            assert ( element_pointer != nullptr );
            element_pointer->data = new_element;
$
            cache->data_hash_table.try_emplace ( element_pointer->data, element_pointer );
        }
$
        element_pointer->cells.push_back ( i );
        cache->data_vector.push_back ( new_element );
    }

    // error
}

list_data_s *Find_Elem_Hash_Table ( cache_s *cache, std::unordered_map< int, list_data_s *> hash_table, int desired_elem )
{
    assert ( cache != nullptr );

    const int hash = cache->hash_int(desired_elem); // int

    std::unordered_map<int, list_data_s *>::iterator elem_iterator = hash_table.find ( hash ); // int
    if ( elem_iterator == hash_table.end () ) {
        #ifdef DEBUG 
            std::cout << "No founded\n"; //
        #endif

        return nullptr; // something else
    }
$
    return elem_iterator->second;
} 

int Filling_Cache ( cache_s *cache );

void Cache_Processing ( cache_s *cache )
{
    assert ( cache != nullptr );

    // add first data in cache_list
    Filling_Cache ( cache ); // return value

    size_t elem_n = cache->data_vector.size();
    for ( int i = 0; i < elem_n; ++i ) {

    }
    
    // processing ( the )
    // search the biggest 

    // errors
}

int Filling_Cache ( cache_s *cache ) //pair
{
    assert ( cache != nullptr );

    //size_t max_irr = 0;

    for ( int i = 0; i < cache->capacity; ++i ) {
        // get element 
        int element = cache->data_vector.front(); 
        cache->data_vector.erase ( cache->data_vector.begin() );

        // get irr
        list_data_s *element_pointer = Find_Elem_Hash_Table ( cache, cache->data_hash_table, element ); 
        assert ( element_pointer != nullptr );
        size_t element_irr = 0;
        if ( element_pointer->cells.size() > 1 ) {
            element_irr = element_pointer->cells[1] - element_pointer->cells[0] - 1;
        }
        else {
            element_irr = cache->data_vector.size() + 1;
        }
        element_pointer->cells.erase( element_pointer->cells.begin() );

        // filling cache ( list, hash table, map )
        if ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) == nullptr ) {
            cache->cache_list.push_front ( element_pointer );
            cache->cache_hash_table.try_emplace ( element_pointer->data, element_pointer );
            // assert ( Find_Elem_Hash_Table ( ...) != nullptr )
            cache->cache_map.emplace ( element_irr, element_pointer );
        }
        else { --i; }
        element_pointer->current_irr = element_irr;

        // search the biggest 
        //max_irr = ( element_irr > max_irr ) ? element_irr : max_irr;
    }

    return 0;
}

int Processing ( cache_s *cache )
{
    assert ( cache != nullptr );


    return 0;//
}  

void Print_Vector ( std::vector<int> &data )
{
   std::cout << "DATA :" << '\n';

   for (int n : data )
        std::cout << n << ' ';
    std::cout << '\n';
$
}