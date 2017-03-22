#include "Index1.hpp"
#include "test_common.hpp"

#include <iostream>
#include <gtest/gtest.h>

#define DS_FN "dataset_1.txt"
#define RANGE_FN "range_cases_1.txt"
#define TOPK_FN "topk_cases_1.txt"

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
	testing::InitGoogleTest(&argc, argv);

	std::string ds_filename = DS_FN;		//Dataset filename
	std::string range_filename = RANGE_FN;	//Range queries and results filename
	std::string topk_filename = TOPK_FN;	//Topk queries and results filename

	range_cases = load_range_cases(range_filename);
	topk_cases = load_topk_cases(topk_filename);

	i = new Index1(ds_filename);

	return RUN_ALL_TESTS();
}
