#include "ideal.h"

#define NDEBUG

#ifdef DEBUG
#define $ std::cout<< __LINE__  << '\t' << __FUNCTION__ << '\n';
#else
#define $ ;
#endif

int main ( int argc, char* argv[] ) 
{
    cache_s cache = {};
    unsigned int start_time =  clock();

    Cache_Ctor ( cache, (argc > 1) ? argv[1] : nullptr );
    //Print_Vector ( *( Find_Elem_Hash_Table ( cache, 11119 )) );
    size_t output = Filling_Cache ( cache );
    //Print_List_Typo ( cache );
    output = cache.emements_number - Cache_Processing ( cache ) - output;
    std::cout << output << '\n';

    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    printf ( "Successfully. Time = %d s \n", search_time / 1000 );

    /*size_t output = cache.emements_number - Cache_Processing ( &cache );
    unsigned int end_time = clock();
    std::cout << output << '\n';
$
    unsigned int search_time = end_time - start_time;
    printf ( "Successfully. Time = %d s \n", search_time / 1000 );
    Cache_Dtor ( &cache ); */
$
    return 0;
}

errors Cache_Ctor ( cache_s &cache, const char *file_name ) 
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
    int elem_number = cache.emements_number;
    for ( int i = 0; i < elem_number; ++i ) {
        int new_element = 0;
        file >> new_element;
        std::vector<int> *element_pointer = Find_Elem_Hash_Table ( cache, new_element );

        if ( element_pointer == nullptr ) {
            std::vector<int> cell = {}; // vierd
$           cache.data_hash_table.emplace ( new_element, cell );
            element_pointer = Find_Elem_Hash_Table ( cache, new_element );
        }
        assert ( element_pointer != nullptr);

$       element_pointer->push_back ( i );
        cache.data_vector.push_back ( new_element );
    }

    file.close();

    return NO_ERRORS;
}

std::vector<int> *Find_Elem_Hash_Table ( cache_s &cache, int desired_elem )
{
    const int hash = cache.hash_int(desired_elem); // int

    std::unordered_multimap< int, std::vector<int>>::iterator elem_iterator = cache.data_hash_table.find ( hash ); // int
    if ( elem_iterator == cache.data_hash_table.end () ) {
        #ifdef DEBUG 
            std::cout << "No founded\n"; //
        #endif

        return nullptr; 
    }
$
    return &(elem_iterator->second);
} 

int Find_IRR_Hash_Table ( cache_s &cache, int desired_elem )
{
    const int hash = cache.hash_int(desired_elem); // int

    std::unordered_multimap< int, int>::iterator elem_iterator = cache.cache_hash_table.find ( hash ); // int
    if ( elem_iterator == cache.cache_hash_table.end () ) {
        #ifdef DEBUG 
            std::cout << "No founded\n"; //
        #endif

        return -1; 
    }
$
    return elem_iterator->second;
} 

size_t Filling_Cache ( cache_s &cache ) 
{
    size_t misses_number = 0;

    for ( int i = 0; ( i < cache.capacity ) && ( cache.data_vector.size()); ++i ) { // while
        // get element 
        int element = cache.data_vector.front();  // +
        cache.data_vector.erase ( cache.data_vector.begin() ); // +

        // get irr
        std::vector<int> *element_pointer = Find_Elem_Hash_Table ( cache, element ); 
        int element_irr = Find_IRR_Hash_Table ( cache, element );
        assert ( element_pointer != nullptr );
        assert ( element_pointer->size() >= 1 );
        if ( ( element_pointer->size() == 1 ) && ( element_irr == -1 ) ) {
            element_pointer->erase( element_pointer->begin() );
            cache.data_hash_table.erase ( cache.data_hash_table.find ( element ) );
            ++misses_number; 
            --i;
$
            continue;
            std::cout << "ERROR FILLING CACHE" << '\n'; //
        }
        size_t new_element_irr = (*element_pointer)[1] - (*element_pointer)[0] - 1; //
        element_pointer->erase( element_pointer->begin() ); // place
        //std::cout << "IRR   " << element_irr << "   DATA   " << element << '\n';
$
        // filling cache ( list, hash table, map )
        if ( element_irr == -1 ) { // thiiis
            cache.cache_list.push_front ( element );
            cache.cache_hash_table.emplace ( element, new_element_irr );
            assert ( Find_IRR_Hash_Table ( cache, element ) != -1 );

            ++misses_number;
        }
        else { 
            cache.cache_hash_table[element] = new_element_irr;
            --i;
        }
        //element_pointer->current_irr = element_irr;
    }

    return misses_number;
}

                            // T, int
bool compare ( const std::pair<int, int> &a, const std::pair<int, int> &b )
{
   return a.second < b.second;
}

int Cache_Processing ( cache_s &cache )
{
    // add first data in cache_list
    size_t misses_total_n = Filling_Cache ( cache ); 
$
    size_t elem_n = cache.data_vector.size();
    for ( int i = 0; i < elem_n; ++i ) {
        // get next elem
        int element = cache.data_vector.front();  // +
        cache.data_vector.erase ( cache.data_vector.begin() );
$
        // get irr
        std::vector<int> *element_vector_pointer = Find_Elem_Hash_Table ( cache, element ); 
        int irr = Find_IRR_Hash_Table ( cache, element );
        assert ( element_vector_pointer != nullptr );
        assert ( element_vector_pointer->size() >= 1 );
        if ( ( element_vector_pointer->size() == 1 ) && ( irr == -1 ) ) {
            element_vector_pointer->erase( element_vector_pointer->begin() );
            cache.data_hash_table.erase ( cache.data_hash_table.find ( element ) ); 
            
            ++misses_total_n;

            continue;
            std::cout << "ERROR FILLING CACHE" << '\n'; //
        }
        size_t element_irr = (*element_vector_pointer)[1] - (*element_vector_pointer)[0] - 1; //
        element_vector_pointer->erase( element_vector_pointer->begin() ); // place
        //element_pointer->current_irr = element_irr;

        if ( irr == -1 ) {
            // search max irr
            int max_irr_element = max_element(cache.cache_hash_table.begin(), cache.cache_hash_table.end(), compare)->first; 
            // delete its
            cache.cache_list.remove( max_irr_element );
            cache.cache_hash_table.erase( cache.cache_hash_table.find ( max_irr_element ) ); // why
            assert ( Find_IRR_Hash_Table ( cache, element ) == -1 ); //
$
            // insert in cache_list one
            cache.cache_list.push_front ( element );
            cache.cache_hash_table.emplace ( element, element_irr );
            assert ( Find_IRR_Hash_Table ( cache, element ) != -1 );

            ++misses_total_n;
        }
        else {
            cache.cache_hash_table[element] = element_irr;
        }
        // else found save new irr
    }

    return misses_total_n;
}


/*void Cache_Dtor ( cache_s *cache )
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
} */


void Print_Vector ( std::vector<int> &data )
{
   std::cout << "DATA :" << '\n';

   for (int n : data )
        std::cout << n << ' ';
    std::cout << '\n';
$
}

void Print_List_Typo ( cache_s &cache )
{
    size_t start_size = cache.cache_list.size();

    for ( int i = 0; i < start_size; ++i ) {
        std::cout << "Data cache : " << cache.cache_list.front() << '\n';
        cache.cache_list.pop_front();
    }
}
