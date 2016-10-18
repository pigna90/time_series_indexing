#include <iostream>
#include <utility>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>
#include <queue>
#include <stdlib.h>

using namespace std;

//Alias for main data structure
using d_structure =  map<string,set<tuple<int,int>>>;

void print_data(d_structure);

//Serialization
void save_data(d_structure, string);
d_structure load_data(string);
pair<int, int> getMaxMinDate(string);
d_structure populateDataStructure(string);

//Queries
vector<int> range_tuple(d_structure, string , int , int);
vector<tuple<int,int>> top_k_range_tuple(d_structure, string, int, int, int);
