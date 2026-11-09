#pragma once
/*
	my_short_headers_list.hpp
*/
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
using std::atomic_flag;
#include <random>
using std::default_random_engine;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::bernoulli_distribution;

#include <regex>
using std::smatch; using std::regex;
using std::sregex_iterator; using std::regex_search;
using namespace std::regex_constants;

#include <bitset>
using std::bitset;

#include <numeric>
using std::accumulate; 

#include <algorithm>
using std::equal_range; 

#include <tuple>
using std::tuple; using std::tuple_size; using std::make_tuple;
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <string>
using std::string;
#include <string.h>

#include <cstring>
using std::strcmp;

#include <iostream>
using std::cout; using std::endl; using std::cerr; 
using std::ostream; 
using std::istream; 
using std::istream_iterator;
using std::istreambuf_iterator;
using std::boolalpha; 
#include <fstream>
using std::ofstream; 
using std::fstream;
using std::ifstream;

#include <sstream>
using std::ostringstream;
using std::istringstream;
using std::stringstream;

#include <cstdlib> 				// for EXIT_FAILURE

#include <map>
using std::map;
#include <stdio.h>
#include <iterator>
using std::begin; using std::end;
#include <type_traits>
using std::remove_reference_t;
using std::is_const_v;
#include <functional>   // std::hash
using namespace std::chrono;
using namespace std::literals;

#include "../type_name_as_str.h"
#include "../IO_stream_in_file.hpp"
#include "concurrent_stack.hpp"
/*
 ********************************** 
*/
