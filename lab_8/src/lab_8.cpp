#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

int main(){
    uint64_t n, p, m;
    cin>>n>>p>>m;
    uint64_t cur_sum=m;
    vector<uint64_t> v(n);
    vector<uint64_t> cnt(n);
    v[0]=1;
    for(uint64_t i=1; i<n;++i){
        v[i]= v[i-1]*p;
    }

    for(uint64_t i=n-1; i>=0;--i){
        uint64_t cnt_i = cur_sum/v[i];
        cnt[i] = cnt_i;
        cur_sum = cur_sum - v[i]*cnt_i;
        if(i==0 or cur_sum==0) break;
    }

    for(uint64_t i=0; i<n;++i){
        cout<<cnt[i]<<'\n';
    }

}
