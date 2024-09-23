#include "ideal.h"

//#define DEBUG

#ifdef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

int main ( int argc, char* argv[] ) 
{
    cache_s cache = {};

    Cache_Ctor ( &cache, (argc > 1) ? argv[1] : nullptr );

    size_t output = cache.emements_number - Cache_Processing ( &cache );
    std::cout << output << '\n';
$
    Cache_Dtor ( &cache );
$
    return 0;
}

errors Cache_Ctor ( cache_s *cache, const char *file_name ) 
{
    assert ( cache != nullptr );

    std::ifstream file ( file_name );
    if ( !file ) {
        std::cout<< "FILE NOT FOUNDED\n";

        return READ_FILE_ERR; 
    }

    file >> cache->capacity;
    file >> cache->emements_number;
    cache->data_vector.reserve ( cache->emements_number );
$
    for ( int i = 0; i < cache->emements_number; ++i ) {
        int new_element = 0;
        file >> new_element;
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

    file.close();

    return NO_ERRORS;
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

        return nullptr; 
    }
$
    return elem_iterator->second;
} 

bool compare(const std::pair<int, list_data_s *>&a, const std::pair<int, list_data_s *>&b)
{
   return a.second->current_irr < b.second->current_irr;
}

int Cache_Processing ( cache_s *cache )
{
    assert ( cache != nullptr );

    // add first data in cache_list
    size_t misses_total_n = Filling_Cache ( cache ); 
$
    size_t elem_n = cache->data_vector.size();
    for ( int i = 0; i < elem_n; ++i ) {
        // get next elem
        int element = cache->data_vector.front(); 
        cache->data_vector.erase ( cache->data_vector.begin() );
$
        // count its irr
        list_data_s *element_pointer = Find_Elem_Hash_Table ( cache, cache->data_hash_table, element ); 
        assert ( element_pointer != nullptr );
        size_t element_irr=(element_pointer->cells.size()>1) ? element_pointer->cells[1]-element_pointer->cells[0]-1 : element_irr=cache->data_vector.size()+1; 
        element_pointer->cells.erase( element_pointer->cells.begin() );
        element_pointer->current_irr = element_irr;

        if ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) == nullptr ) {
            // search max irr
            list_data_s * max_element_pointer = max_element(cache->cache_hash_table.begin(), cache->cache_hash_table.end(), compare)->second; 
            // delete its
            cache->cache_list.remove( max_element_pointer);
            cache->cache_hash_table.erase( cache->cache_hash_table.find ( max_element_pointer->data ) );
            assert ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) == nullptr ); //
$
            // insert in cache_list one
            cache->cache_list.push_front ( element_pointer );
            cache->cache_hash_table.emplace ( element_pointer->data, element_pointer );
            assert ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element_pointer->data ) != nullptr );

            ++misses_total_n;
        }
    }

    return misses_total_n;
}

size_t Filling_Cache ( cache_s *cache ) 
{
    assert ( cache != nullptr );

    size_t misses_number = 0;
    for ( int i = 0; ( i < cache->capacity ) && ( cache->data_vector.size()); ++i ) {
        // get element 
        int element = cache->data_vector.front(); 
        cache->data_vector.erase ( cache->data_vector.begin() );

        // get irr
        list_data_s *element_pointer = Find_Elem_Hash_Table ( cache, cache->data_hash_table, element ); 
        assert ( element_pointer != nullptr );
        size_t element_irr=(element_pointer->cells.size()>1) ? element_pointer->cells[1]-element_pointer->cells[0]-1 : element_irr=cache->data_vector.size()+1; 
        element_pointer->cells.erase( element_pointer->cells.begin() );
$
        // filling cache ( list, hash table, map )
        if ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) == nullptr ) {
            cache->cache_list.push_front ( element_pointer );
            cache->cache_hash_table.emplace ( element_pointer->data, element_pointer );
            assert ( Find_Elem_Hash_Table ( cache, cache->cache_hash_table, element ) != nullptr );

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

void Cache_Dtor ( cache_s *cache )
{
    assert ( cache != nullptr );
    assert ( cache->data_vector.size() == 0 );
$
    int size = cache->data_hash_table.size();
    for ( int i = 0; i < cache->data_hash_table.size(); ++i ) {
        free ( cache->data_hash_table.begin()->second );
         cache->data_hash_table.erase ( cache->data_hash_table.begin() );
    }
    cache->cache_hash_table.clear();
    cache->cache_list.clear();
    cache->capacity = 0;
    cache->emements_number = 0;

    return;
} 