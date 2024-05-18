#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

const int STRING_LENGTH = 5;
const int BIT_COUNT = 5;

void str_to_char(string str_from, char* array_to, int array_size);
int first_diff_bit(char char_1, char char_2);
bool get_bit(char symbol, int bit);
void print_bits(int a);
void print_bit_mask(int a, int bit_capacity);


struct TStr{
    friend istream& operator >>(istream& is, TStr &el);
    friend ostream& operator <<(ostream& os, TStr &el);

    friend ifstream& operator >>(ifstream& is, TStr &el);
    friend ofstream& operator <<(ofstream& os, TStr &el);

    TStr(){
        val_size=0;
        first_idx = STRING_LENGTH-1;
        memset(val, '0',  sizeof(char)* STRING_LENGTH);
    };

    TStr(string str){
        memset(val, '0',  sizeof(char)* STRING_LENGTH);
        if (str.size() > STRING_LENGTH){
            val_size= STRING_LENGTH;
        } else{
            val_size = str.size();
        }
        str_to_char(str, val, STRING_LENGTH);
        first_idx = STRING_LENGTH - val_size;
    };

    TStr(const TStr &other){
        memset(val, '0',  sizeof(char)* STRING_LENGTH);
        val_size = other.val_size;
        first_idx = other.first_idx;
        for (int i=first_idx; i< STRING_LENGTH; ++i){
            val[i] = other.val[i];
        } 
    }

    TStr(TStr &&other) noexcept{
        // val = other.val;
        memcpy(val, other.val, STRING_LENGTH);
        val_size = other.val_size;
        first_idx = other.first_idx;
        // for (int i=first_idx; i< STRING_LENGTH; ++i){
        //     val[i] = other.val[i];
        // } 
    }
    

    TStr& operator=(const TStr& other){
        memset(val, '0',  sizeof(char)* STRING_LENGTH);
        val_size = other.val_size;
        first_idx = other.first_idx;
        for (int i=first_idx; i< STRING_LENGTH; ++i){
            val[i] = other.val[i];
        } 
        return *this;
    }

    operator string() const {
        return string(&val[first_idx], val_size );
    } 

    char operator[](int needed_idx){
        return this->val[needed_idx];
    } 

    int operator ==(const TStr& other) const{ // возвращает первый непохожий элемент, если строки ==, то -1
        if (this->val_size == 0 and this->val_size == other.val_size){
            return -1;
        }else if(first_idx != other.first_idx){
            return min(first_idx, other.first_idx);
        }
        
        for(int i = first_idx; i< STRING_LENGTH ; ++i){
            if (this->val[i]!= other.val[i]){
                return i;
            }
            // cout<<"Cur val: "<<this->val[i]<<" El val = "<<el.val[i]<<"\n";
        }
        return -1;
    }

    int size(){
        return val_size;
    }

    int get_first_idx(){
        return first_idx;
    }

    void print_bit_str(){
        for(int i =0; i<STRING_LENGTH;++i){
            print_bit_mask(val[i]-'a'+1, BIT_COUNT);
            cout<<".";
        }
    }

    private:
        char val[STRING_LENGTH];
        int val_size;
        int first_idx;

} ;
