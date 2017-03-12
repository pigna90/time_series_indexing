#include "Index1.hpp"
#include "test_common.hpp"

#include <iostream>
#include <gtest/gtest.h>

Index1 *i;

std::vector<range_case> range_cases;
std::vector<topk_case> topk_cases;

/*Baseline Range unit test*/
TEST(Index1, Range){
	for(auto const &c : range_cases){
		std::vector<uint32_t> res = i->range(c.page, c.date_begin, c.date_end);

		ASSERT_EQ(res.size(), c.results.size());

		for(size_t idx=0; idx<res.size(); ++idx){
			EXPECT_EQ(res[idx], c.results[idx]);
		}
	}
}

/*Baseline Topk unit test*/
TEST(Index1, TopK){
	for(auto const &c : topk_cases){
		std::vector<std::pair<uint32_t, uint32_t>> res = i->rangeTopK(c.page, c.date_begin, c.date_end, c.k);

		ASSERT_EQ(res.size(), c.results.size());

		for(size_t idx=0; idx<res.size(); ++idx){
			EXPECT_EQ(res[idx].first, c.results[idx].first);
			EXPECT_EQ(res[idx].second, c.results[idx].second);
		}
	}
}

int main(int argc, char** argv) {
	if(argc == 4){
		testing::InitGoogleTest(&argc, argv);

		std::string ds_filename = argv[1];		//Dataset filename
		std::string range_filename = argv[2];	//Range queries and results filename
		std::string topk_filename = argv[3];	//Topk queries and results filename

		range_cases = load_range_cases(range_filename);
        topk_cases = load_topk_cases(topk_filename);

		i = new Index1(ds_filename);


		return RUN_ALL_TESTS();
	}
	else
		std::cout << "Missing parameters" << std::endl << "Exit." << std::endl;
    return 0;
}
