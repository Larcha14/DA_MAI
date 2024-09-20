#pragma once
#include <iostream>

template <class T, class U>
class TMy_pair{
    public:
        TMy_pair() = default;
        TMy_pair& operator=(const TMy_pair& other){
            first = other.first;
            second = other.second;
            return *this;
        }

        TMy_pair(T el_1, U el_2){
            first = el_1;
            second = el_2;
        }

        ~TMy_pair() = default;

        T first;
        U second;
};

