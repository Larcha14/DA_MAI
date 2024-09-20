#pragma once
#include <iostream>
#include "TStr.h"
#include "TPair.h"
#include "TCustom_error.h"
#include <fstream>

namespace Patricia_trie{

    int make_idx(int letter_pos, int bit_pos);
    TMy_pair<int, int> get_idx(int idx);

    struct TNode{
        TNode(TStr &new_str_key, uint64_t new_val){
            str_key = new_str_key;
            val = new_val;
        }

        TStr str_key;
        uint64_t val;
        TNode* refs[2];
        int idx; // letter_position * BIT_COUNT + bit_position

    };
    

    class TTrie{
        public:
            TTrie();
            TNode* get_root();
            // bool is_in_struct(TStr find_key);
            TMy_pair<bool, uint64_t> search(TStr needed_key);
            bool insert(TStr insert_key, uint64_t insrt_val);
            bool erase(TStr erase_key);
            void save_trie(string & path);
            void load_trie(string & path);
            void print_trie();
            int size();
            ~TTrie();
            
        private:
            TNode* root;
            int elem_count;
            void destroy_nods(TNode* start_node);
            void destroy_tree();
            bool add(TNode* new_node);
            TNode* get_node(TStr find_key, TNode* start_node );
            TMy_pair <TNode*, TNode*>  find_node(TNode *needed_node);
            TMy_pair<TNode*, TNode*> find_pair_for_insert(TNode *needed_node);
            void print_nodes(TNode* node, int prev_idx);
            TMy_pair<TNode*, TMy_pair<TNode*, TNode*>>  find_PRC(TStr &needed_str);
            void save_nodes(TNode* cur_node, ofstream &file_to);
            void write_node_to_file(TNode* cur_node, ofstream &file_to);
            void load_nodes(TNode* cur_node, ifstream &stream);
            void read_node_from_file(ifstream &stream);

            void del_node(TNode* node);
            void check_root();
            void root_create();

    };

};