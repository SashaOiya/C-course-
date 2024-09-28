#ifndef LIRS_H
#define LIRS_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <vector>

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

struct list_data_s {
    int data = 0; // not only int
    data_type type = NO_TYPE;
    bool place_in_cache_list = false;
    std::list< int>::iterator cache_iterator = {};
    std::list< int>::iterator hir_iterator = {};
#define INF -1
};

struct cache_s { 
        std::list<int> cache_list = {};   // hir_resident + hir_no_resident + lir
        std::list<int> hir_list    = {};  // only hir_resident
        std::unordered_map< int, list_data_s > cache_hash_table  = {}; // not only int
        size_t capacity = 0;
        size_t lir_size = 0;
        size_t hir_size = 0; 
        size_t elements_number = 0;
        size_t currennt_lir_n = 0;  // to control the size and trim the cache_list
        std::hash<int> hash_int; // int ?
}; 

errors Get_Elem ( cache_s &cache );
int Cache_Ctor ( cache_s &cache, const char *file_name );
list_data_s *Find_Elem_Hash_Table ( cache_s &cache, int desired_elem ); // const
void Clear_Cache_List ( cache_s &cache );
void Clear_Hir_List ( cache_s &cache );                               // int
int Cache_Processing ( cache_s &cache, list_data_s *hash_elem_pointer, const int element );

#endif      // LIRS_H