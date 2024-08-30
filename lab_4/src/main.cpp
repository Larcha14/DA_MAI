#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <sstream>

using namespace std;

typedef pair<uint64_t, uint64_t> TCoord;
typedef pair<uint64_t, TCoord> TSymbol;


uint64_t bin_search(uint64_t needed_idx, vector<uint64_t> &array){
    uint64_t l=0, r=array.size()-1;
    while(l<r){
        uint64_t m = (l+r+1)/2;
        if (array[m]< needed_idx){
            l = m;
        } else{
            r = m -1;
        }

    }
    // if (array[l]>= needed_idx){
    //     return 1;
    // }
    return array[l];
    // return needed_idx - array[l];
}

template <class T>
uint64_t pattern_parser(string const &input_text, vector<T> &q_words_array){
    T cur_elem;
    istringstream input_stream(input_text);
    uint64_t size=0;
    while(input_stream>>cur_elem){
        q_words_array.push_back(cur_elem);
        size++;
    }
    return size;;

}

template <class T>
uint64_t text_parser(string const &input_text, vector<pair<T, TCoord>> &q_words_array, uint64_t line_num){
    T cur_elem;
    istringstream input_stream(input_text);
    uint64_t in_line_idx = 1; // Храним все индексы начиная с 1
    while(input_stream>>cur_elem){
        pair<T, TCoord> tmp_pair(cur_elem, TCoord(line_num, in_line_idx));
        q_words_array.push_back(tmp_pair);
        in_line_idx++;
    }
    return in_line_idx-1;

}

namespace Boyer_Moore_search{
    template <class T>
    class pattern{
        private:
            vector<T> pattern_array;
            unordered_map<T, vector<uint64_t>> bad_symbol_pos;
            vector<uint64_t> good_suf_pos;

            void bad_symbols_init();
            void good_suffix_init();

            uint64_t bad_symbol_shift(uint64_t cur_idx, T mismatch_val);
            uint64_t good_suffix_shift(uint64_t cur_idx);


        public:
            pattern(vector<T> new_array);
            uint64_t get_shift(uint64_t cur_idx, T mismatch_val);
            // uint64_t bad_symbol_shift(uint64_t cur_idx, T mismatch_val);
            T operator [] (uint64_t idx) const{
                return this->pattern_array[idx];
            }

            T& operator [] (const uint64_t idx){
                return this->pattern_array[idx];
            }

            void print_bad_symbol_array();

            vector<T> reverse_array(vector<T> const &array);



            ~pattern(){};

    };

    template <class T>
    vector<uint64_t> z_function(vector<T> const &array);

};
 
template <class T>
Boyer_Moore_search::pattern<T>::pattern(vector<T> new_array){
    pattern_array = move(new_array);

    bad_symbols_init();
    good_suffix_init();
}
     
template <class T>
void Boyer_Moore_search::pattern<T>::bad_symbols_init(){
    for(int i=0; i<pattern_array.size();++i){
        if(!bad_symbol_pos.contains(pattern_array[i])){
            vector<uint64_t> new_vector(1, i);
            bad_symbol_pos.insert({pattern_array[i], new_vector});
        } else{
            vector<uint64_t> &vector_ref= bad_symbol_pos[pattern_array[i]];
            vector_ref.push_back(i);
        }
    }
}

template<class T>
uint64_t Boyer_Moore_search::pattern<T>::bad_symbol_shift(uint64_t idx, T mismatch_val){ // самое правое вхождение символа, сильное правилр плохого символа
    if (!bad_symbol_pos.contains(mismatch_val)){ // нет такого символа в паттерне, значит сдвигаем слово полностью
        return idx+1;
    } else{
        vector<uint64_t> &vector_ref= bad_symbol_pos[mismatch_val];
        uint64_t answ = bin_search(idx, vector_ref);
        if ( answ >= idx) answ = 1;
        return answ;
    }
}

