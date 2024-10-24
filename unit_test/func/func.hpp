#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <filesystem>
#include <gtest/gtest.h>

#include "lirs.hpp"
#include "ideal.hpp"

namespace test_funcs
{
	inline std::string get_result ( const std::string& filename, bool ideal )// use std::istringstream instead of std::ifstream
    {
        using key_type = int;
        using page_type = key_type;
        
        std::ifstream file(filename);
        if (!file) { throw "Error open file\n"; }

        size_t capacity = 0;
        file >> capacity;
        if ( !file.good() ) { throw "Error while reading the capacity of cache\n"; }
        
        size_t elements_number = 0;
        file >> elements_number;
        if ( !file.good() ) { throw "Error while reading the number of elements\n"; }

        int hits = 0;
        if ( ideal == true ) {
            std::deque<key_type> key_storage = {}; 
            for ( size_t i = 0; i < elements_number; ++i ) {
                key_type key = 0;
                file >> key;
                key_storage.push_back ( key );
            }

            IdealCache<key_type, page_type> cache { capacity, elements_number, key_storage.begin(), key_storage.end() };
            auto begin_itt = key_storage.begin(), end_itt = key_storage.end();

            for ( int itt_counter = 0; begin_itt + itt_counter != end_itt; ++itt_counter ) {
                hits += cache.lookup_update ( begin_itt[itt_counter] );
            }
        }
        else  {
            LirsCache<int, int> cache {capacity, elements_number};

            int key = 0;
            for ( size_t i = 0; i < elements_number; ++i ) { 
                file >> key;
                hits += cache.lookup_update(key);
            }
        }

        return std::to_string(hits);
    }

    std::string get_answer ( const std::string& filename )
    {
        std::ifstream answer_file(filename);

        std::string answer;
        answer_file >> answer;

        return answer;
    }

	void run_test ( const std::string& test_name, bool ideal )
	{
		std::string test_directory = "/test/";

        std::cout << TEST_DATA_DIR << "\n";
		std::string test_path = std::string ( TEST_DATA_DIR ) + test_directory + test_name;
		std::string result    = get_result ( test_path + ".dat", ideal );

        std::string answer_type = ideal ? "_ideal.ans" : "_lirs.ans";
		std::string answer = get_answer ( test_path + answer_type );

		EXPECT_EQ(result, answer);
	}
}
