#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <queue>
using namespace std;

const string DURATION_PREFIX = "s";

void greedy(uint64_t m, vector<uint64_t> &v){
    uint64_t cur_sum=m;
    uint64_t n = v.size();
    vector<uint64_t> cnt(n);

    for(uint64_t i=n-1; i>=0;--i){
        uint64_t cnt_i = cur_sum/v[i];
        cnt[i] = cnt_i;
        cur_sum = cur_sum - v[i]*cnt_i;
        if(i==0 or cur_sum==0) break;
    }
    return;
}

void dp_algorithm(uint64_t m, vector<uint64_t> &v){
    uint64_t n = v.size();
    vector<uint64_t> cnt(n);
    vector<uint64_t> dp(m+1, 0);
    vector<int64_t> cur_cnt(m+1,-1);
    cur_cnt[0]=0;
    for(uint64_t i_coin=0; i_coin<n;++i_coin ){
        for(uint64_t j=0; j<m+1;++j){
            if (cur_cnt[j]!=-1){
                uint64_t new_sum = j+v[i_coin];
                uint64_t new_cnt = cur_cnt[j]+1;
                if (new_sum > m) break;
                if(cur_cnt[new_sum]==-1 or cur_cnt[new_sum]>new_cnt){
                    cur_cnt[new_sum] = new_cnt;
                }
            }
        }
    }

    return;
}


int main(){
    uint64_t n, p, m;
    cin>>n>>p>>m;
    vector<uint64_t> denomination(n);
    denomination[0] =1;
    for(uint64_t i=1; i<n;++i){
        denomination[i]= denomination[i-1]*p;
    }

   
    auto begin_time=chrono::high_resolution_clock::now();
    greedy(m, denomination);
    auto end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> dummy_time=end_time - begin_time;

    begin_time=chrono::high_resolution_clock::now();
    dp_algorithm(m,denomination);
    end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> stl_sort_time=end_time - begin_time;
    cout <<fixed;
    cout.precision(9);
    // cout<<"Array size = "<<counter<<'\n';
    cout<<"Greedy algorithm: "<< dummy_time.count()<<" "<<DURATION_PREFIX<<'\n';
    cout<<"DP algorithm    : "<< stl_sort_time.count()<<" "<<DURATION_PREFIX<<'\n';
}