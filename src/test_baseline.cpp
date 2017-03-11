#include "Baseline.hpp"
#include "Index1.hpp"
#include "test_common.hpp"

#include <iostream>
#include <gtest/gtest.h>

Baseline b;

std::vector<range_case> cases;

TEST(Range, Baseline){
	for(auto const &c : cases){
		std::vector<uint32_t> res = b.range(c.page, c.date_begin, c.date_end);

		ASSERT_EQ(res.size(), c.results.size());

		for(size_t idx=0; idx<res.size(); ++idx){
			EXPECT_EQ(res[idx], c.results[idx]);
		}
	}
}

int main(int argc, char** argv) {
	if(argc == 3){
		testing::InitGoogleTest(&argc, argv);

        std::string ds_filename = argv[1];		//Dataset filename
        std::string test_filename = argv[2];	//Queries and results filename
        cases = load_range_cases(test_filename);

		b = Baseline(ds_filename);

        return RUN_ALL_TESTS();
	}
	else
		std::cout << "Missing parameters" << std::endl << "Exit." << std::endl;
    return 0;
}
