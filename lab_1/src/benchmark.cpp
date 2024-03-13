#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <vector>

using namespace std;

using duration_t = chrono::microseconds;
const string DURATION_PREFIX = "s";

const int EXT_COEF = 2;
const int STR_SIZE = 65;
const int MIN_CAP = 10;
const int64_t EL_RANGE = 65536;


class TObj{
    public:
        unsigned int key;
        int64_t idx;
};

// ====================== TStr_64 ===========================

class TStr_64{
    friend istream& operator >>(istream& is, TStr_64 &el);
    friend ostream& operator <<(ostream& os, TStr_64 &el);
    public:
        TStr_64() = default;
        int size(){
            return val_size;
        }
        string To_str(){
            return string(val, STR_SIZE);
        }
        bool Is_nonval(){
            return val_size==1;
        }
    private:
        char val[STR_SIZE];
        int val_size;
};

istream& operator>>(istream& is, TStr_64 &el){
    string tmp;
    getline(cin, tmp);
    el.val_size=tmp.size();
    strncpy(el.val, tmp.c_str(), el.val_size);
    // is>>el.val;
    // el.val_size = strlen(el.val);

    return is;
}

ostream& operator <<(ostream& os, TStr_64 &el){
    os<<string(el.val, el.val_size);
    return os;
}
// =========================================================

// ====================== sort ===========================

void counting_sort(TObj *q_array, int64_t q_array_size, vector<TObj> &in_array){
    int64_t counting_array[EL_RANGE];
    fill_n(counting_array, EL_RANGE, 0);
    for(int i=0; i< in_array.size(); ++i){
        counting_array[in_array[i].key]++;
    }
    for(int i=1; i< EL_RANGE; ++i){
        counting_array[i]+=counting_array[i-1];
    }
    for (int i=in_array.size()-1; i>=0;--i ){
        counting_array[in_array[i].key]--;
        q_array[counting_array[in_array[i].key]]=in_array[i];
    }
}

bool cmp(TObj a, TObj b){
    return a.key<b.key;
}

// ====================== main ===========================

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    vector<TStr_64> val_array;
    vector<TObj> key_array;
    // vector<TObj> key_array_stl;
    TStr_64 val;
    unsigned int key;
    int64_t counter=0;
    while (cin>>key>>val){
        TObj tmp;
        if (!val.Is_nonval()){
            tmp.idx=counter;
            tmp.key=key;

            key_array.push_back(tmp);
            // key_array_stl.push_back(tmp);

            val_array.push_back(val);
            counter++;          
        }
    }
    TObj q_array[counter];

    auto sort_begin_time=chrono::high_resolution_clock::now();
    counting_sort(q_array, counter, key_array);
    auto sort_end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> count_sort_time=sort_end_time - sort_begin_time;

    sort_begin_time=chrono::high_resolution_clock::now();
    sort(key_array.begin(), key_array.end(), cmp);
    sort_end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> stl_sort_time=sort_end_time - sort_begin_time;
    cout <<fixed;
    cout.precision(9);
    cout<<"Array size = "<<counter<<'\n';
    cout<<"Counting sort time: "<< count_sort_time.count()<<" "<<DURATION_PREFIX<<'\n';
    cout<<"STL-sort time:      "<< stl_sort_time.count()<<" "<<DURATION_PREFIX<<'\n';
    // for(int i=0;i<key_array.size();++i){
    //     cout<<key_array[i].key<<' ';
    // }
    // cout<<'\n';
    // for(int i=0;i<key_array.size();++i){
    //     cout<<q_array[i].key<<' ';
    // }

}