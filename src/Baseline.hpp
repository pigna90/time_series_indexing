#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <fstream>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>

class Baseline {
private:
	std::map<std::string, std::vector<uint32_t>> m_time_series;
	std::set<uint32_t> m_dates;

	template <class Archive>
	void serialize(Archive &archive) {
		archive(m_time_series);
	}

	//tuple<uint32_t, string, uint32_t> TimeSeries::parseLine(const string &line) const {
		//int date, count;
		//string page, token;

		////Parse visit date
		//token = line.substr(0, line.find("\t"));
		//date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
		////Parse page name
		//token = line.substr(line.find("\t")+1, line.length());
		//page = token.substr(0, token.find("\t"));
		////Parse page counter
		//count = atoi((token.substr(token.find("\t")+1, token.length())).c_str());

		//return make_tuple(date, page, count);
	//}
	
public:
	Baseline() {}
	
	~Baseline() {}
	
	Baseline(const std::string &dataset_name) {
		/*Il costruttore con un parametro assume le caratteristiche
		 * della funzione populateDataStructure precedentemente
		 * implementata, di conseguenza andrà rimossa dalla classe.*/
		 /*Valutare se inserire in questo posto anche la funzione
		  * parseLine, chiamata da populateDataStructure*/
		std::set<std::string> lines;
		std::ifstream infile(dataset_name);
		std::string token;
		
		for(std::string line; getline(infile, line); ) {
			lines.insert(line);
			token = line.substr(0, line.find("\t"));
			uint32_t date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
			m_dates.insert(date);
		}
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
			//std::cout << page << " " << date << " " << count << std::endl;
			if (m_time_series.find(page) == m_time_series.end()) {
				std::vector<uint32_t> visits(m_dates.size(),0);
				m_time_series.insert(make_pair(page,visits));
			}
			size_t dateIndex = std::distance(m_dates.begin(),m_dates.find(date));
			std::cout << dateIndex << std::endl;
			(m_time_series[page])[dateIndex] = count;
		}
	}

	void serialize_data(const std::string &file_name) const {
		std::ofstream os(file_name, std::ios::binary);                    
		cereal::BinaryOutputArchive output(os); // stream to cout
		output(m_time_series);
	}
		
	void load_data(const std::string &file_name) {
		std::ifstream is(file_name, std::ios::binary);
		cereal::BinaryInputArchive input(is);
		input(m_time_series);
	}

	inline std::vector<uint32_t> range(
										const std::string &page,
										uint32_t time1,
										uint32_t time2) const {
		
		
	}

	inline std::vector<std::pair<uint32_t, uint32_t>> rangeTopK(
										const std::string &page,
										uint32_t time1,
										uint32_t time2,
										uint32_t k) const {
		
	}

	uint32_t size() const {
		/*Possibile realizzazione: inserire la mappa all'interno di
		 * un std::vector<tipodellamappa> e successivamente calcolare la dimensione
		 * occupata come:
		 * size_t result = sizeof(tipodellamappa) * vettore.size()*/
	}
};
