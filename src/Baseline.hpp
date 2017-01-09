#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <cereal/types/unordered_map.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

class Baseline {
private:
	//Main data structure for time series
	std::unordered_map<std::string, std::vector<uint32_t>> m_time_series;
	//Map of dates for lookup
	std::map<uint32_t,size_t> m_map_dates;
	//Vector of dates
	std::vector<uint32_t> m_dates;

	template <class Archive>
	void serialize(Archive &archive) {
		archive(m_time_series, m_map_dates, m_dates);
	}

public:
	Baseline() {}
	~Baseline() {}

	/* Reads time series from file and populate data structure
	* *
	* dataset_name: dataset file name
	*/
	Baseline(const std::string &dataset_name) {
		std::vector<std::string> lines;
		std::ifstream infile(dataset_name);
		std::string token;

		//Reads all dates available on data
		for(std::string line; getline(infile, line); ) {
			lines.push_back(line);
			token = line.substr(0, line.find("\t"));
			uint32_t date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
			m_dates.push_back(date);
		}
		infile.close();

		//Sorting and erasing duplicates from dates vector
		std::sort(m_dates.begin(), m_dates.end());
		m_dates.erase( unique( m_dates.begin(), m_dates.end() ), m_dates.end());

		//HashTable for retrieve lookup index position instead
		//of find it from vector
		size_t value = 0;
		for(auto d: m_dates){
			m_map_dates.insert(std::make_pair(d,value));
			value++;
		}

		for(auto line: lines){
			uint32_t date, count;
			std::string page, token;

			//Parse visit date
			token = line.substr(0, line.find("\t"));
			date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
			//Parse page name
			token = line.substr(line.find("\t")+1, line.length());
			page = token.substr(0, token.find("\t"));
			//Parse page counter
			count = atoi((token.substr(token.find("\t")+1, token.length())).c_str());

			if (m_time_series.find(page) == m_time_series.end()) {
				std::vector<uint32_t> visits(m_dates.size(),0);
				m_time_series.insert(make_pair(page,visits));
			}
			(m_time_series[page])[m_map_dates[date]] = count;
		}
	}

	/* Return all pages */
	std::vector<std::string> get_pages() const{
		std::vector<std::string> pages;
		for(auto page: m_time_series)
			pages.push_back(page.first);
		return pages;
	}

	/* Return all dates */
	std::vector<uint32_t> get_dates() const{
		return m_dates;
	}

	/* Serialize data structure on binary file
	 * *
	 * file_name: binary file name
	 */
	void serialize_data(const std::string &file_name) const {
		std::ofstream os(file_name, std::ios::binary);
		cereal::BinaryOutputArchive output(os);
		output(m_time_series, m_map_dates, m_dates);
	}

	/* Load data structure from a binary file
	 * *
	 * file_name: name of binary file
	 */
	void load_data(const std::string &file_name) {
		std::ifstream is(file_name, std::ios::binary);
		cereal::BinaryInputArchive input(is);
		input(m_time_series, m_map_dates, m_dates);
	}

	/* Return a vector with all the counters of page
	 * in the time interval [time1, time2] in tuple version
	 * *
	 * page: string of the selected webpage
	 * time1: start of the selected range
	 * time2: finish of the selected range
	 */
	inline std::vector<uint32_t> range(
		const std::string &page, uint32_t time1, uint32_t time2) const {

		const auto pg = m_time_series.at(page);
		const auto lt_end = m_map_dates.at(time1);
		const auto rt_end = m_map_dates.at(time2);

		std::vector<uint32_t> result;
		result.reserve(rt_end - lt_end + 1);

		for(size_t idx = lt_end; idx <= rt_end; ++idx)
			result.push_back(pg[idx]);

		return result;
	}

	/* Return a heap structure with all the top K counters of page
	* in the time interval [time1, time2] in tuple version
	* *
	* page: string of the selected webpage
	* time1: start of the selected range
	* time2: finish of the selected range
	* k: number of counters required
	*/
	inline std::vector<std::pair<uint32_t, uint32_t>> rangeTopK(
		const std::string &page, uint32_t time1, uint32_t time2, uint32_t k) const {

		const auto pg = m_time_series.at(page);
		const auto lt_end = m_map_dates.at(time1);
		const auto rt_end = m_map_dates.at(time2);

		auto compare = [](const std::pair<uint32_t, uint32_t> &p1,
					      const std::pair<uint32_t, uint32_t> &p2) -> bool { return p1.second > p2.second; };

		std::priority_queue<std::pair<uint32_t, uint32_t>,
							std::vector<std::pair<uint32_t, uint32_t>>,
							decltype(compare)> heap(compare);

		for(size_t idx = lt_end; idx <= rt_end; ++idx) {
			if(heap.size() < k)
				heap.emplace(m_dates[idx], pg[idx]);
			else if(heap.top().second < pg[idx]) {
				heap.emplace(m_dates[idx], pg[idx]);
				heap.pop();
			}
		}

		std::vector<std::pair<uint32_t, uint32_t>> result;
		size_t size = heap.size();
		result.reserve(size);

		for(size_t idx = 0; idx < size; ++idx) {
			result.push_back(heap.top());
			heap.pop();
		}

		return result;
	}

	/* Returns the memory size occupied by data structure*/
	size_t size() const {
		std::vector<std::pair<std::string, std::vector<uint32_t>>> values;

		for(auto idx = m_time_series.begin(); idx != m_time_series.end(); ++idx) {
			values.emplace_back(idx->first, idx->second);
		}

		size_t result = sizeof(std::string) * sizeof(std::vector<uint32_t>) * values.size();
		return result;
	}

	/* Print data structure content for debug*/
	void print(void) const {
		for(auto const &elem : m_time_series) {
			std::cout << elem.first << std::endl;
			std::vector<uint32_t> visits = elem.second;
			for(size_t i=0; i<m_dates.size(); ++i){
				std::cout << "\t" << m_dates[i] << " " << visits[i] << std::endl;
			}
		}
	}
};
