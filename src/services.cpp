#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*Read all dates from dataset and return max/min date
 * *
 * dataset: name of dataset file
 * *
 */
pair<int, int> getMaxMinDate(const string &dataset){
	vector<int> v;
	ifstream infile(dataset);
	for(string line; getline(infile, line);){
		string token = line.substr(0, line.find("\t"));
		string token_1 = token.substr(0, line.find("-")) + token.substr(line.find("-")+1, token.length()-1);
		v.push_back(atoi(token_1.c_str()));
	}
	int maxDate = *max_element(v.begin(), v.end());
	int minDate = *min_element(v.begin(), v.end());
	
	return make_pair(minDate, maxDate);
}

int main(void){
	
}
