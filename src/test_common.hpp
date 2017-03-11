#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct range_case{
    std::string page;
    uint32_t date_begin;
    uint32_t date_end;

    std::vector<uint32_t> results;
};

std::vector<range_case> load_range_cases(const std::string cases_filename){
    std::ifstream infile(cases_filename);
    std::string token;

	std::vector<range_case> cases_out;

    for(std::string line; getline(infile, line); ) {
		if(line.find("#") == std::string::npos){
			range_case c;
			c.page = line.substr(0, line.find("\t"));
			line = line.substr(line.find("\t")+1, line.size());
			c.date_begin = stoi(line.substr(0, line.find("\t")));
			line = line.substr(line.find("\t")+1, line.size());
			c.date_end = stoi(line.substr(0, line.find("/")));
			line = line.substr(line.find("/")+1, line.size());

			uint32_t val;
			while(line.find("\t") != std::string::npos){
				val = stoi(line.substr(0, line.find("\t")));
				line = line.substr(line.find("\t")+1, line.size());
				c.results.push_back(val);
			}
			val = stoi(line.substr(0, line.find("\n")));
			c.results.push_back(val);

			cases_out.push_back(c);
		}
    }
    return cases_out;
}
