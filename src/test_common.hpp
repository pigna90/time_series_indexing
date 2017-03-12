#include <vector>
#include <string>
#include <fstream>
#include <iostream>

/* Struct for range query/result test case */
struct range_case{
    std::string page;
    uint32_t date_begin;
    uint32_t date_end;

    std::vector<uint32_t> results;
};

/* Struct for topk query/result test case */
struct topk_case{
	std::string page;
    uint32_t date_begin;
    uint32_t date_end;
    uint16_t k;

    std::vector<std::pair<uint32_t, uint32_t>> results;
};

/* Loads range cases from file
 *
 * cases_filename: file name that contains cases for range
 */
std::vector<range_case> load_range_cases(const std::string cases_filename){
    std::ifstream infile(cases_filename);
    std::string token;

	std::vector<range_case> cases;

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

			cases.push_back(c);
		}
    }
    return cases;
}

/* Loads topk cases from file
 *
 * cases_filename: file name that contains cases for topk
 */
std::vector<topk_case> load_topk_cases(const std::string cases_filename){
	std::ifstream infile(cases_filename);
    std::string token;

	std::vector<topk_case> cases;

	for(std::string line; getline(infile, line); ) {
		if(line.find("#") == std::string::npos){
			topk_case c;
			c.page = line.substr(0, line.find("\t"));
			line = line.substr(line.find("\t")+1, line.size());
			c.date_begin = stoi(line.substr(0, line.find("\t")));
			line = line.substr(line.find("\t")+1, line.size());
			c.date_end = stoi(line.substr(0, line.find("\t")));
			line = line.substr(line.find("\t")+1, line.size());
			c.k = stoi(line.substr(0, line.find("/")));
			line = line.substr(line.find("/")+1, line.size());

			uint32_t time,val;
			while(line.find("\t") != std::string::npos){
				time = stoi(line.substr(0, line.find(",")));
				line = line.substr(line.find(",")+1, line.size());
				val = stoi(line.substr(0, line.find("\t")));
				line = line.substr(line.find("\t")+1, line.size());
				c.results.emplace_back(time, val);
			}
			time = stoi(line.substr(0, line.find(",")));
			line = line.substr(line.find(",")+1, line.size());
			val = stoi(line.substr(0, line.find("\n")));
			c.results.emplace_back(time, val);

			cases.push_back(c);
		}
	}
	return cases;
}
