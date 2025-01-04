#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <vector>
#include <queue>
#include <cstring>
#include <exception>
#include <algorithm>
#include <cmath>
using namespace std;

struct my_pair{
    double dist;
    uint idx;

    my_pair(){
        dist = -1;
        idx =0;
    }
    my_pair(double new_dist, uint new_idx){
        dist = new_dist;
        idx = new_idx;
    }
    bool operator() ( my_pair el1, my_pair el2){
        return el1.dist > el2.dist;
    }
};

class compression_class {
public:
    bool operator() (my_pair el1, my_pair el2) {
        return el1.dist > el2.dist;
    }
};

double dist_calc(pair<int, int> c1, pair<int,int> c2){ //heuristic approximation
    return pow(pow((c2.first - c1.first),2) + pow((c2.second - c1.second),2), 0.5);
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    uint n, m;
    cin>>n>>m;
    vector<pair<int,int>> coord(n+1);
    vector<vector<my_pair>> graph(n+1);
    for(uint i=1;i<=n;++i){
        cin>>coord[i].first>>coord[i].second;
    }
    for(uint i=0; i<m;++i){
        uint v1,v2;
        cin>>v1>>v2;
        double dist = dist_calc(coord[v1], coord[v2]);
        graph[v1].push_back(my_pair(dist, v2));
        graph[v2].push_back(my_pair(dist, v1));
    }

    uint q;
    cin>>q;
    for(int i=0; i<q;++i){
        priority_queue<my_pair, vector<my_pair>, compression_class> PQ;
        vector<double> path(n+1, -1);
        uint start,finish;
        cin>>start>>finish;

        path[start] = 0;

        PQ.push(my_pair(0,start));
        while(!PQ.empty()){
            my_pair cur_val = PQ.top();
            if (cur_val.idx == finish) break;
            PQ.pop();
            
            for(int j=0; j< graph[cur_val.idx].size();++j){
                uint next_val = graph[cur_val.idx][j].idx;
                double next_path = path[cur_val.idx] + graph[cur_val.idx][j].dist;
                if (path[next_val] > 0 and next_path > path[next_val] ) continue;

                if (path[next_val] < 0 or next_path < path[next_val] ){
                    path[next_val] = next_path;
                    double evristic_dist = next_path + dist_calc(coord[next_val], coord[finish]);
                    PQ.push(my_pair(evristic_dist, next_val));
                }
            }
        }
        if (path[finish] < 0) {
            cout<<-1<<'\n';
        } else{
            cout<< fixed <<setprecision(7)<<path[finish]<<'\n';
        }
    }
}  