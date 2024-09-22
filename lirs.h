#ifndef LIRS_H
#define LIRS_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include <ctime>

#ifdef DEBUG
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
};

struct cache_s { 
        std::list<list_data_s *> cache_list = {};   // hir_resident + hir_no_resident + lir
        std::list<list_data_s *> hir_list    = {};  // only hir_resident
        std::unordered_map< int, list_data_s *> cache_hash_table  = {}; // not only int
        size_t lir_size = 0;
        size_t hir_size = 0; 
        //size_t capacity = 0; aaaaaaaaaaallllllll
        size_t elements_number = 0;
        size_t currennt_lir_n = 0;  // to control the size and trim the cache_list
        std::hash<int> hash_int; // int ?
}; 

#endif      // LIRS_H