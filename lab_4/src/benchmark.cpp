#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstdint>

#include <algorithm>
#include <chrono>

using namespace std;

using duration_t = chrono::microseconds;
const string DURATION_PREFIX = "s";

const int EXT_COEF = 2;
const int STR_SIZE = 65;
const int MIN_CAP = 10;
const int64_t EL_RANGE = 65536;

typedef pair<uint64_t, uint64_t> TPos;

template <class T>
using TVal = pair<T, TPos>;

// Возвращаем ближайший элемент, меньше данного
template<class T>
uint64_t nearest_pos(T needed_val, vector<T> &array){
    if(array.size()==0) return 0;
    uint64_t l =0;
    uint64_t r = array.size()-1;
    while (l<r){
        uint64_t m = (r+l+1)/2;
        // cout<<"\nStep | l = "<<l<<"; r = "<<r<<"; m = "<<m<<"\n";
        if (needed_val > array[m]){
            l = m;
        } else {
            r = m-1;
        }
    }
    return l;
}


template<class T>
vector<T> pattern_parser(){
    string str_input_data;
    getline(cin, str_input_data);
    istringstream input_stream(str_input_data);
    vector<T> v_output_data;
    T cur_val;
    while(input_stream>>cur_val){
        v_output_data.push_back(cur_val);
    }
    return v_output_data;
}

template<class T>
vector<TVal<T>> text_parser(uint64_t start_num_line, uint64_t start_num_val){
    vector<TVal<T>> v_output_data;
    string str_line;
    uint64_t cnt_line = start_num_line;
    while(getline(cin, str_line)){
        istringstream input_stream(str_line);
        T cur_val;
        uint64_t cnt_val = start_num_val;
        while(input_stream>>cur_val){
            v_output_data.push_back(pair(cur_val, pair(cnt_line, cnt_val)));
            cnt_val++;
        }
        cnt_line++;
    }

    return v_output_data;
}

namespace BM_search{
    template<class T>
    class TPatern{
        private:
            vector<T> pattern_data;
            unordered_map<T, vector<uint64_t>> bad_symbol_pos;
            vector<uint64_t> good_suf_pos;
            

            void bad_symbols_init();
            void good_suffix_init();

            uint64_t bad_symbol_shift(uint64_t cur_idx, T mismatch_val);
            uint64_t good_suffix_shift(uint64_t cur_idx);    

                 

        public:
            vector<uint64_t> N_array;

            TPatern(vector<T> input_array){
                pattern_data = move(input_array);
                bad_symbols_init();
                good_suffix_init();
            }

            uint64_t size(){
                return pattern_data.size();
            }

            T operator [] (uint64_t idx) const{
                return this->pattern_data[idx];
            }

            T& operator [] (const uint64_t idx){
                return this->pattern_data[idx];
            }

            uint64_t get_shift(uint64_t cur_pattern_idx, T mismatch_val);

            void print_bad_symbol_array(); 
            void print_good_suffix_array();

            ~TPatern(){};
    };

    template<class T>
    vector<TPos> search_func( vector<TVal<T>> text_data, TPatern<T> pattern);

    template <class T>
    vector<uint64_t> make_z_function(vector<T> input_vector);

    template <class T>
    vector<T> reverse_array(vector<T> &input_vector);
};

template<class T>
vector<TPos> BM_search::search_func( vector<TVal<T>> &text_data, TPatern<T> &pattern){
    uint64_t p_size = pattern.pattern_size();
    vector<TPos> answ;
    uint64_t r_border = p_size-1;
    if (text_data.size()< p_size) return answ;
    while(r_border<text_data.size()){
        uint64_t i_t=r_border, j_p=p_size-1;
        while(j_p>=0 and text_data[i_t].first ==pattern[j_p]){
            i_t--;
            j_p--;
        }
        if(j_p<0){
            answ.push_back(text_data[i_t].second);
            r_border++;
        } else{
            r_border += pattern.get_shift(j_p, text_data[i_t].first);
        }
    }
    return answ;
}


