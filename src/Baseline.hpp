#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <cereal/archives/binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

class Baseline {
private:
	std::map<std::string, std::vector<uint32_t>> m_time_series;
	std::vector<uint16_t> m_dates;

	template <class Archive>
	void TimeSeries::serialize(Archive &archive) {
		archive(m_time_series);
	}

public:
	Baseline() {}
	~Baseline() {}
	Baseline(const std::string &dataset_name) {
		/*Il costruttore con un parametro assume le caratteristiche
		 * della funzione populateDataStructure precedentemente
		 * implementata, di conseguenza andrà rimossa dalla classe.*/
		 /*Valutare se inserire in questo posto anche la funzione
		  * parseLine, chiamata da populateDataStructure*/
	}

	void serialize_data(const string &file_name) const {
		std::ofstream os(file_name, std::ios::binary);                    
		cereal::BinaryOutputArchive output(os);
		output(m_time_series);
	}
		
	void load_data(const string &file_name) {
		std::ifstream is(file_name, std::ios::binary);
		cereal::BinaryInputArchive input(is);
		input(m_time_series);
	}

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

	inline std::vector<std::pair<uint16_t, uint32_t>> rangeTopK(
		const std::string &page, uint32_t time1, uint32_t time2, uint32_t k) const {
			
		const auto lt_idx = std::lower_bound(m_dates.begin(), m_dates.end(), time1);
		const auto rt_idx = std::lower_bound(lt_idx, m_dates.end(), time2);
		const auto pg = m_time_series.at(page);
		
		const auto lt_end = lt_idx - m_dates.begin();
		const auto rt_end = rt_idx - m_dates.begin();
		
		std::priority_queue<std::pair<uint16_t, uint32_t>> heap;
		
		for(size_t idx = lt_end; idx <= rt_end; ++idx) {
			if(heap.size() < k)
				heap.emplace(m_dates[idx], pg[idx]);
			else if(heap.top().second < pg[idx]) {
				heap.emplace(m_dates[idx], pg[idx]);
				heap.pop();
			}
		}
		
		std::vector<std::pair<uint16_t, uint32_t>> result;
		result.reserve(heap.size());
		
		for(size_t idx = 0; idx < heap.size(); ++idx) {
			result.push_back(heap.top());
			heap.pop();
		}
		
		return result;
	}
	
	size_t size() const {
		std::vector<std::vector<uint32_t>> data;
		std::copy(m_time_series.begin(), m_time_series.end(), data.begin());
		
		size_t result = sizeof(std::vector<uint32_t>) * data.size();  
		return result;
	}
};
