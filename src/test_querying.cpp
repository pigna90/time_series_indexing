#include "../include/header.hpp"
 
int main(int argc, char* argv[]){
	if(argc == 2){
		string in_data_bin = argv[1];
		d_structure s = load_data(in_data_bin);

		/*
		 * Example of using range function
		 */
		 
		string page = "2016_Formula_One_season";
		int time1 = 2016050100;
		int time2 = 2016050108;

		vector<int> finding = range_tuple(s, page, time1, time2);
		for (int i = 0; i < finding.size(); ++i) {
			cout << finding[i] << endl;
		}

		/*
		 * Example of using top_k_range_tuple function
		 */

		int k = 3;
		vector<tuple<int,int>> looking_tuple = top_k_range_tuple(s, page, time1, time2, k);
		for (int i = 0; i < looking_tuple.size(); ++i) {
			cout << get<0>(looking_tuple[i]) << "-" << get<1>(looking_tuple[i]) << endl;
		}

		return 0;
	}
	else
		cout << "Missing parameters" << endl << "Exit." << endl;
}

