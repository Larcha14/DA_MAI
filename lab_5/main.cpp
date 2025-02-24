#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <map>
#include <cstdint>
// #include <stdio.h>
// #include <cstddef>

using namespace std;

struct edge;

struct node{
    int64_t idx;
    map<char, edge*> edges;
    node *suff_link;
    node(int64_t new_idx) : idx(new_idx){}


};

struct edge{
    int64_t start_idx;
    int64_t *end_idx;
    node *next_node;
    node *last_node;

    edge(int64_t new_start_idx, int64_t *new_end_idx, node *new_last_node, node *new_next_node) : start_idx(new_start_idx), end_idx(new_end_idx), next_node(new_next_node), last_node(new_last_node){}
};

class suff_tree{
    public:
        suff_tree();
        void extension(string s);
        pair<int64_t, vector<int64_t>> LCS(int64_t s1_size);
        void print_tree();
        vector<int64_t> find(string s);


        ~suff_tree();
    private:
        string data;
        node* root;
        int64_t end;
        int64_t cnt_node;

        void del_nodes(node* cur_node);
        void print_node(int64_t level, node *cur_node);
        pair<int64_t, vector<int64_t>> dfs(node* cur_node, int64_t border);
        bool check_suff_belong(node* cur_node, int64_t suff_border);
        void find_dfs(node *cur_node, vector<int64_t> &v);
};

suff_tree::suff_tree(){
    end = 0;
    cnt_node = 0;
    root = new node(0);
    // root->suff_link = root;
}

void suff_tree::print_node(int64_t level, node *cur_node){
    for(auto el : cur_node->edges){
        for(int i=0; i< level;++i){
            cout<<"====|";
        }
        cout<<"("<<el.second->start_idx<<", "<<*el.second->end_idx<<")";
        ino64_t len = *el.second->end_idx - el.second->start_idx;
        cout<<data.substr(el.second->start_idx, *el.second->end_idx - el.second->start_idx)<<"*"<<el.second->next_node->idx<<"*"<<'\n';
        print_node(level+1, el.second->next_node);
    }
}

void suff_tree::print_tree(){
    cout<<"\nSuff tree:\n";
    if (end ==0 ) {
        cout<<"Empty";
    } else{
        cout<<"Root\n";
        print_node(1,root);
    }
    cout<<"\n";
}

void suff_tree::find_dfs(node* cur_node, vector<int64_t> &v){
    if (cur_node->edges.empty()){
        v.push_back(cur_node->idx+1);
        return;
    }
    for(auto el : cur_node->edges){
        find_dfs(el.second->next_node, v);
    }
}

vector<int64_t> suff_tree::find(string s){
    // cout<<"LOL\n";
    vector<int64_t> v;
    node* cur_node = root;
    edge* cur_edge = NULL;
    int64_t cur_idx = 0;
    // cout<<"JI\n";
    if (s.size() > data.size()) return v;
    while(true){
        if (cur_edge!=NULL){
            // cout<<"EDGE.\n";
            for(int i = cur_edge->start_idx; i< *cur_edge->end_idx;++i ){
                // cout<<"STILL ALIVE\n";
                if (s[cur_idx] != data[i]){
                    return v;
                } else{
                    cur_idx++;
                }
                if(cur_idx == s.size()){
                    // cur_node = cur_edge->next_node;
                    // cur_edge = NULL;
                    break;
                }
            }
            // cout<<"STILL ALIVE\n";
            cur_node = cur_edge->next_node;
            cur_edge = NULL;
        } else{
            // cout<<"NODE.\n";
            
            if (cur_idx >= s.size()){
                
                find_dfs(cur_node, v);
                return v;
            }

            if (cur_node->edges.count(s[cur_idx])){
                
                cur_edge = cur_node->edges[s[cur_idx]];
                // cur_node = cur_edge->next_node;
                // cout<<"JESUS  "<<cur_edge->start_idx<<"\n";
            } else{
                // cout<<"JJ\n";
                break;
            }

        }

    }


    return v;
}

