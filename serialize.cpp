#include <iostream>
#include <utility>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>

#define SMALL_DATA "small_time_series.txt"
#define BIG_DATA "time_series.txt"

using namespace std;

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
int populateDataStructure(string dataset){
	ifstream infile(dataset);
	map<string,set<pair<int,int>>> s;
	set<int> all_dates;
	for( std::string line; getline( infile, line ); ){
		int date,count;
		string page;
		tie(date,page,count) = parseLine(line);
		all_dates.insert(date);
		//The page doesn't exists on hashmap
		if (s.find(page) == s.end()) {
			set<pair<int,int>> page_dates;
			page_dates.insert(make_pair(date,count));
			s.insert(make_pair(page,page_dates));
		}
		//The page alredy exists
		else 
			s[page].insert(make_pair(date,count));
	}
	//for(auto const &elem : s){
		//cout << elem.first << endl;
		//for(auto const p : elem.second){
			//cout << "\t" << p.first << " " << p.second << endl;
		//}
	//}
}

int main(){
	populateDataStructure(SMALL_DATA);
}
