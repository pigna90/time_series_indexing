#pragma once

#include <sdsl/sd_vector.hpp>
#include <sdsl/rmq_support.hpp>
#include <map>
#include <unordered_map>
#include <queue>

class Index1{
private:
	//Elias-Fano rappresentation of pages visits
	sdsl::sd_vector<> m_visits_ef;
	//Support structure for m_visits_ef
	sdsl::select_support_sd<1> m_visits_supp;
	//Pages start index
	std::unordered_map<std::string, size_t> m_pages;
	//Range maximum query on m_visits
	sdsl::rmq_succinct_sct<0> m_rmq;
	//Map of dates for lookup
	std::unordered_map<uint32_t, size_t> m_map_dates; //Succinct? //Unordered_map ?
	//Vector of dates
	std::vector<uint32_t> m_dates;
public:
	Index1() {}
	~Index1() {}

	/* Reads time series from file and populate data structures
	* *
	* dataset_name: dataset file name
	*/
	Index1(const std::string &dataset_name){
		std::vector<std::string> lines;
		std::ifstream infile(dataset_name);
		std::string token;

		//Reads all dates available on data
		size_t page_id = 0;
		for(std::string line; getline(infile, line); ) {
			lines.push_back(line);
			//Parse timestamp
			token = line.substr(0, line.find("\t"));
			uint32_t date = atoi((token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1)).c_str());
			//Parse page name
			token = line.substr(line.find("\t")+1, line.length());
			std::string page = token.substr(0, token.find("\t"));

			m_dates.push_back(date);

			if (m_pages.find(page) == m_pages.end()) {
				m_pages.insert(make_pair(page, page_id));
				page_id++;
			}
		}
		infile.close();

		//Sorting and erasing duplicates from dates vector
		std::sort(m_dates.begin(), m_dates.end());
		m_dates.erase( unique( m_dates.begin(), m_dates.end() ), m_dates.end());

		for(auto e : m_dates)
			std::cout << e << std::endl;

		//HashTable for retrieve lookup index position instead
		//of find it from vector
		size_t date_id = 0;
		for(auto d: m_dates){
			m_map_dates.insert(std::make_pair(d, date_id));
			date_id++;
		}

		std::vector<uint64_t> visits(m_dates.size()*m_pages.size(), 0);

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

			visits[(m_dates.size()*m_pages.at(page))+m_map_dates.at(date)] = count;
		}

		//Build structure for Range Maximum Query
		m_rmq = sdsl::rmq_succinct_sct<0>(&visits);

		//Prefix sum
		uint64_t sum = 0;
		for(size_t idx=0; idx<visits.size(); ++idx){
			sum += visits[idx];
			visits[idx] = sum;
		}

		//Build Elias-Fano data rappresentation
		m_visits_ef = sdsl::sd_vector<> (visits.begin(), visits.end());

		//Support structure for m_visits_ef
		m_visits_supp = sdsl::select_support_sd<1> (&m_visits_ef);
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

		const auto lt_end = (m_dates.size()*m_pages.at(page))+m_map_dates.at(time1)+1;
		const auto rt_end = (m_dates.size()*m_pages.at(page))+m_map_dates.at(time2)+1;

		std::vector<uint32_t> result;
		result.reserve(rt_end - lt_end + 1);

		uint64_t sum = lt_end==1 ? 0 : m_visits_supp(lt_end-1);

		for(size_t idx=lt_end; idx<=rt_end; ++idx){
			result.push_back(m_visits_supp(idx) - sum);
			sum = m_visits_supp(idx);
		}

		return result;
	}

	/* Return a vector with all the top K counters of page
	* in the time interval [time1, time2] in tuple version
	* *
	* page: string of the selected webpage
	* time1: start of the selected range
	* time2: finish of the selected range
	* k: number of counters required
	*/
	inline std::vector<std::pair<uint32_t, uint32_t>> rangeTopK(
		const std::string &page, uint32_t time1, uint32_t time2, uint32_t k) const {

		struct weight_interval{
			uint64_t w;
			size_t idx, lb, rb;
			weight_interval(uint64_t f_w, size_t f_idx, size_t f_lb, size_t f_rb) :
				w(f_w), idx(f_idx), lb(f_lb), rb(f_rb) {}

			bool operator<(const weight_interval& wi) const {
				return std::tie(w, idx, lb, rb) < std::tie(wi.w, wi.idx, wi.lb, wi.rb);
			}
		};

		const auto lt_end = (m_dates.size()*m_pages.at(page))+m_map_dates.at(time1)+1;
		const auto rt_end = (m_dates.size()*m_pages.at(page))+m_map_dates.at(time2)+1;

		std::priority_queue<weight_interval> pq;
		auto push_interval = [&](size_t f_lb, size_t f_rb) {
			if ( f_rb > f_lb ) {
				size_t max_idx = rmq(f_lb, f_rb);
				pq.push(weight_interval(w[max_idx], max_idx, f_lb, f_rb));
			}
		};

		vector<size_t> res;
		push_interval(r[0], r[1]);
		while ( res.size() < k and !pq.empty() ) {
			auto iv = pq.top(); pq.pop();
			res.push_back(iv.idx);
			push_interval(iv.lb, iv.idx-1);
			push_interval(iv.idx+1, iv.rb);
		}
		//IV sono
	}
};
