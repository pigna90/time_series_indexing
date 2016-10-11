#include "header.hpp"
 
int main(){
	d_structure s = populateDataStructure(SMALL_DATA);

    /*
    for(auto const &elem : s) {
        cout << elem.first << endl;
        for (auto const p : elem.second) {
            cout << "\t" << p.first << " " << p.second << endl;
        }
    }
    */

    /*
     * Example of using range function
     */

    string page = "2016_Formula_One_season";
    int time1 = 2016050100;
    int time2 = 2016050108;

    //vector<int> finding = range(s, page, time1, time2);
    //for (int i = 0; i < finding.size(); ++i) {
        //cout << finding[i] << endl;
    //}

    /*
     * Example of using top_k_range function
     */

    //int k = 3;
    //vector<pair<int,int>> looking = top_k_range(s, page, time1, time2, k);
    //for (int i = 0; i < looking.size(); ++i) {
        //cout << looking[i].first << "-" << looking[i].second << endl;
    //}

    /*
     * Example of using top_k_range_tuple function
     */

    int g = 3;
    vector<tuple<int,int>> looking_tuple = top_k_range_tuple(s, page, time1, time2, g);
    for (int i = 0; i < looking_tuple.size(); ++i) {
        cout << get<0>(looking_tuple[i]) << "-" << get<1>(looking_tuple[i]) << endl;
    }

    return 0;
}

