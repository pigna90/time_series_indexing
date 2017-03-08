#include "Baseline.hpp"
#include "Index1.hpp"

#include <iostream>
#include <chrono>
#include <fstream>


struct query{
    std::string page;
    uint32_t date_begin;
    uint32_t date_end;
    uint32_t k=0;
};

std::vector<query> load_queries(const std::string qs_filename){
    std::vector<query> queries;
    std::ifstream infile(qs_filename);
    std::string token;

    for(std::string line; getline(infile, line); ) {
        query q;
        q.page = line.substr(0, line.find("\t"));
        line = line.substr(line.find("\t")+1, line.size());
        q.date_begin = stoi(line.substr(0, line.find("\t")));
        line = line.substr(line.find("\t")+1, line.size());
        q.date_end = stoi(line.substr(0, line.find("\t")));
        line = line.substr(line.find("\t")+1, line.size());
        q.k = stoi(line.substr(0, line.find("\n")));
        queries.push_back(q);
    }
    return queries;
}

template <typename T>
void range_fun(T const b, std::vector<query> const queries){
	auto begin_range = std::chrono::high_resolution_clock::now();
	for(query q: queries)
		b.range(q.page, q.date_begin, q.date_end);
	auto end_range = std::chrono::high_resolution_clock::now();
	std::cout << "Range execution time: ";
	std::cout <<std::chrono::duration_cast<std::chrono::nanoseconds>(end_range-begin_range).count() << " ns";
	std::cout << " ("<<std::chrono::duration_cast<std::chrono::seconds>(end_range-begin_range).count() << " s)" << std::endl;
}

template <typename T>
void topK_fun(T const b, std::vector<query> const queries){
	auto begin_topk = std::chrono::high_resolution_clock::now();
	for(query q: queries)
		b.rangeTopK(q.page, q.date_begin, q.date_end, q.k);
	auto end_topk = std::chrono::high_resolution_clock::now();
	std::cout << "Range topK execution time: ";
	std::cout <<std::chrono::duration_cast<std::chrono::nanoseconds>(end_topk-begin_topk).count() << " ns";
	std::cout << " ("<<std::chrono::duration_cast<std::chrono::seconds>(end_topk-begin_topk).count() << " s)" << std::endl;
}

int main(int argc, char const *argv[]) {
    if(argc == 4){
        char const *id = argv[1];
        std::string ds_filename = argv[2];
        std::string qs_filename = argv[3];
        std::vector<query> queries = load_queries(qs_filename);
        switch (atoi(id)) {
            case 0:{
                Baseline b;
                b.load_data(ds_filename + "." + id);

                range_fun<Baseline> (b, queries);
				topK_fun<Baseline> (b, queries);
			}break;
            case 1:{
                Index1 b(ds_filename);

                range_fun<Index1> (b, queries);
				topK_fun<Index1> (b, queries);
			}break;
        }
	}
	else
		std::cout << "Missing parameters" << std::endl << "Exit." << std::endl;
    return 0;
}
