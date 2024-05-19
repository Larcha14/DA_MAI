#include "TPatricia.h"

typedef Patricia_trie::TNode TNode;

int Patricia_trie::make_idx(int letter_pos, int bit_pos){
    return letter_pos*BIT_COUNT + bit_pos; 
}

TMy_pair<int, int> Patricia_trie::get_idx(int idx){
        TMy_pair<int, int> return_idx;
        return_idx.first = idx/BIT_COUNT;
        return_idx.second = idx%BIT_COUNT;
        return return_idx;
}


Patricia_trie::TTrie::TTrie(){
    elem_count=0;
    root_create();
}

void Patricia_trie::TTrie::root_create(){
    root = (TNode*)malloc(sizeof(TNode));
    if (root == NULL){
        throw TCustom_error("Unable to create node");
    }
    TStr tmp_str;
    root->str_key = tmp_str;
    root->refs[0] = root;
    root->refs[1] = NULL;
    root->idx = -1;
    cout<<"[INFO] add root: "<<root->str_key<<" Val = "<<root->val<<'\n';
}

void Patricia_trie::TTrie::check_root(){
    if (root==NULL) root_create();
}

void Patricia_trie::TTrie::del_node(TNode* node){
    if(node != NULL){
        cout<<"[INFO] del node: "<<node->str_key<<" Val = "<<node->val<<'\n';
        free(node);
        node = NULL;
    }
}

TMy_pair< TNode*, TNode*> Patricia_trie::TTrie::find_node(TNode *needed_node){
    TMy_pair<TNode*, TNode*> pair;
    TNode *cur_node = root;
    TNode *prev_node = root;
    bool next_leave =0;

    while( cur_node->idx < cur_node->refs[next_leave]->idx){
        cout<<"\tCur node = "<<cur_node->str_key<<'\n';
        TMy_pair<int, int> idx_pair;
        cur_node = cur_node->refs[next_leave];
        idx_pair = get_idx(cur_node->idx);
        char check_letter = needed_node->str_key[idx_pair.first];
        cout<<"idx = "<<idx_pair.first<<" "<<idx_pair.second<<" letter "<<check_letter<<"\n";
        next_leave = get_bit(check_letter , idx_pair.second);

    }
    cout<<"\tCur node = "<<cur_node->str_key<<'\n';
    cout<<"leave = "<<next_leave<<" el ="<<cur_node->refs[next_leave]->str_key<<"\n";
    pair.first = cur_node;
    pair.second = cur_node->refs[next_leave];
    // return cur_node->refs[next_leave];
    return pair;
}

TMy_pair<TNode*, TNode*> Patricia_trie::TTrie::find_pair_for_insert (TNode *needed_node){
    TNode *prev_node = root;
    TNode *cur_node = root;
    TMy_pair<TNode*, TNode*> nodes;
    bool next_leave =0;
    cout<<"\nNeeded node: "<<needed_node->idx<<"\n";
    while( cur_node->idx < cur_node->refs[next_leave]->idx and cur_node->refs[next_leave]->idx <= needed_node->idx){
        TMy_pair<int, int> idx_pair;
        cur_node = cur_node->refs[next_leave];
        idx_pair = get_idx(cur_node->idx);
        char check_letter = needed_node->str_key[idx_pair.first];
        next_leave = get_bit(check_letter , idx_pair.second);
        prev_node = cur_node;

    }
    
    nodes.first = cur_node;
    nodes.second = cur_node->refs[next_leave];

    return nodes;   
}

TMy_pair<TNode*, TMy_pair<TNode*, TNode*>>  Patricia_trie::TTrie::find_PRC(TStr &needed_str){
    TNode* prev_node = root;
    TNode* cur_node = root;
    TNode* prev_prev_node = root;
    bool next_leave=0;
    

    while( cur_node->idx < cur_node->refs[next_leave]->idx){
        TMy_pair<int, int> idx_pair;
        cur_node = cur_node->refs[next_leave];
        idx_pair = get_idx(cur_node->idx);
        char check_letter = needed_str[idx_pair.first];
        next_leave = get_bit(check_letter , idx_pair.second);

        prev_prev_node = prev_node;
        prev_node = cur_node;

    }

    // prev_node = cur_node;
    cur_node = cur_node->refs[next_leave];

    TMy_pair<TNode*, TNode*> nodes_cur;
    TMy_pair<TNode*, TMy_pair<TNode*, TNode*>>  all_nodes;
    nodes_cur.first = prev_node;
    nodes_cur.second = cur_node;
    all_nodes.first = prev_prev_node;
    all_nodes.second = nodes_cur;
    

    return all_nodes;
}

