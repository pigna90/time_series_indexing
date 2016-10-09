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

#define SMALL_DATA "data/small_time_series.txt"
#define BIG_DATA "data/time_series.txt"

using namespace std;

//Alias for main data structure
using d_structure =  map<string,set<tuple<int,int>>>;

/*Implementation of cereal interface for serialization
 * *
 * ar: type of archive
 * data: data structure to serialize
 * *
 */
template <class Archive>
void serialize( Archive & ar ,d_structure & data){
	ar( data);
}

/*Serialize data on binary file
 * *
 * data: data structure to serialize
 * file_name: name of binary file
 * *
 */
void save_data(d_structure data,string file_name){
	std::ofstream os(file_name,std::ios::binary);                    
	cereal::BinaryOutputArchive output(os); // stream to cout
	output(data);
}

/*Deserialize data from binary file
 * *
 * file_name: name of binary file
 * *
 */
d_structure load_data(string file_name){
	d_structure data;
	std::ifstream is(file_name,std::ios::binary);
	cereal::BinaryInputArchive input(is);
	input(data);
	return data;
}

/*Read all dates from dataset and return max/min date
 * *
 * dataset: name of dataset file
 * *
 */
pair<int,int> getMaxMinDate(string dataset){
	vector<int> v;
	ifstream infile(dataset);
	for( string line; getline( infile, line ); ){
		string token = line.substr(0, line.find("\t"));
		string token_1 = token.substr(0, line.find("-")) + token.substr(line.find("-")+1,token.length()-1);
		if(atoi(token_1.c_str()) == 0)
			cout << line << endl;
		v.push_back(atoi(token_1.c_str()));
	}
	int maxDate = *max_element(v.begin(),v.end());
	int minDate = *min_element(v.begin(),v.end());
	
	return make_pair(minDate,maxDate);
}

/*Parse a line of dataset and return date,page name and visit counter
 * *
 * line: line to parse eg. "20160626-23	2006_FIFA_World_Cup	198"
 * *
 */
tuple<int,string,int> parseLine(string line){
	int date, count;
	string page,token;

	//Parse visit date
	token = line.substr(0, line.find("\t"));
	date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1,token.length()-1)).c_str());
	//Parse page name
	token = line.substr(line.find("\t")+1,line.length());
	page = token.substr(0,token.find("\t"));
	//Parse page counter
	count = atoi((token.substr(token.find("\t")+1,token.length())).c_str());

	return make_tuple(date,page,count);
}

/* Create the data structure from dataset.
 * *
 * dataset: name of dataset file
 * *
 */
d_structure populateDataStructure(string dataset){
	ifstream infile(dataset);
	d_structure s;
	set<int> all_dates;
	for( std::string line; getline( infile, line ); ){
		int date,count;
		string page;
		tie(date,page,count) = parseLine(line);
		all_dates.insert(date);
		//The page doesn't exists on hashmap
		if (s.find(page) == s.end()) {
			set<tuple<int,int>> page_dates;
			page_dates.insert(make_tuple(date,count));
			s.insert(make_pair(page,page_dates));
		}
		//The page alredy exists
		else 
			s[page].insert(make_pair(date,count));
	}
	
	return s;
}

/*Print out data on std::cout
 * *
 * */
void print_data(d_structure data){
	for(auto const &elem : data){
		cout << elem.first << endl;
		for(auto const p : elem.second){
			int date;
			int count;
			tie(date, count) = p;
			cout << "\t" << date << " " << count << endl;
		}
	}
}

int main(){
	//d_structure data= populateDataStructure(SMALL_DATA);

	d_structure in = load_data("/tmp/data.bin");
	print_data(in);
}
