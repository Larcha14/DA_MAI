#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdint>
using namespace std;

int main(){
    uint64_t n;
    cin>>n;
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

    cout<<v[1].second<<'\n';
    stack<uint64_t> answ;
    answ.push(v[1].first);
    uint64_t cur_s = 1;
    while(true){
        uint64_t step = answ.top();
        if (step ==1){
            cur_s++;
            
        } else if(step==2){
            cur_s =cur_s*2;
        } else{
            cur_s =cur_s*3;
        }
        if (cur_s==n) break;
        answ.push(v[cur_s].first);

    }
    while(!answ.empty()){
        uint64_t a = answ.top();
        answ.pop();
        if(a ==1){
            cout<<"-1 ";
        } else {
            cout<<"/"<<a<<" ";
        }
    }

}