void suff_tree::extension(string s) {
    data = s;
    node *cur_node = root; // есть всегда (либо текущая вершина, либо ближайшая вершина от ребра, считая от start_idx)
    edge *cur_edge = NULL;
    node *last_inter_node = NULL;

    int64_t old_suff_idx = 0;
    int64_t new_suff_idx =0;

    int64_t cur_suff_len = 0; //временная длина совпавшего суффикса
    int64_t suff_len =0; // длина совпавшего суффикса

    int64_t edge_idx = 0;
    int64_t suff_idx = 0;
    // bool bypass = false;

    for(int64_t idx =0; idx <s.size();++idx){
        // cout<<"idx = "<<idx<<"; new_suff_idx = "<<new_suff_idx<<"; suff_idx = "<<suff_idx<<"; suf_len = "<<suff_len<<"\n";
        // cout<<s[new_suff_idx+suff_idx]<<'\n';
        // suff_len = idx - new_suff_idx;
        // cur_suff_len = suff_len;

        while (suff_len >= 0){
            if (cur_edge == NULL){ // в вершине
                // cout<<"NODE. ";

                if (cur_node->edges.count(s[new_suff_idx+suff_idx])){ // есть ребро, переходим по нему, начинаем считать совпадения, если это не в процессе обхода по совпавшему суффиксу, в противном случае - нач индекс совпадания не сбрасываем
                    // "FUC\n";
                    // cout<<"EDGE is. Find: "<<s[new_suff_idx+suff_idx]<<'\n';
                    cur_edge = cur_node->edges[s[new_suff_idx+suff_idx]];

                    // if (*cur_edge->end_idx - cur_edge->start_idx == 1){
                    //         cur_edge = NULL;
                    //         edge_idx = 0;
                    //         cur_node = cur_edge->next_node;

                    // }
                    if (suff_len==0 and suff_idx ==0) {
                        new_suff_idx = idx;

                        // old_suff_idx = cur_node->edges[s[new_suff_idx+suff_idx]]->start_idx;
                        suff_idx = 1;
                        cur_suff_len = 1;
                        // cur_edge = cur_node->edges[s[new_suff_idx+suff_idx]];
                        edge_idx =1;

                        break;
                        // bypass = true;
                    }
                    // cur_edge = cur_node->edges[s[new_suff_idx+suff_idx]];
                    suff_idx++;
                    // if (suff_len==0){}
                    cur_suff_len++;

                    edge_idx = 1;
                    // cout<<"Only suff++\n";
                    if (suff_len==0) break;
                    // break;
                    // // cout<<"AAAAAAAAAAAAAA\n";

                    
                } else{// нет ребра - создаем
                    // cout<<"Create new edge| idx= "<<new_suff_idx+suff_idx<<" | word = "<<s[new_suff_idx+suff_idx]<<'\n';

                    
                    node *new_node= new node(cnt_node);
                    cnt_node++;
                    edge *new_edge = new edge(new_suff_idx+suff_idx, &end, cur_node, new_node);
    
                    cur_node->edges[s[new_suff_idx+suff_idx]] = new_edge;

                    if (cur_node->suff_link){
                        cur_node = cur_node->suff_link;
                        // cout<<"-bypass- "<<cur_node->idx<<'\n';

                    } 
                    else{
                        new_suff_idx++;
                        suff_idx = 0;
                        // suff_len = idx - new_suff_idx;
                        // cur_suff_len = suff_len;
                        break;
                        // // cout<<"HUI\n";
                    }

                    // new_suff_idx++;
                    // // suff_idx--;
                    // suff_len = idx - new_suff_idx;
                    // cur_suff_len = suff_len;
                    // suff_idx = 0;

                }
                // edge_idx = 0;
                
            } else{ // на ребре, тут можем оказаться только в случае обхода
                // cout<<"EDGE. start_id = "<<cur_edge->start_idx<<" | end_idx = "<<*cur_edge->end_idx<<'\n';
                // cout<<"cur_sufflen = "<<cur_suff_len<<'\n';
                // cout<<"edge_idx = "<<edge_idx<<'\n';

                if (1 == *cur_edge->end_idx - cur_edge->start_idx ){
                    // cur_edge = cur_edge->last_node->edges[s[new_suff_idx+suff_idx]];
                    cur_node = cur_edge->next_node;
                    cur_edge = NULL;
                    
                    edge_idx = 0;                    
                    cur_suff_len-=2;
                    // cout<<"\nio\n";
                    continue;

                }
                if (suff_len>0 and  cur_suff_len >=  *cur_edge->end_idx - cur_edge->start_idx){ // случай, когда обходим другие ребра чтобы вставить суфф ссылку
                    cur_suff_len -= *cur_edge->end_idx - cur_edge->start_idx;
                    suff_idx +=*cur_edge->end_idx - cur_edge->start_idx;
                    cur_node = cur_edge->next_node;
                    edge_idx =0;
                    cur_edge = NULL;
                    // cout<<"->Next EDGE\n";
                    continue;
                }
                
                if (suff_len > 0){
                    edge_idx = cur_suff_len-1;
                    suff_idx = cur_suff_len-1;
                }


                if (s[new_suff_idx+suff_idx]!=s[cur_edge->start_idx+edge_idx]){ // если нашли несовпадение - создаем внутреннюю вершину
                    // cout<<"Create inter node\n";
                    // cout<<"s[new_suff_idx+suff_idx] = "<<s[new_suff_idx+suff_idx]<<" | s[cur_edge->start_idx+edge_idx] = "<<s[cur_edge->start_idx+edge_idx]<<'\n' ;
                    // // cout<<"idx ="<<cur_edge->start_idx+edge_idx;
                    // // cout<<'\n';
                    
                    node* new_node = new node(cnt_node);
                    node* cont_node = new node(cur_edge->start_idx);


                    int64_t *new_end = new int64_t(cur_edge->start_idx+edge_idx);

                    edge *new_edge = new edge(idx, &end, cur_edge->next_node, new_node);
                    edge *cont_edge = new edge(*new_end, &end, cur_edge->next_node, cont_node);

                    // cur_node = cur_edge->next_node;
                    cur_edge->next_node->edges[s[idx]] = new_edge;
                    cur_edge->next_node->edges[s[*new_end]] = cont_edge;
                    // cur_edge->next_node->idx = -1;

                    cur_edge->end_idx = new_end;

                    if (last_inter_node){
                        last_inter_node->suff_link = cur_edge->next_node;
                        // cout<<"[INFO]: SETTING suff link. Suff_link from "<<last_inter_node->idx<<" to "<<cur_edge->next_node->idx<<"\n";
                        
                    }

                    last_inter_node = cur_edge->next_node;

                    if (cur_node->suff_link){
                        // cout<<"[INFO] go using suff link from -> "<<cur_node->idx<<" | to -> "<<cur_node->suff_link->idx<<'\n';
                        cur_node = cur_node->suff_link;
                        // // cout<<"[INFO] go using suff link from \n";
                    }

                    new_suff_idx++;
                    suff_len = idx - new_suff_idx;
                    cur_suff_len = suff_len;
                    suff_idx = 0;
                    // cout<<"INFO after split: new_suff_idx = "<<new_suff_idx<<" | cur_len = "<<cur_suff_len<<" | suf_len = "<<suff_len<<'\n';

                    cur_edge = NULL;
                    cnt_node++;

                } else{
                    suff_idx++;
                    edge_idx++;
                    cur_suff_len++;
                    if (cur_edge->start_idx+edge_idx >= *cur_edge->end_idx){ // совершали обход и закончилось ребро, оказались в вершине
                    
                        cur_node = cur_edge->next_node;
                        cur_edge = NULL;
                    }
                    // if(idx == s.size()-2){
                    //     continue;
                    // }
                    break;;
                    
                }
            }


            // if(cur_node->suff_link){
            //     cur_node = cur_node->suff_link;
            // // }
            // start_suff_idx++;
            // cur_suff_len = idx - start_suff_idx; пересмотреть
            // по-хорошему, если мы просто обходим (правило 3) мы ничего не должны делать, но смотря какой это обход, если первый, то ничего, если уже добавление суффикса после создания внутренней вершины, то нам 
            // // // cout<<"****"<<cur_suff_len;
        }    
        end++;  
        if(last_inter_node){
            last_inter_node->suff_link = root;
            // cout<<"[INFO]: SETTING suff link. Suff_link from "<<last_inter_node->idx<<" to root"<<"\n";
            last_inter_node = NULL;
        } 
        // // cout<<"idx = "<<idx<<"; new_suff_idx = "<<new_suff_idx<<"; suff_idx = "<<suff_idx<<";\n";
    }
}