bool Patricia_trie::TTrie::erase(TStr needed_key){
    check_root();
    if (this->elem_count == 0 or root==NULL){
        // throw TCustom_error("Unable to del node. Trie is empty");
        throw TCustom_error("NoSuchWord");
    }
    TMy_pair<TNode*, TMy_pair<TNode*, TNode*>>  all_nodes = find_PRC(needed_key);
    
    if ((needed_key == all_nodes.second.second->str_key)!=-1){
        return false;
    }

    // Root del
    if (this->elem_count == 1){
        cout<<"\n[INFO] del root: "<<root->str_key<<" val = "<<root->val<<"\n";
        // free(root);
        elem_count=0;
        root->val=0;
        root->str_key = TStr();
        // root==NULL;
        return true;
    } 
    
    TNode* cur_node = all_nodes.second.second; // X
    TNode* prev_node = all_nodes.second.first; // Q
    TNode* prev_prev_node = all_nodes.first; // M
    cout<<"Path to del: \n\tcur node = "<<all_nodes.second.second->str_key<<"\n\tprev_node = "<<all_nodes.second.first->str_key<<"\n\tprev_prev_node = "<<all_nodes.first->str_key<<"\n";

    // if (cur_node == root){
    //     root->str_key = prev_node->str_key;
    //     root->val = prev_node->val;
    //     if( prev_prev_node->refs[0]==prev_node) prev_prev_node->refs[0] = root
    // }

    if (prev_node==cur_node){
        bool return_leave, new_leave;

        if (cur_node->refs[1]==cur_node) return_leave = 1; 
        else return_leave = 0;

        if (prev_prev_node->refs[1]==cur_node) new_leave = 1;
        else new_leave = 0;

        prev_prev_node->refs[new_leave] = cur_node->refs[!return_leave];
        cout<<"\n[INFO] del elem: "<<cur_node->str_key<<" val = "<<cur_node->val<<"\n";
        // free(cur_node);
        del_node(cur_node);
        elem_count--;
        return true;
    }
    cout<<"-------\n";
    cout<<"Cur(X) = "<<cur_node->str_key<<'\n';
    cout<<"Prev(Q) = "<< prev_node ->str_key <<'\n';
    cout<<"Prev prev(M) = "<<prev_prev_node->str_key<<'\n';
    cout<<"-------\n";



    bool return_leave, parent_leave;
    TMy_pair< TNode*, TNode*> prev_return = find_node(prev_node);


    if (prev_node->refs[1]==cur_node) return_leave =1;
    else return_leave =0;

    cout<<"My pizduk = "<<prev_node->refs[return_leave]->str_key<<"  return leave = "<<return_leave<<'\n';

    if (prev_prev_node->refs[0]==prev_node) parent_leave =0;
    else parent_leave = 1;

    cout<<"P = "<<prev_return.first->str_key<< "  some hmyr = "<<prev_return.second->str_key<<"\n";

    if (prev_return.first->refs[0]==prev_node) prev_return.first->refs[0] = cur_node;
    else prev_return.first->refs[1] = cur_node;


    // if (prev_node->refs[1]==cur_node) return_leave =1;
    // else return_leave =0;
    // if (prev_prev_node->refs[0]==prev_node) parent_leave =0;
    // else parent_leave = 1;

    prev_prev_node->refs[parent_leave]= prev_node->refs[!return_leave];
    cout<<"Parent leave = "<<parent_leave<<"\n";
    cout<<"Return leave = "<<return_leave<<"\n";

    cur_node->str_key = move(prev_node->str_key);
    cur_node->val = prev_node->val;
    // free(prev_node);
    del_node(cur_node);

    elem_count--;
    return true;
    
}