template<class T>
void Boyer_Moore_search::pattern<T>::print_bad_symbol_array(){
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
vector<uint64_t> Boyer_Moore_search::z_function(vector<T> const &array){
    uint64_t n = array.size();
    vector<uint64_t> z(n);
    z[0] = n;
    uint64_t l=0, r=0;
    for(int i=0; i< n;++i){
        if (i<r){
            z[i] = min(r-i, z[i-l]);
        }
        while(i + z[i] < n and array[z[i]]==array[i+z[i]]){
            ++z[i];
        }
        if (i + z[i] > r){
            l=i;
            r = l + z[i]-1;
        }
        // cout<<"i= "<<i<<"; l= "<<l<<" r= "<<r<<"; z[i]= "<<z[i]<<'\n';
    }
    return z;
}

template <class T>
vector<T> Boyer_Moore_search::pattern<T>::reverse_array(vector<T> const &input_array){
    vector<T> v_answ;
    for(int i=0; i< input_array.size();++i){
        v_answ.push_back(input_array[input_array.size()-1-i]);
    }
    return v_answ;
}


template <class T>
void Boyer_Moore_search::pattern<T>::good_suffix_init(){
    vector<uint64_t> z_pr =  z_function(reverse_array(pattern_array)); // z_function(reverse(pattern))
    vector<uint64_t> z_p = z_function(pattern_array);
    vector<uint64_t> prefix_shift = z_function(pattern_array);
    uint64_t n =z_pr.size();
    good_suf_pos.resize(n); // L_j' array
    for(int i=n-1; i>0;--i){ // 0 не включаем т.к. там стоит невалидное значение
        if (z_pr[i]!=0){
            good_suf_pos[n -1-z_pr[i]] = i; 
        }
        
    }
    good_suf_pos[n-1]=1;
    bool zero_find = false;
    bool fill_array =false;
    uint64_t fill_idx=1;
    for(int i=n-2; i>=0;--i){
        if (fill_array){
            if (good_suf_pos[i]==0) good_suf_pos[i]=fill_idx;
        }
        if (prefix_shift[i]!=0 and not fill_array){
            fill_idx=i;
            zero_find=true;
        }
        if (prefix_shift[i]==0 and zero_find and not fill_array){
            fill_array = true;
        }
        if (good_suf_pos[i]==0) good_suf_pos[i]=1;
    }


}

template <class T>
uint64_t Boyer_Moore_search::pattern<T>::good_suffix_shift(uint64_t cur_idx){
    return good_suf_pos[cur_idx];
}

template <class T>
uint64_t Boyer_Moore_search::pattern<T>::get_shift(uint64_t cur_idx, T mismatch_val){
    return max(good_suffix_shift(cur_idx), bad_symbol_shift(cur_idx, mismatch_val));
}

int main(){
    // Считывааем паттерн и создаем вектор 
    string pattern_str;
    uint64_t pattern_size;
    vector<uint64_t> pattern_array;
    getline(cin, pattern_str);
    pattern_size = pattern_parser<uint64_t>(pattern_str, pattern_array);

    Boyer_Moore_search::pattern<uint64_t> pattern(pattern_array);
    // pat.print_bad_symbol_array();

    //Считываем текст и создаем вектор
    vector<TSymbol> text_array;
    string text;
    uint64_t cnt =1; // Храним все индексы начиная с 1
    uint64_t line_lenght;
    while(getline(cin, text)){
        // cout<<"Curent string: "<<text<<"\n";
        line_lenght = text_parser<uint64_t>(text, text_array, cnt);

        // cout<<"Size = "<<cnt<<'\n'<<"Our array: ";
        // for(uint64_t i=0; i< cnt;++i){
        //     cout<<data_array[i]<<' ';
        // }
        cout<<"\n";
        // cout<<"summ = "<<data_array[0]+data_array[1]<<"\n";
        cnt++;
    }

    vector<TCoord> answ;
    uint64_t r_border = pattern_size; // == k
    uint64_t text_size = text_array.size();
    while (r_border <= text_size){
        uint64_t i_p = pattern_size -1;
        uint64_t h_t = r_border;
        while(pattern[i_p]==text_array[h_t].first and i_p>=0){
            i_p--;
            h_t--;
        }
        if (i_p==-1) {
            answ.push_back(text_array[h_t].second);
            r_border++;
        } else{
            r_border+= pattern.get_shift(i_p, text_array[h_t].first);
        }
    }

    //Вывод результатов
    for(int i=0; i<answ.size();++i ){
        cout<<answ[i].first<<", "<<answ[i].second;
        if(i!=answ.size()-1){
            cout<<"\n";
        }
    }


    // cout<<"Bad symbol: "<<pat.bad_symbol_shift(9, 'b');
    // vector<uint64_t> answ = Boyer_Moore_search::z_function(st);
    // for(int i=0; i< answ.size(); ++i){
    //     cout<< st[i]<<' ';
    // }
    // cout<<'\n';
    // for(int i=0; i< answ.size(); ++i){
    //     cout<< answ[i]<<' ';
    // }
}
// 13 0 3 0 1 0 5 0 5 0 3 0 1