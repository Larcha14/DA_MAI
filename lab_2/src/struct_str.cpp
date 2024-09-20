#include <iostream>
#include <string>
#include <string.h>

using namespace std;


const int EXT_COEF = 2;
const int MIN_CAP = 10;

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

struct TNode{
    TNode(string new_str_key, uint64_t new_val){
        str_key = new_str_key;
        val = new_val;
    }

    string str_key;
    uint64_t val;
    TNode* refs[2];
    int idx; // letter_position * BIT_COUNT + bit_position

};

int main(){
    TMy_vector<TNode*> vector;
    TNode nod("hui", 11);
    vector.Push_back(&nod);
    cout<<vector[0]->str_key;

}