bool Patricia_trie::TTrie::add(TNode* new_node){
    TNode* check_node = find_node(new_node).second;
    // TMy_pair<TNode*, TNode*> check_node = find_node(new_node);
    int diff_letter = check_node->str_key == new_node->str_key;
    if (diff_letter == -1){
        return false;
    } else {
        cout<<"\nDiff letter: i="<< diff_letter<<" check "<<check_node->str_key[diff_letter]<<'\n';
        int diff_bit = first_diff_bit(new_node->str_key[diff_letter], check_node->str_key[diff_letter]);
        cout<<"\nDiff bit: i="<< diff_bit<<" check "<<'\n';
        new_node->idx = make_idx(diff_letter, diff_bit);
        TMy_pair<TNode*, TNode*> nodes = find_pair_for_insert(new_node);
        cout<<"Pair: prev = "<<nodes.first->str_key<<" cur = "<< nodes.second->str_key<<"\n";
        if (nodes.first->refs[0]==nodes.second){
            nodes.first->refs[0]= new_node;
        } else{
            nodes.first->refs[1]= new_node;
        }

        TMy_pair<int, int> idx_pair;
        idx_pair = get_idx(new_node->idx);
        cout<<"IDX pair: "<<idx_pair.first<<" ; "<<idx_pair.second<<'\n';
        char check_letter = new_node->str_key[idx_pair.first];
        bool check_bit = get_bit(check_letter , idx_pair.second);

        new_node->refs[check_bit] = new_node;
        new_node->refs[!check_bit] = nodes.second;
        cout<<"\n=========================\n";
        cout<<"New_node: ";
        if (new_node == NULL){
            cout<<"NULL\n";
        } else{
            cout<<" str = "<<new_node->str_key<<" val="<<new_node->val<<" idx= "<<new_node->idx<<"\n";
            // print_bit_mask(STRING_LENGTH*BIT_COUNT - new_node->idx,    STRING_LENGTH*BIT_COUNT);

        }
        cout<<"Childs: 0 = ";
        if (new_node->refs[0]!=NULL){
            cout<<new_node->refs[0]->str_key;
        } else {
            cout<<"NULL";
        }
        cout<<"\n\t1 = ";
        if (new_node->refs[1]!=NULL){
            cout<<new_node->refs[1]->str_key;
        } else {
            cout<<"NULL\n";
        }
        cout<<"\n=========================\n";
        return true;
    }
}

bool Patricia_trie::TTrie::insert(TStr insert_key, uint64_t insert_val){
    if (elem_count ==0 ){
        check_root();
        root->str_key= insert_key;
        root->val=insert_val;
        elem_count++;
        // cout<<"[INFO] change root: "<<root->str_key<<" new Val = "<<root->val;
        cout<<"\n=========================\n";
        cout<<"New_node: ";
        cout<<" str = "<<root->str_key<<" val="<<root->val<<" idx= "<<root->idx<<'\n';
        cout<<"\n=========================\n";
        return true;
        
    }
    TNode* new_node= (TNode*)malloc(sizeof(TNode));
    new_node->val = insert_val;
    new_node->str_key= insert_key;
    if (!add(new_node)){
        return false;
    }
    elem_count++; 
    return true;
}

TMy_pair<bool, uint64_t> Patricia_trie::TTrie::search(TStr neded_key){
    check_root();
    TMy_pair<bool, uint64_t> answ_pair(false, 0);
    TNode* tmp_node = (TNode*)malloc(sizeof(TNode));
    tmp_node->str_key = neded_key;

    TNode* cur_node = find_node(tmp_node).second;


    if ((cur_node->str_key == neded_key) ==-1){
        answ_pair.first = true;
        answ_pair.second=cur_node->val;

    }
    // free(tmp_node);
    del_node(cur_node);
    return answ_pair;
}

TNode* Patricia_trie::TTrie::get_root(){
    return root;
}

int Patricia_trie::TTrie::size(){
    return elem_count;
}

void Patricia_trie::TTrie::print_trie(){
    check_root();
    cout<<"\n=====================TRIE==================\n";
    cout<<"Size: "<<size()<<"\n";
    cout<<"Root: ";
    if (root!=NULL or elem_count!=0){
        cout<<" str = "<<root->str_key<<" val="<<root->val<<" idx= "<<root->idx<<'\n';
    }else {
        cout<<"NULL\n";
        return;
    }
    cout<<"\tChild : ";
    cout<<" str = "<<root->refs[0]->str_key<<" val="<<root->refs[0]->val<<" idx= "<<root->refs[0]->idx<<" bit str: ";
    // print_bit_mask(root->refs[0]->idx, BIT_COUNT*STRING_LENGTH);
    root->str_key.print_bit_str();
    cout<<'\n';
    print_nodes(root->refs[0], root->idx);
    cout<<"\n===========================================\n";
}

