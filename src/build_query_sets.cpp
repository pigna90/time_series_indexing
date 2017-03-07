#pragma once

#include "Baseline.hpp"
#include "Index1.hpp"

#include <iostream>
#include <fstream>

struct query{
    std::string page;
    uint32_t date_begin;
    uint32_t date_end;
    uint32_t k=0;
};

std::vector<query> range_queries (
    uint32_t n, uint32_t date_range, std::vector<std::string> pages,std::vector<uint32_t> dates){
    std::vector<query> queries(n);
    for(size_t i=0; i<n; ++i){
        query q;
        q.page = pages[rand() % (pages.size() - 1) + 0];
        q.date_begin = dates[rand() % ((dates.size()-date_range) - 1) + 0];
        q.date_end = dates[rand() % (dates.size() - (dates.size()-date_range)+1) + (dates.size()-date_range-1)];
        queries[i] = q;
    }
    return queries;
}

int main(int argc, char const *argv[]) {
    if(argc == 3){
        std::string ds_filename = argv[1];
        uint32_t num_of_query = atoi(argv[2]);
        Baseline b;
        b.load_data(ds_filename + ".0");
        std::vector<std::string> pages = b.get_pages();
        std::vector<uint32_t> dates = b.get_dates();
        std::sort(dates.begin(), dates.end());
        auto k = 3;
        for(double p=0.10; p<=0.80; p=p+0.30){
            uint32_t range = p*dates.size();
            std::vector<query> queries = range_queries(num_of_query, range, pages, dates);
            std::ofstream outfile;
            outfile.open(ds_filename + ".queries." + std::to_string((int)(p*100)),std::ios::out);
            for(auto e: queries){
                outfile << e.page << "\t" << e.date_begin << "\t" << e.date_end << "\t" << k << "\n";
            }
            k += 3;
        }
    }
    else
		std::cout << "Missing parameters" << std::endl << "Exit." << std::endl;
    return 0;
}
