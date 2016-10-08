#include <iostream>
#include <utility>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <queue>
#include <stdlib.h>

#define SMALL_DATA "data/small_time_series.txt"
#define BIG_DATA "data/time_series.txt"

using namespace std;

//Alias for main data structure
using d_structure = map<string, set<pair<int, int>>>;

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
		if(atoi(token_1.c_str()) == 0)
			cout << line << endl;
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
d_structure populateDataStructure(string dataset) {
	ifstream infile(dataset);
	d_structure s;
	set<int> all_dates;
	for(string line; getline(infile, line); ) {
		int date, count;
		string page;
		tie(date, page, count) = parseLine(line);
		all_dates.insert(date);
		//The page doesn't exists on hashmap
		if (s.find(page) == s.end()) {
			set<pair<int, int>> page_dates;
			page_dates.insert(make_pair(date, count));
			s.insert(make_pair(page, page_dates));
		}
		//The page already exists
		else 
			s[page].insert(make_pair(date, count));
	}
	
	return s;
}

/* Return a vector with all the counters of page in the time interval [time1, time2]
 * *
 * s: structure previously populated
 * page: string of the selected webpage
 * time1: start of the selected range
 * time2: finish of the selected range
 * *
 */
vector<int> range(d_structure s, string page, int time1, int time2) {
    vector<int> result;

    for(auto const e : s[page]) {
        if (e.first >= time1 && e.first <= time2) {
                result.emplace_back(e.second);
        }
    }

    return result;
}

/* Return a vector with all the counters of page in the time interval [time1, time2] in tuple version
 * *
 * s: structure previously populated
 * page: string of the selected webpage
 * time1: start of the selected range
 * time2: finish of the selected range
 * *
 */
vector<int> range_tuple(d_structure s, string page, int time1, int time2) {
    vector<int> result;

    for(auto const e : s[page]) {
        if (get<0>(e) >= time1 && get<0>(e) <= time2) {
            result.emplace_back(get<1>(e));
        }
    }

    return result;
}

/* Comparing class in order to obtain Max Heap
 * *
 * operator(): using second-value ordering for object of type std::pair<int,int>
 * *
 */
class Compare {
    public:
        bool operator() (const pair<int, int>& p1, const pair<int, int>& p2) {
            return p1.second < p2.second;
        }
};

/* Comparing class in order to obtain Max Heap in tuple version
 * *
 * operator(): using second-value ordering for object of type std::pair<int,int>
 * *
 */
class Compare_tuple {
public:
    bool operator() (const tuple<int, int>& p1, const tuple<int, int>& p2) {
        return get<1>(p1) < get<1>(p2);
    }
};

/* Return a heap structure with all the top K counters of page in the time interval [time1, time2]
 * *
 * s: structure previously populated
 * page: string of the selected webpage
 * time1: start of the selected range
 * time2: finish of the selected range
 * k: number of counters required
 * *
 */
vector<pair<int,int>> top_k_range(d_structure s, string page, int time1, int time2, int k) {
    priority_queue<pair<int,int>, vector<pair<int,int>>, Compare> top_k(s[page].begin(), s[page].end());

    vector<pair<int,int>> result(k);

    for(int i = 0; i < k; ++i) {
        if(top_k.top().first >= time1 && top_k.top().first <= time2) {
            result[i] = top_k.top();
        } else {
            --i;
        }
        top_k.pop();
    }

    return result;
}

/* Return a heap structure with all the top K counters of page in the time interval [time1, time2] in tuple version
* *
* s: structure previously populated
* page: string of the selected webpage
* time1: start of the selected range
* time2: finish of the selected range
* k: number of counters required
* *
*/
vector<tuple<int,int>> top_k_range_tuple(d_structure s, string page, int time1, int time2, int k) {
    priority_queue<tuple<int,int>, vector<tuple<int,int>>, Compare_tuple> top_k(s[page].begin(), s[page].end());

    vector<tuple<int,int>> result(k);

    for(int i = 0; i < k; ++i) {
        if(get<0>(top_k.top()) >= time1 && get<0>(top_k.top()) <= time2) {
            result[i] = top_k.top();
        } else {
            --i;
        }
        top_k.pop();
    }

    return result;
}

int main(){
	d_structure s = populateDataStructure(SMALL_DATA);

    /*
    for(auto const &elem : s) {
        cout << elem.first << endl;
        for (auto const p : elem.second) {
            cout << "\t" << p.first << " " << p.second << endl;
        }
    }
    */

    /*
     * Example of using range function
     */

    string page = "2016_Formula_One_season";
    int time1 = 2016050100;
    int time2 = 2016050108;

    vector<int> finding = range(s, page, time1, time2);
    for (int i = 0; i < finding.size(); ++i) {
        cout << finding[i] << endl;
    }

    /*
     * Example of using top_k_range function
     */

    int k = 3;
    vector<pair<int,int>> looking = top_k_range(s, page, time1, time2, k);
    for (int i = 0; i < looking.size(); ++i) {
        cout << looking[i].first << "-" << looking[i].second << endl;
    }

    /*
     * Example of using top_k_range_tuple function
     */

    int g = 3;
    vector<tuple<int,int>> looking_tuple = top_k_range_tuple(s, page, time1, time2, g);
    for (int i = 0; i < looking.size(); ++i) {
        cout << get<0>(looking[i]) << "-" << get<1>(looking[i]) << endl;
    }

    return 0;
}
