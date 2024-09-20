#include "TStr.h"

istream& operator>>(istream& is, TStr &el){
    string str;
    // getline(cin, str);
    cin>>str;
    
    if (str.size() > STRING_LENGTH){
        el.val_size= STRING_LENGTH;
    } else{
        el.val_size = str.size();
    }
    el.first_idx = STRING_LENGTH - el.val_size;
    str_to_char(str, el.val, STRING_LENGTH);
    
    return is;

}

ostream& operator <<(ostream& os, TStr &el){
    os<<string(&el.val[el.first_idx], el.val_size );
    return os;
}

ifstream& operator >>(ifstream& in, TStr &el){
    string str;
    
    // getline(cin, str);
    in>>str;
    
    if (str.size() > STRING_LENGTH){
        el.val_size= STRING_LENGTH;
    } else{
        el.val_size = str.size();
    }
    el.first_idx = STRING_LENGTH - el.val_size;
    str_to_char(str, el.val, STRING_LENGTH);
    
    return in;
}

ofstream& operator <<(ofstream& os, TStr &el){
    os<<string(&el.val[el.first_idx], el.val_size );
    return os;
}

void str_to_char(string str_from, char* array_to, int array_size){
    int j=str_from.size();
    for(int i= array_size-1; i>=0; --i){
        if (j==0){
            return;
        }
        j--;
        array_to[i] = tolower(str_from[j]);
    }
}

int first_diff_bit(char char_1, char char_2){
    int bit_mask_1, bit_mask_2;
    if (char_1 == char_2) return -1;

    if (char_1 == '0'){
        bit_mask_1 = 0;
    }  else{
        bit_mask_1 = char_1 - 'a'+1;
    }

    if (char_2 =='0'){
         bit_mask_2 =0;
    } else{
         bit_mask_2 =char_2 - 'a' +1;
    }


    int diff = bit_mask_1 ^ bit_mask_2;
    int bit_count=0;
    while(diff > 1){
        diff = diff>>1;
        ++bit_count;
    }
    //выводим биты слева-направо
    return BIT_COUNT- bit_count-1;
}


bool get_bit(char symbol, int bit){
    int symbol_idx;
    if (symbol == '0'){
        symbol_idx = 0;
    } else {
        symbol_idx = symbol - 'a' +1 ;
    }
    bit = BIT_COUNT - bit - 1;
    return  symbol_idx &(1<<bit);
     
}

void print_bits(int a){
    if (a/2 >0){
        print_bits(a/2);
    }
    cout<<a%2;
}


void print_bit_mask(int a, int bit_capacity) {
    // cout<<a<<" = ";
    int idx = bit_capacity-1;
    while((a & (1<<idx))==0 and idx >= 0){
        idx--;
        cout<<"0";
    }
    print_bits(a);
    // cout<<"\n";
}

// int main(){
//     // cout<<"First diff bit = "<<first_diff_bit('h', 0);
//     cout<<"Get bit : "<<get_bit('h'-'a' +1, 0);
// }