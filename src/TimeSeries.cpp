#ifndef TIMESERIES_CPP
#define TIMESERIES_CPP

#include "../include/TimeSeries.hpp"

tuple<int, string, int> TimeSeries::parseLine(const string &line) {
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

void TimeSeries::populateDataStructure(const string &dataset) {
	ifstream infile(dataset);
	set<int> all_dates;
	for(string line; getline(infile, line); ) {
		int date, count;
		string page;
		tie(date, page, count) = parseLine(line);
		all_dates.insert(date);
		//The page doesn't exists on hashmap
		if (ts.find(page) == ts.end()) {
			set<tuple<int,int>> page_dates;
			page_dates.insert(make_tuple(date,count));
			ts.insert(make_pair(page,page_dates));
		}
		//The page already exists
		else 
			ts[page].insert(make_pair(date, count));
	}
}

TimeSeries::TimeSeries(const string &dataset){
	populateDataStructure(dataset);
}

TimeSeries::TimeSeries(){}

TimeSeries::~TimeSeries(){}

template <class Archive>
void TimeSeries::serialize(Archive & archive){
	archive(ts);
}

void TimeSeries::save_data(const string &file_name){
	ofstream os(file_name,ios::binary);                    
	cereal::BinaryOutputArchive output(os); // stream to cout
	output(ts);
}

void TimeSeries::load_data(const string &file_name){
	ifstream is(file_name,ios::binary);
	cereal::BinaryInputArchive input(is);
	input(ts);
}

vector<int> TimeSeries::range(const string &page, const int time1, const int time2) {
    vector<int> result;

    for(auto const e : ts[page]) {
        if (get<0>(e) >= time1 && get<0>(e) <= time2) {
            result.emplace_back(get<1>(e));
        }
    }

    return result;
}

void TimeSeries::print_time_series(void){
	for(auto const &elem : ts){
		cout << elem.first << endl;
		for(auto const p : elem.second){
			int date;
			int count;
			tie(date, count) = p;
			cout << "\t" << date << " " << count << endl;
		}
	}
}

#endif
