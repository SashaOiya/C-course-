#include "lirs.h"

#define NDEBUG

void Print_List_Typo ( cache_s cache ); //

int main ( int argc, char* argv[] )
{
    cache_s cache = {};
    unsigned int start_time =  clock();

    int output = Cache_Ctor ( cache, (argc > 1) ? argv[1] : nullptr );

    unsigned int end_time = clock();
    std::cout << cache.elements_number - output << '\n';

    Print_List_Typo ( cache );
    printf ( "Successfully. Time = %d s \n", ( end_time - start_time ) / 1000 );

    return 0;
}

void Get_Hir_Lir_Size ( cache_s &cache );

int Cache_Ctor ( cache_s &cache, const char *file_name ) 
{
    std::ifstream file ( file_name );
    if ( !file ) {
        std::cout<< "FILE NOT FOUNDED\n";

        return -1; 
    }
    file >> cache.capacity >> cache.elements_number;
    Get_Hir_Lir_Size ( cache );

    cache.cache_hash_table.reserve ( cache.elements_number );
    //cache.cache_list.reserve ( cache.capacity );
    //cache.hir_list.reserve ( cache.hir_size );

    int output = 0;
    for (  int i = 0; i < cache.elements_number; ++i ) {
        int element = 0; // not only int
        file >> element;

$       list_data_s *hash_elem_pointer = Find_Elem_Hash_Table ( cache, element ); 

        output += Cache_Processing ( cache, hash_elem_pointer, element );
    }
    file.close();

    return output;
}

void Get_Hir_Lir_Size ( cache_s &cache )
{
    if ( cache.capacity >= 10 ) {
        cache.hir_size = cache.capacity * 2 / 10;
        cache.lir_size = cache.capacity - cache.hir_size;
    }
    else if ( cache.capacity < 10 ) {
        cache.hir_size = cache.capacity / 3;
        cache.lir_size = cache.capacity - cache.hir_size;
    }
    else if ( cache.capacity <= 2 ) {
        cache.lir_size = cache.capacity / 2;
        cache.hir_size = cache.capacity - cache.lir_size;
    } 
$
    std::cout << cache.hir_size << '\t' << cache.lir_size << '\n'; // delete
}

void Checking_Lir_Size ( cache_s &cache, int data );

int Cache_Processing ( cache_s &cache, list_data_s *hash_elem_pointer, const int element ) // int cringe 
{
    if ( hash_elem_pointer == nullptr ) {
$       list_data_s temp_list_elem = {};

        temp_list_elem.data = element;
        temp_list_elem.type = HIR_RESIDENT;
        temp_list_elem.place_in_cache_list = true; 

        //cache.cache_list.push_front ( element ); // temp_lis
        //cache.hir_list.push_front ( element ); // temp_list
        cache.cache_list.push_back ( element ); // temp_lis
        temp_list_elem.cache_iterator = --( cache.cache_list.end() );
        cache.hir_list.push_back ( element ); 
        temp_list_elem.hir_iterator = --( cache.hir_list.end() );
        cache.cache_hash_table.try_emplace ( element, temp_list_elem );
        Clear_Hir_List ( cache ); //

        return 1; //
    }
    else {
        const data_type &elem_type = hash_elem_pointer->type;
        const int &hash_elem = hash_elem_pointer->data;
        assert ( elem_type != NO_TYPE );

        if ( elem_type == HIR_RESIDENT ) {
            if ( hash_elem_pointer->place_in_cache_list == false ) {
 $              hash_elem_pointer->place_in_cache_list = true;

                //cache.cache_list.push_front ( hash_elem );
                cache.cache_list.push_back ( hash_elem );
                hash_elem_pointer->cache_iterator = --( cache.cache_list.end() );
                cache.hir_list.erase ( hash_elem_pointer->hir_iterator ); // class in cache_s // --
                //cache.hir_list.push_front ( hash_elem );
                cache.hir_list.push_back ( hash_elem );
                hash_elem_pointer->hir_iterator = --( cache.hir_list.end() );
            }
            else {
$               hash_elem_pointer->type = LIR;
                ++(cache.currennt_lir_n);
                cache.hir_list.erase ( hash_elem_pointer->hir_iterator ); // --

                Checking_Lir_Size ( cache, hash_elem ); // may be *
                hash_elem_pointer->place_in_cache_list = cache.cache_list.size();
            }
        }
        else if ( elem_type == LIR ) {  
$           cache.cache_list.erase ( hash_elem_pointer->cache_iterator ); // --
            cache.cache_list.push_back ( hash_elem );
            hash_elem_pointer->cache_iterator = --( cache.cache_list.end() );
            Clear_Cache_List ( cache );
        }  
        else if ( elem_type == HIR_NO_RESIDENT ) {
$           hash_elem_pointer->type = LIR;
            ++(cache.currennt_lir_n);
            hash_elem_pointer->place_in_cache_list = true;

            Checking_Lir_Size ( cache, hash_elem );
            Clear_Hir_List ( cache );
        }
    }

    return 0;
}

