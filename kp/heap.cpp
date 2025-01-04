#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <exception>
#include <algorithm>
using namespace std;


class err: public exception{
    public:
        err(const string& message): message{message}
        {}
        const char* what() const noexcept override{
            return message.c_str();
        }
    private:
        string message;

};

// template <class T>
// concept T = convertible_to<T, int> ;

const uint64_t MIN_CAP = 10; 
const int EXT_COEF = 2;

template <class T>
class heap_min{
    private:
        T* buff;
        uint64_t heap_size;
        uint64_t capasity;
        uint64_t last_idx;
        bool increase();
        bool decrease();
        void push_up(uint64_t idx);
        void push_down(uint64_t idx);
        void change(uint64_t i1, uint64_t i2);
        uint64_t parant_idx(uint64_t idx);
        uint64_t son_idx(uint64_t idx, bool right);

    public:
        heap_min();
        T pop();
        T top();
        bool push(T new_val);
        uint64_t size();
        bool empty();
        void print();
        ~heap_min();
};

template <class T>
heap_min<T>::~heap_min(){
    free(this->buff);
    heap_size = 0;
    capasity =0;
    last_idx =0;
}

template <class T>
heap_min<T>::heap_min(){
    T* tmp_buff = (T *)malloc(MIN_CAP*sizeof(T));
    if (tmp_buff==NULL)throw err("Unexpected error. Impossible to increase memmory");
    this->buff = tmp_buff;
    this->buff[0]=0;
    this->heap_size = 0;
    this->capasity = MIN_CAP;
    this->last_idx = 0;
}

template <class T>
bool heap_min<T>::increase(){
    uint64_t new_cap = this->capasity*EXT_COEF;
    T* tmp_buff = (T*)realloc(this->buff, new_cap*sizeof(T));
    if (tmp_buff == NULL) return false;
    buff = tmp_buff;
    this->capasity = new_cap;
    return true;
}

template <class T>
bool heap_min<T>::decrease(){
    if (this->capasity <=MIN_CAP or this->heap_size > (this->capasity/(EXT_COEF*EXT_COEF))) return true;
    uint64_t new_cap = capasity/EXT_COEF;
    T * tmp_buff = (T*)realloc(this->buff, new_cap*sizeof(T));
    if (tmp_buff==NULL){
        return false;
    }
    this->buff = tmp_buff;
    this->capasity = new_cap;
    return true;
}

template <class T>
uint64_t heap_min<T>::size(){
    return this->heap_size;
}

template <class T>
bool heap_min<T>::empty(){
    return heap_size==0;
}

template <class T>
T heap_min<T>::top(){
    return buff[0];
}

template <class T>
uint64_t heap_min<T>::parant_idx(uint64_t idx){
    return (idx -1)/2;
}

template <class T>
uint64_t heap_min<T>::son_idx(uint64_t idx, bool right){
    return idx*2+1+right;
}

template <class T>
void heap_min<T>::change(uint64_t i1, uint64_t i2){
    T tmp = this->buff[i1];
    this->buff[i1] = this->buff[i2];
    this->buff[i2] = tmp;
}

template <class T>
void heap_min<T>::push_up(uint64_t idx){
    if (idx ==0) return;
    uint64_t i_p = parant_idx(idx);
    if (this->buff[i_p] >= this->buff[idx] ) return;
    change(i_p, idx);
    push_up(i_p);
}

template <class T>
void heap_min<T>::push_down(uint64_t idx){   
    uint64_t ls = son_idx(idx, 0);
    uint64_t rs = son_idx(idx, 1);
    if (ls > this->heap_size and rs > this->heap_size){
        return;
    } else if(ls > this->heap_size){
        change(idx, rs);
        return;
    } else if (rs > this->heap_size){
        change(idx, ls);
        return;
    }
    if(this->buff[ls] <= this->buff[idx] and this->buff[rs]<= this->buff[idx]) return;
    if (this->buff[ls]> this->buff[rs]){
        change(idx, ls);
        push_down(ls);
    } else{
        change(idx, rs);
        push_down(rs);
    }
}

template <class T>
T heap_min<T>::pop(){
    if(this->empty()) {
        throw err("Impossible to erase element. Heap is empty");
    }
    T answ = this->top();
    this->heap_size --;
    change(0, heap_size);
    push_down(0);
    if(!decrease())throw err("Unexpected error. Impossible to decrease memmory");
    return answ;
}

template <class T>
bool heap_min<T>::push(T new_val){
    buff[this->heap_size] = new_val;
    push_up(this->heap_size);
    heap_size++;
    if (this->heap_size ==this->capasity-1){
        if(!increase()) throw err("Unexpected error. Impossible to increase memmory");
    }
    return true;
}

template <class T>
void heap_min<T>::print(){
    if(this->empty()){
        cout<<"Empty";
    } else{
        for(uint64_t i=0; i<this->size();++i){
            cout<<this->buff[i]<<' ';
        }
    }
    cout<<"\n";
}

int main(){
    try
    {
        heap_min<int> q;
        int tmp;
        while(cin>>tmp){
            q.push(tmp);
            cout<<"Size = "<<q.size()<<"\n";
        }
        q.print();
        q.pop();

        cout<<"\nTop = "<<q.top()<<'\n';
        cout<<"Size = "<<q.size()<<"\n";
    }
    catch(const err e)
    {
        std::cerr << e.what() << '\n';
    }
    
}