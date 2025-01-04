#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <queue>
#include <algorithm>
#include <float.h>
#include <numbers>
#include <cmath>
using namespace std;

const string DURATION_PREFIX = "s";
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

vector<pair<int,int>> coord;
vector<vector<my_pair>> graph;


double dijkstra(uint idx_from, uint target){
    uint n = graph.size();
    // vector<int> used(n, 0);
    vector<double> dist(n, -1);
    priority_queue<my_pair, vector<my_pair>, compression_class> PQ;
    dist[idx_from] = 0;

    PQ.push(my_pair(0,idx_from));
    while(!PQ.empty()){
        my_pair cur_val = PQ.top();
        // if (cur_val.idx == target) break;
        PQ.pop();

        if(dist[cur_val.idx] > 0 and cur_val.dist > dist[cur_val.idx]) continue;

        for(int j=0; j< graph[cur_val.idx].size();++j){
            uint next_val = graph[cur_val.idx][j].idx;
            double next_path = dist[cur_val.idx] + graph[cur_val.idx][j].dist;
            if (dist[next_val] > 0 and next_path > dist[next_val]) continue;

            if (dist[next_val] < 0 or next_path < dist[next_val] ){
                dist[next_val] = next_path;
                PQ.push(my_pair(dist[next_val], next_val));
            }
        }
    }
    return dist[target];



}

double a_star(uint idx_from, uint finish){
    uint n = graph.size();
    priority_queue<my_pair, vector<my_pair>, compression_class> PQ;
    vector<double> path(n+1, -1);

    path[idx_from] = 0;

    PQ.push(my_pair(0,idx_from));
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
    return path[finish];
}


int main(){
    uint n, m;
    cin>>n>>m;
    coord.resize(n+1);
    graph.resize(n+1);

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
        uint start,target_idx;
        cin>>start>>target_idx;

        auto begin_time=chrono::high_resolution_clock::now();
        dijkstra(start, target_idx);
        auto end_time=chrono::high_resolution_clock::now();
        chrono::duration<double> dummy_time=end_time - begin_time;

        begin_time=chrono::high_resolution_clock::now();
        double answ = a_star(start, target_idx);
        end_time=chrono::high_resolution_clock::now();
        chrono::duration<double> stl_sort_time=end_time - begin_time;
        cout <<fixed;
        cout.precision(9);
        // cout<<"Array size = "<<counter<<'\n';
        cout<<"Dijkstra : "<< dummy_time.count()<<" "<<DURATION_PREFIX<<'\n';
        cout<<"A*       : "<< stl_sort_time.count()<<" "<<DURATION_PREFIX<<'\n';
        cout<<"Answ = "<<answ<<'\n';
    }
}