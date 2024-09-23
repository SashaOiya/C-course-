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
    std::hash<int> hash_int;
    size_t capacity = 0;
    size_t emements_number = 0;
};

enum errors {
    NO_ERRORS = 0,
    CACHE_SIZE_ERR = 1,
    READ_FILE_ERR = 2
};

void Print_Vector ( std::vector<int> &data );
errors Cache_Ctor ( cache_s *cache, const char*file_name );
size_t Filling_Cache ( cache_s *cache );
int Cache_Processing ( cache_s *cache );
bool compare(const std::pair<int, list_data_s *>&a, const std::pair<int, list_data_s *>&b);
list_data_s *Find_Elem_Hash_Table ( cache_s *cache, std::unordered_multimap< int, list_data_s *> hash_table, int desired_elem );
size_t Filling_Cache ( cache_s *cache );
void Cache_Dtor ( cache_s *cache );