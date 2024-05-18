#include "TPatricia.h"

int main(){
    Patricia_trie::TTrie trie;
    string request;
    while(cin>>request){
        // cin>>request;
        switch (request[0]){
            case '=':{
                cout<<trie.size()<<'\n';
                break;
            }
            case '@':{
                trie.print_trie();
                break;
            }
            case '+':{
                TStr word;
                uint64_t val;
                cin>>word>>val;
                // cout<<"\nThis is +\n";
                if(trie.insert(word, val)){
                    cout<<"OK";
                } else{
                    cout<<"Exist";
                }
                break;
            }
            case '-':{
                TStr word;
                cin>>word;
                try
                {
                    if (trie.erase(word)){
                        cout<<"OK";
                    }else{
                        cout<<"NoSuchWord";
                    }
                }
                catch(const TCustom_error& e)
                {
                    cout << e.what();
                }
                break;
            }
            case '!':{
                string cmd, path;
                cin>>cmd>>path;
                if (cmd=="Save"){
                    trie.save_trie(path);
                } else if (cmd=="Load"){
                    trie.load_trie(path);
                }
                // Загружаем/выгружаем словарь
                cout << "OK";
                break;
            }
            default:
                TStr word = request;
                TMy_pair<bool, uint64_t> answ_pair = trie.search(word);
                if (answ_pair.first){
                    cout<<"OK: "<<answ_pair.second;
                } else{
                    cout<<"NoSuchWord";
                }

                break; 
            
        } 
        cout<<'\n';
    }

    // Patricia_trie::TTrie trie;
    // string input_str;
    // TStr tmp_str("Hui");
    // TStr s2("HUU");
    // TStr s3("akkkk");
    // while (cin>>input_str){
    //     if (input_str=="add"){
    //         uint64_t val_tmp;
    //         cin>>input_str;
    //         TStr tmp_str(input_str);
    //         cin>>val_tmp;
    //         trie.insert(tmp_str, val_tmp);
    //         // trie.print_trie();
    //         // cout<<"Size = "<<trie.size()<<'\n';          
    //     } else if(input_str == "del"){
    //         cin>>input_str;
    //         TStr tmp_str(input_str);
    //         trie.erase(tmp_str);
    //     } else if(input_str== "print"){
    //         trie.print_trie();
    //         cout<<"Size = "<<trie.size()<<'\n'; 
    //     } else {
    //         break;
            
    //     }

    // }

}
