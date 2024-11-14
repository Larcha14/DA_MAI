#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <queue>
using namespace std;

const string DURATION_PREFIX = "s";

pair<uint64_t, vector<uint>> dummy_min_cost(uint64_t n){
    vector<uint> v;
    if(n==1) return pair(0, v);
    uint act;
    uint64_t cost;
    pair<uint64_t, vector<uint>> tmp;
    if(n-1>=1){
        tmp = dummy_min_cost(n-1);
        cost = n + tmp.first;
        act =1;
        v = tmp.second;
        if(n%2==0){
            tmp = dummy_min_cost(n/2);
            if (n+tmp.first< cost){
                cost = tmp.first+n;
                act = 2;
                v = tmp.second;
            }
        }
        if(n%3==0){
            tmp = dummy_min_cost(n/3);
            if (n+tmp.first< cost){
                cost = tmp.first+n;
                act = 3;
                v = tmp.second;
            }
        }
        v.push_back(act);
    }

    return pair(cost, v);
}

vector<pair<uint64_t, uint64_t>> my_cost( uint64_t n){
    vector<pair<uint64_t, uint64_t>> v(n+1); // <cmd, prev_sum>
    queue<uint64_t> q;

    q.push(n);
    while(q.size()>0){
        uint64_t a = q.front();
        q.pop();
        
        uint64_t s = v[a].second +a;
        vector<uint64_t> steps(3);
        steps[0] = a-1;
        if (a%2==0) steps[1] = a/2;
        if (a%3==0) steps[2] = a/3;
        for(int i=0; i<3;++i){
            if (steps[i]>0){
                if (v[steps[i]].second ==0){
                    v[steps[i]]=make_pair(i+1, s);
                    q.push(steps[i]);
                } else if (v[steps[i]].second > s){
                    v[steps[i]] = make_pair(i+1, s);
                    q.push(steps[i]);
                }
            }
        }
        
    }
    return v;
}

int main(){
    uint64_t n;
    cin>>n;
   
    auto begin_time=chrono::high_resolution_clock::now();
    dummy_min_cost(n);
    auto end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> dummy_time=end_time - begin_time;

    begin_time=chrono::high_resolution_clock::now();
    my_cost(n);
    end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> stl_sort_time=end_time - begin_time;
    cout <<fixed;
    cout.precision(9);
    // cout<<"Array size = "<<counter<<'\n';
    cout<<"Dummy algorithm : "<< dummy_time.count()<<" "<<DURATION_PREFIX<<'\n';
    cout<<"My algorithm : "<< stl_sort_time.count()<<" "<<DURATION_PREFIX<<'\n';
}