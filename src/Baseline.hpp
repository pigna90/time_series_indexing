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
/*#include <cereal/archives/binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>*/

class Baseline {
private:
	std::map<std::string, std::vector<uint32_t>> m_time_series;
	std::vector<uint32_t> m_dates;

	/*template <class Archive>
	void serialize(Archive &archive) {
		archive(m_time_series,m_dates);
	}*/

public:
	Baseline() {}
	~Baseline() {}

	Baseline(const std::string &dataset_name) {
		std::set<std::string> lines;
		std::ifstream infile(dataset_name);
		std::string token;
		
		for(std::string line; getline(infile, line); ) {
			lines.insert(line);
			token = line.substr(0, line.find("\t"));
			uint32_t date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
			if(std::find(m_dates.begin(), m_dates.end(), date) == m_dates.end())
				m_dates.push_back(date);
		}
		std::sort(m_dates.begin(), m_dates.end());
		infile.close();
		
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
			size_t date_idx = std::find(m_dates.begin(),m_dates.end(),date) - m_dates.begin();
			(m_time_series[page])[date_idx] = count;
		}
	}

	/*void serialize_data(const std::string &file_name) const {
		std::ofstream os(file_name, std::ios::binary);                    
		cereal::BinaryOutputArchive output(os);
		output(m_time_series,m_dates);
	}
		
	void load_data(const std::string &file_name) {
		std::ifstream is(file_name, std::ios::binary);
		cereal::BinaryInputArchive input(is);
		input(m_time_series,m_dates);
	}*/

	inline std::vector<uint32_t> range(
		const std::string &page, uint32_t time1, uint32_t time2) const {
		
		const auto lt_idx = std::lower_bound(m_dates.begin(), m_dates.end(), time1);
		const auto rt_idx = std::lower_bound(lt_idx, m_dates.end(), time2);		
		const auto pg = m_time_series.at(page);
		
		const auto lt_end = lt_idx - m_dates.begin();
		const auto rt_end = rt_idx - m_dates.begin();
		
		std::vector<uint32_t> result;
		result.reserve(rt_end - lt_end + 1);
		
		for(size_t idx = lt_end; idx <= rt_end; ++idx)
			result.push_back(pg[idx]);
			
		return result;
	}
	
	inline std::vector<std::pair<uint32_t, uint32_t>> rangeTopK(
		const std::string &page, uint32_t time1, uint32_t time2, uint32_t k) const {
			
		const auto lt_idx = std::lower_bound(m_dates.begin(), m_dates.end(), time1);
		const auto rt_idx = std::lower_bound(lt_idx, m_dates.end(), time2);
		const auto pg = m_time_series.at(page);
		
		const auto lt_end = lt_idx - m_dates.begin();
		const auto rt_end = rt_idx - m_dates.begin();
		
		std::priority_queue<std::pair<uint32_t, uint32_t>,
							std::vector<std::pair<uint32_t, uint32_t>>,
							[](const std::pair<uint32_t, uint32_t> &p1, const std::pair<uint32_t, uint32_t> &p2) {
								return p1.second > p2.second;
							}> heap;
		
		for(size_t idx = lt_end; idx <= rt_end; ++idx) {
			if(heap.size() < k)
				heap.emplace(m_dates[idx], pg[idx]);
			else if(heap.top().second < pg[idx]) {
				heap.emplace(m_dates[idx], pg[idx]);
				heap.pop();
			}
		}
		
		std::vector<std::pair<uint32_t, uint32_t>> result;
		result.reserve(heap.size());
		
		for(size_t idx = 0; idx < heap.size(); ++idx) {
			result.push_back(heap.top());
			heap.pop();
		}
		
		return result;
	}
	
	size_t size() const {
		std::vector<std::pair<std::string, std::vector<uint32_t>>> values;
		
		for(auto idx = m_time_series.begin(); idx != m_time_series.end(); ++idx) {
			values.emplace_back(idx->first, idx->second);
		}
		
		size_t result = sizeof(std::string) * sizeof(std::vector<uint32_t>) * values.size();  
		return result;
	}

	void print(void) const {
		for(auto const &elem : m_time_series) {
			std::cout << elem.first << std::endl;
			for(auto const visit : elem.second) {
				size_t date_idx = std::find((elem.second).begin(),(elem.second).end(),visit) - (elem.second).begin();
				std::cout << "\t" << m_dates[date_idx] << " " << visit << std::endl;
			}
		}
	}
};
