#! /usr/bin/env python3

import sys
import random
import string

MIN_PROBABILITY = 0.1
MAX_PROBABILITY = 0.99
PROBABILITY_SET = 0.6

test_dir = 'tests'
benchmark_dir = 'prod_tests'
TEST_CNT= 4


def generate_probability():
    return round(random.uniform(MIN_PROBABILITY, MAX_PROBABILITY), 2)

def generate_idx(val_cnt):
    return random.randint(0, val_cnt)

def generate_val():
    return random.randint(0,2**64 -1)    
string.ascii_lowercase

def generate_key():
    key_len = random.randint(1,256)
    key =  ''.join(random.choice(string.ascii_lowercase) for _ in range(key_len) )
    return key

def main():
    if len(sys.argv)!= 2:
        print(f"Usage: {sys.argv[0]} <test path>")
        sys.exit(1)
    test_dir = sys.argv[1]
    keys_dict = dict()
    keys_array =[]
    
    for num in range(1, TEST_CNT+1):
        cnt_values = 100* (10**num) # количество 
        actions_array = ["+", "-", "?"]
        actions_dict_cnt =dict.fromkeys(["+", "-", "?"], 0)

        filename = f"{test_dir}/{num:02}"
        with open(f"{filename}.t", 'w') as tfd, \
             open(f"{filename}.a", 'w') as afd:
            # arr = []
            while(actions_dict_cnt["+"]<cnt_values or actions_dict_cnt["-"]<cnt_values or actions_dict_cnt["?"]<cnt_values):
                action = random.choice(actions_array)
                actions_dict_cnt[action]+=1
                if action =="+":
                    key  = generate_key()
                    val = generate_val()
                    keys_array.append((key, val))
                    tfd.write(f"+ {key} {val}\n")


                    get_val = keys_dict.get(key, -1)
                    if get_val == -1:
                        afd.write(f"OK\n")
                        keys_dict[key] = val
                    else:
                        afd.write(f"Exist\n")
                        
                elif action == "-":
                    if len(keys_array)==0:
                        key  = generate_key()
                    else:
                        key  = keys_array[generate_idx(len(keys_array)-1)][0]
                    tfd.write(f"- {key}\n")

                    get_val = keys_dict.pop(key, -1)
                    if get_val ==-1:
                        afd.write(f"NoSuchWord\n")
                    else:
                        afd.write(f"OK\n")

                elif action == "?":
                    probability = generate_probability()
                    if probability <= PROBABILITY_SET and len(keys_array)>0: # вероятность попадания элемента из списка
                        key  = keys_array[generate_idx(len(keys_array)-1)][0]
                    else: 
                        key  = generate_key()

                    tfd.write(f"{key}\n")
                    get_val = keys_dict.get(key, -1)
                    if get_val == -1:
                        afd.write(f"NoSuchWord\n")
                    else:
                        afd.write(f"OK: {get_val}\n")
                
                if actions_dict_cnt[action] >=cnt_values:
                    actions_array.remove(action)
                    if len(actions_array) ==0:
                        break

                
                # tfd.write(f"{key}\t{value}\n")
        print("Test #", num, " having ", cnt_values, " elements was created ")



if __name__ == "__main__":
    main()
