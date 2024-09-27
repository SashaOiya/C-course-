#pragma once

#include <iostream>
#include <map>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <ctime>

/*struct list_data_s {
    int data = 0; // not only int
    std::vector<int> cells = {};
    size_t current_irr = 0;
};*/
// namespace Cache - nado

struct cache_s {
    std::vector<int> data_vector = {};
                            // data
    std::unordered_map< int, std::vector<int>> data_hash_table  = {};
    std::hash<int> hash_int;

    std::list<int> cache_list = {};
                      // data, irr
    std::unordered_map< int, int> cache_hash_table = {}; // compulsory

    size_t capacity = 0; // _
    size_t emements_number = 0;
};
    //cache_t(int capacity): capacity_(capacity) {};
    // Cache::cache_t ideal_cache(capacity, number_of_elems) {  }

enum errors {
    NO_ERRORS = 0,
    CACHE_SIZE_ERR = 1,
    READ_FILE_ERR = 2
};

void Print_Vector ( std::vector<int> &data );
errors Cache_Ctor ( cache_s &cache, const char*file_name );
std::vector<int> *Find_Elem_Hash_Table ( cache_s &cache, int desired_elem );
int Find_IRR_Hash_Table ( cache_s &cache, int desired_elem );
size_t Filling_Cache ( cache_s &cache ) ;
void Print_List_Typo ( cache_s &cache );
/*
int Cache_Processing ( cache_s *cache );
bool compare(const std::pair<int, list_data_s *>&a, const std::pair<int, list_data_s *>&b);
std::vector<int> *Find_Elem_Hash_Table ( cache_s &cache, int desired_elem );
void Cache_Dtor ( cache_s *cache );*/