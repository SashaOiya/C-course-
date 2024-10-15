#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <gtest/gtest.h>

#include "lirs/lirs.hpp"
#include "ideal.hpp"

namespace test_funcs
{
	std::string get_result ( const std::string& filename, bool ideal )
    {
        int hits = 0;
        std::ifstream file(filename);
        if (!file)
        {
            std::cout << "error\n";
            exit(1);
        }

        size_t capacity = 0;
        file >> capacity;
        if ( !std::cin.good() ) {
            std::cerr << "Error while reading the capacity of cache\n";

            exit(1);
        }
        
        size_t elements_number = 0;
        file >> elements_number;
        if ( !std::cin.good() ) {
            std::cerr << "Error while reading the number of elements\n";

            exit(1);
        }

        if ( ideal ) {
            std::deque<key_type> key_storage = {}; 
            for ( size_t i = 0; i < elements_number; ++i ) {
                key_type key = 0;
                std::cin >> key;
                key_storage.push_back ( key );
            }

            IdealCache<key_type, page_type> cache { capacity, elements_number, key_storage.begin(), (key_storage.end()) };
            size_t hits = 0;
            auto begin_itt = key_storage.begin();

            for ( int itt_counter = 0; begin_itt + itt_counter != key_storage.end(); ++itt_counter ) {
                hits += cache.lookup_update ( begin_itt[itt_counter] );
            }
            std::cout << hits << '\n';
        }
        else  {
            LirsCache<int, int> cache {capacity, elements_number};

            int key = 0;
            for ( size_t i = 0; i < elements_number; ++i ) { 
                file >> key;
                output += cache.lookup_update(key);
            }
            hits = elements_number - output;
        }
        file.close();

        return std::to_string(hits);
    }

    std::string get_answer ( const std::string& filename )
    {
        std::ifstream answer_file(filename);

        std::string answer;
        answer_file >> answer;

        answer_file.close();

        return answer;
    }

	void run_test ( const std::string& test_name, bool ideal )
	{
		std::string test_directory = "/test/";

		std::string test_path = std::string ( TEST_DATA_DIR ) + test_directory + test_name;
		std::string result    = get_result ( test_path + ".txt", ideal );

        std::string answer_type = ideal ? "_ideal.txt" : "_lirs.txt";

		std::string answer = get_answer ( test_path + answer_type );

		EXPECT_EQ(result, answer);
	}
}