void Checking_Lir_Size ( cache_s &cache, int data )
{
    if ( cache.cache_list.size() > SINGLE_CACHE_SIZE ) { 
        list_data_s *data_pointer = Find_Elem_Hash_Table ( cache, data );
        cache.cache_list.erase ( data_pointer->cache_iterator ); // --
        cache.cache_list.push_back ( data );
        data_pointer->cache_iterator = --( cache.cache_list.end() );

        assert ( cache.currennt_lir_n <= cache.lir_size + SINGLE_CACHE_SIZE );
        if ( cache.currennt_lir_n == cache.lir_size + SINGLE_CACHE_SIZE ) {
            list_data_s *hash_emen_pointer = Find_Elem_Hash_Table ( cache, cache.cache_list.front() );
$           hash_emen_pointer->type = HIR_RESIDENT; 
            --(cache.currennt_lir_n);
            cache.hir_list.push_back ( cache.cache_list.front() );
            list_data_s *cache_back_pointer = Find_Elem_Hash_Table ( cache, cache.cache_list.front() );
            cache_back_pointer->hir_iterator = --( cache.hir_list.end() );
        }
    }
    Clear_Cache_List ( cache );
}
                                                     // not onle int
list_data_s *Find_Elem_Hash_Table ( cache_s &cache, int desired_elem )
{
    const int hash = cache.hash_int(desired_elem); // int
    std::unordered_map<int, list_data_s >::iterator elem_iterator = cache.cache_hash_table.find ( hash ); // int
    if ( elem_iterator == cache.cache_hash_table.end () ) {
        #ifdef DEBUG 
            std::cout << "No founded\n"; //
        #endif

        return nullptr; 
    }

    return &(elem_iterator->second);
} 

void Clear_Cache_List ( cache_s &cache )
{
    list_data_s *hash_elem_pointer = Find_Elem_Hash_Table ( cache, cache.cache_list.front() );
    assert ( hash_elem_pointer != nullptr );
    while ( ( hash_elem_pointer->type ) != LIR ) {
$       //list_data_s &list_back = cache.cache_list.back();
        hash_elem_pointer->place_in_cache_list = false;

        if ( hash_elem_pointer->type == HIR_NO_RESIDENT ) {
            hash_elem_pointer->type = NO_TYPE;
            cache.cache_hash_table.erase( cache.cache_hash_table.find ( hash_elem_pointer->data ) );
        }
        else { // delete
            assert ( hash_elem_pointer->type == HIR_RESIDENT );
        }
        cache.cache_list.erase ( hash_elem_pointer->cache_iterator );
        // delete ???
        hash_elem_pointer = Find_Elem_Hash_Table ( cache, cache.cache_list.front() );
    }
}

void Clear_Hir_List ( cache_s &cache )
{
$   size_t const current_hir_list_size = cache.hir_list.size (); 
    assert ( current_hir_list_size <= cache.hir_size + SINGLE_CACHE_SIZE );

    if ( current_hir_list_size == cache.hir_size + SINGLE_CACHE_SIZE ) {
        list_data_s *no_resident_hir_pointer = Find_Elem_Hash_Table ( cache, cache.hir_list.back() );
 $      //list_data_s no_resident_hir = cache.hir_list.back();
        cache.hir_list.pop_back ();
        // iterator ??
        
        //list_data_s *no_resident_hir_pointer = Find_Elem_Hash_Table ( cache, no_resident_hir->data );
        assert ( no_resident_hir_pointer != nullptr );
        
        if ( no_resident_hir_pointer->place_in_cache_list == true ) {
$           no_resident_hir_pointer->type = HIR_NO_RESIDENT;
        }
        else {
$           no_resident_hir_pointer->type = NO_TYPE;
            cache.cache_hash_table.erase( cache.cache_hash_table.find ( no_resident_hir_pointer->data ) );
        }
    }
}

void Print_List_Typo ( cache_s cache )
{
    size_t start_size = cache.cache_list.size();

    for ( int i = 0; i < start_size; ++i ) {
        std::cout << "Data cache : " << cache.cache_list.back() << " node_type : " << Find_Elem_Hash_Table ( cache, cache.cache_list.back() )->type << '\n';
        cache.cache_list.pop_back();
    }

    start_size = cache.hir_list.size();
    for ( int i = 0; i < start_size; ++i ) {
        std::cout << "Data hir : " << cache.hir_list.back() << " node_type : " << Find_Elem_Hash_Table ( cache, cache.hir_list.back() )->type << '\n';
        cache.hir_list.pop_back();
    }
}

/*void Cache_Dtor ( cache_s &cache )
{
    size_t size = cache.cache_hash_table.size();
    for ( int i = 0; i < size; ++i ) {
        std::unordered_map<int, list_data_s>::iterator elem_iterator = cache.cache_hash_table.begin();
        cache.cache_hash_table.erase ( cache.cache_hash_table.begin() );
    }
$
} */