void suff_tree::del_nodes(node* cur_node){
    if (!cur_node->edges.empty()){
        for(auto& el : cur_node->edges){
            del_nodes(el.second->next_node);

            delete el.second;
        }

    }
    delete cur_node;

}


// bool suff_tree::check_suff_belong(node* cur_node, int64_t suff_border) {
//     bool containsX = false, containsY = false;


//     while (!stack.empty()) {
//         Node* current = stack.back();
//         stack.pop_back();

//         if (current->suffix_index > -1) {
//             if (current->suffix_index < textXLen) containsX = true;
//             if (current->suffix_index > textXLen) containsY = true;
//         }

//         if (containsX && containsY) return true;

//         for (auto it : current->nodes)
//             stack.push_back(it.second);
//     }

//     return false;
// }

pair<int64_t, vector<int64_t>> suff_tree::dfs(node* cur_node, int64_t border) {
    int64_t max_depth = 0;
    vector<int64_t> v;
    bool is_leaf = true;
    // cout<<"+++Cur_node = "<<cur_node->idx<<"\n";
    for(auto el : cur_node->edges){
        int64_t cur_len = *el.second->end_idx - el.second->start_idx;
        if (*el.second->end_idx<=border){ // продолжжаем рекурсивный спуск
            is_leaf = false;
            pair<int64_t, vector<int64_t>> tmp_pair = dfs(el.second->next_node, border);
            if (tmp_pair.first ==-1){
                if (cur_len > max_depth){
                    // cout<<"{INFO} DFS  New max = "<<cur_len<<" | Old = "<<max_depth<<"\n";
                    max_depth = cur_len;
                    v.clear();
                    v.push_back(*el.second->end_idx);
                    
                } else if(cur_len == max_depth){
                    v.push_back(*el.second->end_idx);
                }
            } else{
                if (tmp_pair.first + cur_len > max_depth){
                    // cout<<"{INFO} DFS  New max = "<<tmp_pair.first + cur_len<<" | Old = "<<max_depth<<"\n";
                    max_depth = tmp_pair.first + cur_len;
                    v.clear();
                    v = tmp_pair.second;
                } else if(tmp_pair.first + cur_len ==max_depth){
                    v.insert(v.end(), tmp_pair.second.begin(), tmp_pair.second.end());
                    // cout<<"ADD to v\n";
                }
            }
        } 
    }
    if(is_leaf) max_depth = -1;
    // cout<<"----\n";
    return {max_depth, v};
}


