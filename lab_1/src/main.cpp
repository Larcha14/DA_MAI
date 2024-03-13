#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
using namespace std;
// #define EXT_COEF 2
// #define STR_SIZE 65
// #define MIN_CAP 10
// // #define EL_RANGE 10
// #define EL_RANGE 65536

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

// ====================== vector ===========================

template <class T>
class TMy_vector{
    private:
        T* vector_buff;
        int64_t vector_cap;
        int64_t vector_size;
        int64_t vector_head;
        int64_t vector_tail;
        bool increase();
    public:
        TMy_vector();
        T operator [] ( int64_t idx ) const{
            return this->vector_buff[idx];
        }
        T& operator [] ( int64_t idx ){
            return this->vector_buff[idx];
        }

        int64_t Size(){
            return this->vector_size;
        }
        
        void Push_back(T val);
        ~TMy_vector();


};

template<class T>
bool TMy_vector<T>::increase(){
    int64_t new_cap= this->vector_cap*EXT_COEF;
    T* tmp = (T*)realloc(this->vector_buff, new_cap*sizeof(T));
    if (tmp==NULL){
        return false;
    }
    this->vector_buff=tmp;
    //cout<<"[INFO] memory allocating from "<<this->vector_cap<<" b to "<<new_cap<<" b\n";
    this->vector_cap=new_cap;
    return true;

}

template< class T>
TMy_vector<T>::TMy_vector(){
    this->vector_size=0;
    this->vector_head=0;
    this->vector_tail=0;
    this->vector_buff=(T*)malloc(MIN_CAP*sizeof(T));
    this->vector_cap=MIN_CAP;
}

template<class T>
TMy_vector<T>::~TMy_vector(){
    free(this->vector_buff);
    this->vector_cap=0;
    this->vector_size=0;
    this->vector_head=0;
    this->vector_tail=0;
}

template<class T>
void TMy_vector<T>::Push_back(T val){
    vector_buff[this->vector_size]=val;
    this->vector_size++;
    
    if (this->vector_size+1 == this->vector_cap){
        this->increase();
    }

}
// =======================================================

// ====================== sort ===========================

void counting_sort(TObj *q_array, int64_t q_array_size, TMy_vector<TObj> &in_array){
    int64_t counting_array[EL_RANGE];
    fill_n(counting_array, EL_RANGE, 0);
    for(int i=0; i< in_array.Size(); ++i){
        counting_array[in_array[i].key]++;
    }
    for(int i=1; i< EL_RANGE; ++i){
        counting_array[i]+=counting_array[i-1];
    }
    for (int i=in_array.Size()-1; i>=0;--i ){
        counting_array[in_array[i].key]--;
        q_array[counting_array[in_array[i].key]]=in_array[i];
    }
}


// ====================== main ===========================

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    TMy_vector<TStr_64> val_array;
    TMy_vector<TObj> key_array;
    TStr_64 val;
    unsigned int key;
    int64_t counter=0;
    while (cin>>key>>val){
        TObj tmp;
        if (!val.Is_nonval()){
            tmp.idx=counter;
            tmp.key=key;

            key_array.Push_back(tmp);

            val_array.Push_back(val);
            counter++;          
        }
    }
    TObj q_array[counter];
    // cout<<"Output:\n";
    counting_sort(q_array, counter, key_array);
    // cout<<'\n';
    if (counter>0){
        cout<<q_array[0].key<<val_array[q_array[0].idx];
    }
    for(int i=1;i< counter; ++i){
        cout<<'\n'<< q_array[i].key<<val_array[q_array[i].idx];
    }
}