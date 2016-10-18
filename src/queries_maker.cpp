#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#define SMALL_DATA "data/small_time_series.txt"
#define BIG_DATA "data/time_series.txt"

using namespace std;

//Alias for main data structure
using d_s =  map<string,set<tuple<int,int>>>;

/*Read all dates from dataset and return max/min date
 * *
 * dataset: name of dataset file
 * *
 */
pair<int, int> getMaxMinDate(string dataset){
	vector<int> v;
	ifstream infile(dataset);
	for(string line; getline(infile, line);){
		string token = line.substr(0, line.find("\t"));
		string token_1 = token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1);
		v.push_back(atoi(token_1.c_str()));
	}
	int maxDate = *max_element(v.begin(), v.end());
	int minDate = *min_element(v.begin(), v.end());
	
	return make_pair(minDate, maxDate);
}

/*Parse a line of dataset and return date, page name and visit counter
 * *
 * line: line to parse eg. "20160626-23	2006_FIFA_World_Cup	198"
 * *
 */
tuple<int, string, int> parseLine(string line) {
	int date, count;
	string page, token;

	//Parse visit date
	token = line.substr(0, line.find("\t"));
	date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
	//Parse page name
	token = line.substr(line.find("\t")+1, line.length());
	page = token.substr(0, token.find("\t"));
	//Parse page counter
	count = atoi((token.substr(token.find("\t")+1, token.length())).c_str());

	return make_tuple(date, page, count);
}

/* Create the data structure from dataset.
 * *
 * dataset: name of dataset file
 * *
 */
d_s populateDataStructure(string dataset) {
	ifstream infile(dataset);
	d_s s;
	set<int> all_dates;
	for(string line; getline(infile, line); ) {
		int date, count;
		string page;
		tie(date, page, count) = parseLine(line);
		all_dates.insert(date);
		//The page doesn't exists on hashmap
		if (s.find(page) == s.end()) {
			set<tuple<int,int>> page_dates;
			page_dates.insert(make_tuple(date,count));
			s.insert(make_pair(page,page_dates));
		}
		//The page already exists
		else 
			s[page].insert(make_pair(date, count));
	}
	
	return s;
}

/* Create n random queries. Format "pageName dateMin dateMax k"
 * *
 * n: number of queries
 * k_min-k_max: range of k
 * dataset: input dataset file name
 * outPath: output file name
 * *
 */ 
void serializeQueries(int n, int k_min, int k_max,string dataset,string outPath){
	//Read structure from file
	d_s data = populateDataStructure(dataset);
	//Output file stream
	ofstream outfile;
	outfile.open(outPath,ios::out);
	
	//Get max and min date from dataset
	pair<int,int> dates = getMaxMinDate(dataset);
	for(int i=0; i<n; i++){
		d_s::iterator it_page = data.begin();
		//Iterate till a random page position on map
		advance(it_page,(rand() % data.size()));
		string page = it_page->first;

		//Compute two random date where date1 > date2
		int maxDate = rand() % (dates.second - dates.first+1) + dates.first;
		int minDate = rand() % (dates.second - maxDate+1) + dates.first;

		//Compute a random integer
		int k = rand() % (k_max-k_min+1) + k_min;

		outfile << page << " " << minDate << " " << maxDate << " " << k << endl;
	}
}

int main(int argc, char* argv[]){
	if(argc == 6){
		int n = stoi(argv[1]);
		int k_min = stoi(argv[2]);
		int k_max = stoi(argv[3]);
		string type_dataset = (*argv[4] == 's' ? SMALL_DATA : BIG_DATA);
		string out_file = argv[5];
				
		srand((unsigned)time(0));
		serializeQueries(n,k_min,k_max,type_dataset,out_file);
	}
	else
		cout << "Missing parameters" << endl << "Exit." << endl;
}
