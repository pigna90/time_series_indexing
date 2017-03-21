#pragma once

#include "Baseline.hpp"
#include "Index1.hpp"

#include <iostream>
#include <fstream>

struct query{
    std::string page;
    uint32_t date_begin;
    uint32_t date_end;
};

std::vector<query> range_queries (
    uint32_t n, uint32_t date_range, std::vector<std::string> pages,std::vector<uint32_t> dates){
    std::vector<query> queries(n);
    for(size_t i=0; i<n; ++i){
        query q;
        q.page = pages[rand() % (pages.size() - 1) + 0];
		size_t l_idx = rand() % ((dates.size() - 1 - date_range) + 1) + 0;
		size_t r_idx = l_idx + date_range;
		q.date_begin = dates[l_idx];
		q.date_end = dates[r_idx];
        queries[i] = q;
    }
    return queries;
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));
    if(argc == 5){
        std::string ds_filename = argv[1];		//Dataset file name
        uint32_t num_of_query = atoi(argv[2]);	//Number of queries
        uint16_t range_perc = atoi(argv[3]);	//Minimum date intervall
        uint16_t max_k = atoi(argv[4]);			//Max number of k

        Baseline b;
		b.load_data(ds_filename + ".0");
        std::vector<std::string> pages = b.get_pages();
        std::vector<uint32_t> dates = b.get_dates();

        for(double p=range_perc; p<=100; p=p+range_perc){
            uint32_t range = (p/100)*(dates.size()-1);
            std::vector<query> queries = range_queries(num_of_query, range, pages, dates);
            for(size_t k=1; k<=max_k; ++k){
				std::ofstream outfile;
				outfile.open(ds_filename + ".queries."
					+ std::to_string((int)p) + "-" + std::to_string(k) + ".txt"
					,std::ios::out);

				for(auto e: queries)
					outfile << e.page << "\t" << e.date_begin << "\t" << e.date_end << "\t" << k << "\n";
			}
        }
    }
    else
		std::cout << "Missing parameters" << std::endl << "Exit." << std::endl;
    return 0;
}
