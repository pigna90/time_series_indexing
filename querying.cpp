#include "header.hpp"

/* Return a vector with all the counters of page in the time interval [time1, time2] in tuple version
 * *
 * s: structure previously populated
 * page: string of the selected webpage
 * time1: start of the selected range
 * time2: finish of the selected range
 * *
 */
vector<int> range_tuple(d_structure s, string page, int time1, int time2) {
    vector<int> result;

    for(auto const e : s[page]) {
        if (get<0>(e) >= time1 && get<0>(e) <= time2) {
            result.emplace_back(get<1>(e));
        }
    }

    return result;
}

/* Comparing class in order to obtain Max Heap in tuple version
 * *
 * operator(): using second-value ordering for object of type std::pair<int,int>
 * *
 */
class Compare_tuple {
public:
    bool operator() (const tuple<int, int>& p1, const tuple<int, int>& p2) {
        return get<1>(p1) < get<1>(p2);
    }
};

/* Return a heap structure with all the top K counters of page in the time interval [time1, time2] in tuple version
* *
* s: structure previously populated
* page: string of the selected webpage
* time1: start of the selected range
* time2: finish of the selected range
* k: number of counters required
* *
*/
vector<tuple<int,int>> top_k_range_tuple(d_structure s, string page, int time1, int time2, int k) {
    priority_queue<tuple<int,int>, vector<tuple<int,int>>, Compare_tuple> top_k(s[page].begin(), s[page].end());

    vector<tuple<int,int>> result(k);

    for(int i = 0; i < k; ++i) {
        if(get<0>(top_k.top()) >= time1 && get<0>(top_k.top()) <= time2) {
            result[i] = top_k.top();
        } else {
            --i;
        }
        top_k.pop();
    }

    return result;
}