template<class T>
void BM_search::TPatern<T>::bad_symbols_init(){
    for(int i=0;i<pattern_data.size();++i){
        if(!bad_symbol_pos.contains(pattern_data[i])){
            vector<uint64_t> new_vector(1, i);
            bad_symbol_pos.insert({pattern_data[i], new_vector});
        } else{
            vector<uint64_t> &vector_ref = bad_symbol_pos[pattern_data[i]];
            vector_ref.push_back(i);
        }
    }
}

template<class T>
uint64_t BM_search::TPatern<T>::bad_symbol_shift(uint64_t cur_idx, T mismatch_val){
    if (!bad_symbol_pos.contains(mismatch_val)){
        return 1;
    }
    vector<T> &array_ref = bad_symbol_pos[mismatch_val];
    uint64_t answ= nearest_pos<uint64_t>(cur_idx, array_ref);
    return cur_idx - array_ref[answ];
}

template<class T>
void BM_search::TPatern<T>::print_bad_symbol_array(){
    for(const auto &it : bad_symbol_pos){
        cout<<"Pattern "<<it.first<<" ; idx: ";
        // cout<<it.first<<' '; 
        vector<uint64_t> &vector_ref= bad_symbol_pos[it.first];
        for(int j = 0; j< vector_ref.size();++j){
            cout<<vector_ref[j]<<' ';
        }
        cout<<"\n";        
    }
}


template <class T>
vector<uint64_t> BM_search::make_z_function(vector<T> input_vector){
    uint64_t n = input_vector.size();
    vector<uint64_t> z(n);
    uint64_t l=0,r=0;
    for(int i =1; i< n;++i){
        if(i<r){
            z[i]=min(z[i-l], r-i);
        }
        while (i+z[i]< n and input_vector[z[i]]==input_vector[z[i]+i]){ //наивно
            ++z[i];
        }
        if (i + z[i] > r){
            l=i;
            r = i+ z[i];
        }
    }
    return z;
}

template <class T>
vector<T> BM_search::reverse_array(vector<T> &input_vector){
    uint64_t v_size=input_vector.size();
    vector<T> output_v(v_size);
    for(uint64_t i=0; i<v_size;++i){
        output_v[i] = input_vector[v_size-1-i];
    }
    return output_v;
}

template <class T>
void  BM_search::TPatern<T>::good_suffix_init(){
    uint64_t n = pattern_data.size();
    // cout<<"\nN ="<<n<<"\n";
    if (n==0) return;
    good_suf_pos.resize(n,0);
    N_array.resize(n, 0);
    vector<uint64_t> z_func_re = make_z_function<T>(reverse_array<T>(pattern_data));
    good_suf_pos[n-1] = 1;
    for(uint64_t i=n-1; i>0;--i){
        // cout<<"Z["<<i<<"]="<<z_func_re[i]<<'\n';
        if(z_func_re[i]!=0) {
            good_suf_pos[n-1-z_func_re[i]]=i;
            N_array[n-1-i]= z_func_re[i];
        }
    }
    for(int i=n-2; i>0;--i){
        if (good_suf_pos[i]==0){
            good_suf_pos[i]=good_suf_pos[i+1];
        }
    }
    if(good_suf_pos[0]==0 and n>1) good_suf_pos[0]=good_suf_pos[1];

    // cout<<"\nN array:\n";
    // for(uint i=0; i<n;++i ){
    //     cout<<N_array[i]<<' ';
    // }

}

template<class T>
uint64_t BM_search::TPatern<T>::good_suffix_shift(uint64_t cur_idx){
    return good_suf_pos[cur_idx];
}

template<class T>
void BM_search::TPatern<T>::print_good_suffix_array(){
    cout<<"\nGood suf array:\n";
    for(uint64_t i =0; i<good_suf_pos.size();++i){
        cout<<good_suf_pos[i]<<' ';
    }
}




template<class T>
uint64_t BM_search::TPatern<T>::get_shift(uint64_t cur_pattern_idx, T mismatch_val){
    return max(uint64_t(1), max(bad_symbol_shift(cur_pattern_idx, mismatch_val), good_suffix_shift(cur_pattern_idx)));
}


