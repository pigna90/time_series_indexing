#ifndef TIMESERIES_HPP
#define TIMESERIES_HPP

#include <map>
#include <set>
#include <tuple>
#include <string>
#include <vector>
#include <fstream>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>

using namespace std;

class TimeSeries{
private:
	map<string,set<tuple<int,int>>> ts;
	
	tuple<int, string, int> parseLine(const string &line);
	void populateDataStructure(const string &dataset);
	template <class Archive> void serialize(Archive & archive);
public:
	TimeSeries(const string &dataset);
	TimeSeries();
	~TimeSeries();
	
	void save_data(const string &file_name);
	void load_data(const string &file_name);
	vector<int> range(const string &page, const int time1, const int time2);
	void print_time_series(void);
};

#endif
