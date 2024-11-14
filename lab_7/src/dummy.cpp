#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdint>
using namespace std;

// uint64_t min_cost(uint64_t n){
//     if(n==1) return 0;
//     // vector<uint64_t> costs(3,0);
//     uint64_t cost;
//     if(n-1>=1){
//         cost = n + min_cost(n-1);
//         if(n%2==0) cost = min(cost, n+min_cost(n/2));
//         if(n%3==0) cost = min(cost, n+min_cost(n/3));
//     }

//     return cost;
// }

pair<uint64_t, vector<uint>> min_cost(uint64_t n){
    vector<uint> v;
    if(n==1) return pair(0, v);
    uint act;
    uint64_t cost;
    pair<uint64_t, vector<uint>> tmp;
    if(n-1>=1){
        tmp = min_cost(n-1);
        cost = n + tmp.first;
        act =1;
        v = tmp.second;
        if(n%2==0){
            tmp = min_cost(n/2);
            if (n+tmp.first< cost){
                cost = tmp.first+n;
                act = 2;
                v = tmp.second;
            }
        }
        if(n%3==0){
            tmp = min_cost(n/3);
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

int main(){
    uint64_t n;
    cin>>n;
    pair<uint64_t, vector<uint>> p = min_cost(n);
    cout<<p.first<<"\n";
    for(uint64_t i=p.second.size()-1; i>=0;--i){
        if (p.second[i] ==1){
            cout<<"-1 ";
            
        } else if(p.second[i]==2){
            cout<<"/2 ";
        } else{
            cout<<"/3 ";
        }
        if(i==0) break;
    }
}