// ====================== main ===========================

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    vector<uint64_t> pattern_v = pattern_parser<uint64_t>();
    vector<TVal<uint64_t>> text_data = text_parser<uint64_t>(1,1);
    vector<uint64_t> M_vector(text_data.size(), -1);
    

    //========================================================
    auto algorithm_begin_time=chrono::high_resolution_clock::now();

    BM_search::TPatern<uint64_t> pattern(pattern_v);
    uint64_t p_size = pattern.size();
    uint64_t r_border = p_size-1;
    vector<TPos> v_answ;
    while(r_border<text_data.size()){
        uint64_t i_t=r_border, i_p=p_size-1;
        while(i_p>0 and text_data[i_t].first == pattern[i_p]){
            if(M_vector[i_t]==uint64_t(-1) or (M_vector[i_t]==0  and M_vector[i_t]==pattern.N_array[i_p])){
                if (text_data[i_t].first == pattern[i_p]){
                    i_p--;
                    i_t--;
                } else{
                    M_vector[r_border] = r_border - i_t;
                    break;
                }
            } else if (M_vector[i_t]!=uint64_t(-1)){
                if (M_vector[i_t]<pattern.N_array[i_p]){
                    i_p-=M_vector[i_t];
                    i_t-=M_vector[i_t];
                } else if(M_vector[i_t]==pattern.N_array[i_p]){
                    if (pattern.N_array[i_p]==i_p){
                        M_vector[r_border] = r_border-i_t;
                    }                
                    i_p-=M_vector[i_t];
                    i_t-=M_vector[i_t];                    
                } else{
                    if (pattern.N_array[i_p]==i_p){
                        M_vector[r_border] = r_border-i_t;
                        i_p-=M_vector[i_t];
                        i_t-=M_vector[i_t];                        
                    } else {
                        M_vector[r_border] = r_border - i_t;
                        break;
                    }
                }
            

            }
        }
        // cout<<"Current border = "<<r_border<<"; ip = "<<i_p<<"; it ="<<i_t<<"\n"; 
        if (i_p==0 and text_data[i_t].first == pattern[i_p]){
            v_answ.push_back(text_data[i_t].second);
            r_border++;
        } else{
            r_border+= pattern.get_shift(i_p, text_data[i_t].first);
        }
    }

    auto algorithm_end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> bm_algorithm_time=algorithm_end_time - algorithm_begin_time;


    //==========================================================
    vector<TPos> v_answ_dumm;
    uint64_t text_size = text_data.size();
    algorithm_begin_time=chrono::high_resolution_clock::now();

    if (text_size < p_size or text_size==0 or p_size==0){
		return 1;
    }
    uint64_t idx =0;
    for(uint64_t i=0; i<=text_size - p_size;++i){
        uint64_t l_border=i;
        for(uint64_t j=0; j<p_size;++j){
            idx=j;
            if (pattern_v[j]!=text_data[i + j].first){
                break;
            }

        }
        if ( idx==p_size-1 and pattern_v[idx]==text_data[idx+l_border].first) v_answ_dumm.push_back(text_data[l_border].second);
    }
    
    // while(l_border <= text_size - p_size){
    //     uint64_t idx =0;
    //     while(idx < p_size and pattern_v[idx]==text_data[l_border + idx].first){
    //         idx++;
    //     }
    //     if (idx ==p_size){
    //         v_answ_dumm.push_back(text_data[l_border].second);
    //     }
    //     l_border++;
    // }
    algorithm_end_time=chrono::high_resolution_clock::now();
    chrono::duration<double> stl_sort_time=algorithm_end_time - algorithm_begin_time;
    cout <<fixed;
    cout.precision(9);

    // cout<<"\nAnsw:\n";
    // for(int i=0; i<v_answ_dumm.size();++i){
    //     cout<<v_answ_dumm[i].first<<", "<<v_answ_dumm[i].second<<"\n";
    // }

    cout<<"Pattern size = "<<p_size<<'\n';
    cout<<"Text size = "<<text_size<<"\n";
    cout<<"Apostolico_Giancarlo algorithm time: "<< bm_algorithm_time.count()<<" "<<DURATION_PREFIX<<'\n';
    cout<<"Dumm algorithm time:      "<< stl_sort_time.count()<<" "<<DURATION_PREFIX<<'\n';


}