void Patricia_trie::TTrie::print_nodes(TNode* node, int prev_idx){
    if (node->idx<= prev_idx){
        return;
    } 
    cout<<"___________\n";
    cout<<"Node: " ;
    if (node == NULL){
        cout<<"NULL\n";
        return;
    } else{
        cout<<" str = "<<node->str_key<<" val="<<node->val<<" idx= "<<node->idx<<" bit str: ";
        node->str_key.print_bit_str();
        cout<<'\n';
    }
    cout<<"\tChild : 0 "<<" str = "<<node->refs[0]->str_key<<" val="<<node->refs[0]->val<<" idx= "<<node->refs[0]->idx<<" bit str: ";
        node->refs[0]->str_key.print_bit_str();
        cout<<'\n';
    cout<<"\tChild : 1 "<<" str = "<<node->refs[1]->str_key<<" val="<<node->refs[1]->val<<" idx= "<<node->refs[1]->idx<<" bit str: ";
        node->refs[1]->str_key.print_bit_str();
        cout<<'\n';
    print_nodes(node->refs[0], node->idx);
    print_nodes(node->refs[1], node->idx);

}


void Patricia_trie::TTrie::save_trie(string & path){
    ofstream file_to;
    file_to.open(path, ios_base::trunc | ios_base::binary);
    if (!file_to.good()){
        throw TCustom_error("Unable to open file");
    }
    file_to<<elem_count<<"\n";
    // file_to.write(elem_count, sizeof(elem_count));
    check_root();
    if (elem_count>0){
        save_nodes(root, file_to);
    }
    file_to.close();
    
}

void Patricia_trie::TTrie::save_nodes(TNode* cur_node, ofstream &file_to){
    write_node_to_file(cur_node, file_to);
    if (cur_node->refs[0]!=NULL and cur_node->idx < cur_node->refs[0]->idx) save_nodes(cur_node->refs[0], file_to);
    if (cur_node->refs[1]!=NULL and cur_node->idx < cur_node->refs[1]->idx) save_nodes(cur_node->refs[1], file_to);

}

void Patricia_trie::TTrie::write_node_to_file(TNode* cur_node, ofstream &file_to){
    file_to<<cur_node->str_key<<" "<<cur_node->val<<"\n";
}

void Patricia_trie::TTrie::load_trie(string & path){
    // this->destroy_tree();
    ifstream file_from;
    file_from.open(path, ios_base::binary);
    if (!file_from.good()){
        throw TCustom_error("Unable to open file");
    }
    int new_size;
    int cnt=0;
    file_from>>new_size;
    cout<<"Our size = "<<new_size<<"\n";
    this->destroy_tree();
    check_root();
    elem_count = 0;
    while (cnt < new_size){
        cout<<"Cur size = "<<this->size()<<"\n";
        // cout<<"Hui\n"
        read_node_from_file(file_from);
        cnt++;
    }
    // TNode* tmp = (TNode*)malloc(sizeof(TNode));
    // file_from>> tmp->str_key;
    // cout<<"Our string: "<<tmp->str_key<<"\n";
    file_from.close();
}


void Patricia_trie::TTrie::read_node_from_file(ifstream &file_from){
    string stmp;
    uint64_t val_tmp;
    // cout<<"Cur str = "<<stmp<<" _ ";
    file_from>>stmp;
    file_from>>val_tmp;
    TStr tmp_key_str(stmp);
    // cout<<"Size = "<<this->size()<<"\n";
    // if (root!= NULL) cout<<"Root ebanou suka = "<<root->str_key<<"\n";
    this->insert(tmp_key_str, val_tmp);
}

void Patricia_trie::TTrie::destroy_nods(TNode* cur_node){
    if (cur_node== NULL){
        return;
    }
    if (cur_node->refs[0]!=NULL and cur_node->idx < cur_node->refs[0]->idx) destroy_nods(cur_node->refs[0]);
    if (cur_node->refs[1]!=NULL and cur_node->idx < cur_node->refs[1]->idx) destroy_nods(cur_node->refs[1]);
    del_node(cur_node);
    return;
}



void Patricia_trie::TTrie::destroy_tree(){
    elem_count = 0;
    if (root!=NULL) destroy_nods(root->refs[0]);
    root =NULL;
}

Patricia_trie::TTrie::~TTrie(){
    destroy_tree();
}