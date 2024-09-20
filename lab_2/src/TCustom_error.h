#pragma once
#include <exception>
#include <string>
using namespace std;

class TCustom_error: public exception{
    public:
        TCustom_error(const string& message): message{message}
        {}
        const char* what() const noexcept override{
            return message.c_str();
        }
    private:
        string message;

};