// int64_t suff_tree::dfs(edge* cur_edge, int64_t border) {
//     int64_t max_depth = 0;
//     int64_t cur_len = *cur_edge->end_idx - cur_edge->start_idx;
//     if (*cur_edge->end_idx==border){
//         return cur_len;
//     } else if(*cur_edge->end_idx< border){
//         for(auto el: cur_edge->next_node->edges){
//             int64_t dfs_val = dfs(el.second, border);
//             if (dfs_val> max_depth) max_depth = dfs_val;
//         }
//         return max_depth + cur_len;
//     } else{
//         return 0;
//     }
// }


// pair<int64_t, vector<int64_t>> suff_tree::LCS(int64_t s1_size) {
//     int64_t max_depth = 0;
//     int64_t mx = 0;
//     vector<int64_t> v;

//     for(auto el : root->edges){
//         if (*el.second->end_idx > s1_size) continue;
//         int64_t cur_len = *el.second->end_idx - el.second->start_idx;
//         int64_t tmp = dfs(el.second, s1_size);
//         if(tmp > mx) mx = tmp;
//         if (cur_len + tmp > max_depth ){
//             max_depth = cur_len + tmp;
//             v.clear();
//             v.push_back(el.second->start_idx);
//         } else if(cur_len + tmp == max_depth){
//             v.push_back(el.second->start_idx);
//         }
//     }
    
//     // cout<<"HUI = "<<mx<<'\n';
//     return {max_depth, v};
// }
pair<int64_t, vector<int64_t>> suff_tree::LCS(int64_t s1_size){

    return dfs(root, s1_size);
}


suff_tree::~suff_tree(){
    del_nodes(root);
}


int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    suff_tree ST;

    // 5

    // string s1,s2;
    // cin>>s1>>s2;
    // if (s1.size()>s2.size()){
    //     swap(s1,s2);
    // }
    // string s = s1+"#"+s2+"$";
    // ST.extension(s);
    // pair<int64_t, vector<int64_t>> answ = ST.LCS(5);
    // cout<<answ.first<<'\n';
    // for(int64_t i =0; i<answ.second.size();++i){
    //     cout<<s.substr(answ.second[i]-answ.first, answ.first)<<'\n';
    // }

    //


    string tmp;
    cin>>tmp;
    tmp += "$";
    ST.extension(tmp);

    ST.print_tree();
    // return 0;

    int idx=0;
    while(cin>>tmp){
        vector<int64_t> v = ST.find(tmp);
        idx++;
        if (v.size()==0) continue;
        sort(v.begin(), v.end());
        cout<<idx<<": ";
        for(int i =0; i<v.size()-1;++i){
            cout<<v[i]<<", ";
        }
        cout<<v[v.size()-1]<<"\n";
        idx++;

    }


    // ST.extension("abakcfakchakc$");
    // ST.extension("abakcfakch$");
    // ST.extension("abad");
    

    // ST.extension("xabay#xabcbay$");

    // ST.extension("abakcfakcak$");
    // // cout<<

    // ST.print_tree();

}
// xabay
// xabcbay
