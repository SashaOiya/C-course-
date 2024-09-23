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
    std::unordered_multimap< int, list_data_s *> data_hash_table  = {};
    std::unordered_multimap< int, list_data_s *> cache_hash_table = {};
          // irr, 
    std::multimap<int, list_data_s *> cache_map = {};
    std::hash<int> hash_int;
    size_t capacity = 0;
    size_t emements_number = 0;
};

void Print_Vector ( std::vector<int> &data );
void Ctor ( cache_s *cache );
size_t Filling_Cache ( cache_s *cache );
bool compare(const std::pair<int, list_data_s *>&a, const std::pair<int, list_data_s *>&b);
int Cache_Processing ( cache_s *cache );

int main () 
{
    cache_s cache = {};

    Ctor ( &cache );

    std::cout << Cache_Processing ( &cache );

    /*Filling_Cache ( &cache );

    for (const auto& p : cache.cache_hash_table)
        std::cout << p.first << " => " << p.second->current_irr << '\n';

    std::cout << (max_element(cache.cache_hash_table.begin(), cache.cache_hash_table.end(), compare)->second)->data; */

    /*std::multimap<int, int> x = {};
    x.emplace ( 1, 2);
    x.emplace ( 1, 3);
    x.emplace ( 2, 3);
    x.emplace ( 3, 5);

    for (const auto& p : x)
        std::cout << p.first << " => " << p.second << '\n';

    std::cout << max_element(x.begin(), x.end(), compare)->second; */

    return 0;
}

list_data_s *Find_Elem_Hash_Table ( cache_s *cache, std::unordered_multimap< int, list_data_s *> hash_table, int desired_elem );

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
            cache->data_hash_table.emplace ( element_pointer->data, element_pointer );
        }
$
        element_pointer->cells.push_back ( i );
        cache->data_vector.push_back ( new_element );
    }

    // error
}

list_data_s *Find_Elem_Hash_Table ( cache_s *cache, std::unordered_multimap< int, list_data_s *> hash_table, int desired_elem )
{
    assert ( cache != nullptr );

    const int hash = cache->hash_int(desired_elem); // int

    std::unordered_multimap<int, list_data_s *>::iterator elem_iterator = hash_table.find ( hash ); // int
    if ( elem_iterator == hash_table.end () ) {
        #ifdef DEBUG 
            std::cout << "No founded\n"; //
        #endif

        return nullptr; // something else
    }
$
    return elem_iterator->second;
} 

size_t Filling_Cache ( cache_s *cache );

bool compare(const std::pair<int, list_data_s *>&a, const std::pair<int, list_data_s *>&b)
{
   return a.second->current_irr < b.second->current_irr;
}

int Cache_Processing ( cache_s *cache )
{
    assert ( cache != nullptr );

    // add first data in cache_list
    size_t misses_total_n = Filling_Cache ( cache ); 

    size_t elem_n = cache->data_vector.size();
    for ( int i = 0; i < elem_n; ++i ) {
        // get next elem
        int element = cache->data_vector.front(); 
        cache->data_vector.erase ( cache->data_vector.begin() );

        // count its irr
        list_data_s *element_pointer = Find_Elem_Hash_Table ( cache, cache->data_hash_table, element ); 
        assert ( element_pointer != nullptr );
        size_t element_irr=(element_pointer->cells.size()>1) ? element_pointer->cells[1]-element_pointer->cells[0]-1 : element_irr=cache->data_vector.size()+1; 
        element_pointer->cells.erase( element_pointer->cells.begin() );
        element_pointer->current_irr = element_irr;

        // если элемент не найден, то прибавляем к переменной 
        if ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) == nullptr ) {
            // search max irr
            list_data_s * max_element_pointer = max_element(cache->cache_hash_table.begin(), cache->cache_hash_table.end(), compare)->second; 
            // delete its
            cache->cache_list.remove( max_element_pointer);
            cache->cache_hash_table.erase( cache->cache_hash_table.find ( max_element_pointer->data ) );
            // assert ( Find_Elem_Hash_Table ( ...) != nullptr )

            // insert in cache_list one
            cache->cache_list.push_front ( element_pointer );
            cache->cache_hash_table.emplace ( element_pointer->data, element_pointer );
            // assert ( Find_Elem_Hash_Table ( ...) != nullptr )
            ++misses_total_n;
        }
    }

    // errors
    return misses_total_n;
}

size_t Filling_Cache ( cache_s *cache ) //pair
{
    assert ( cache != nullptr );

    size_t misses_number = 0;
    for ( int i = 0; i < cache->capacity; ++i ) {
        // get element 
        int element = cache->data_vector.front(); 
        cache->data_vector.erase ( cache->data_vector.begin() );

        // get irr
        list_data_s *element_pointer = Find_Elem_Hash_Table ( cache, cache->data_hash_table, element ); 
        assert ( element_pointer != nullptr );
        size_t element_irr=(element_pointer->cells.size()>1) ? element_pointer->cells[1]-element_pointer->cells[0]-1 : element_irr=cache->data_vector.size()+1; 
        element_pointer->cells.erase( element_pointer->cells.begin() );

        // filling cache ( list, hash table, map )
        if ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) == nullptr ) {
            cache->cache_list.push_front ( element_pointer );
            cache->cache_hash_table.emplace ( element_pointer->data, element_pointer );
            // assert ( Find_Elem_Hash_Table ( ...) != nullptr )
            ++misses_number;
        }
        else { --i; }
        element_pointer->current_irr = element_irr;
    }

    return misses_number;
}

void Print_Vector ( std::vector<int> &data )
{
   std::cout << "DATA :" << '\n';

   for (int n : data )
        std::cout << n << ' ';
    std::cout << '\n';
$
}