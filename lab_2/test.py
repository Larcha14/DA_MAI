#! /usr/bin/env python3

import sys
import random
import string

test_dir = 'tests'
benchmark_dir = 'prod_tests'
TEST_CNT= 10

actions_array = ["+", "-", "?"]

CNT_TEST_PER_FILE = 1000

def generate_val():
    return random.randint(0,2**64 -1)    

def generate_key():
    key_len = random.randint(1,256)
    key =  ''.join(random.choice(string.ascii_letters) for _ in range(key_len) )
    return key

def main():
    if len(sys.argv)!= 2:
        print(f"Usage: {sys.argv[0]} <test path>")
        sys.exit(1)
    test_dir = sys.argv[1]
    keys_dict = {}
    
    for num in range(1, TEST_CNT+1):
        filename = f"{test_dir}/{num:02}"
        with open(f"{filename}.t", 'w') as tfd, \
             open(f"{filename}.a", 'w') as afd:
            # arr = []
            for _ in range(CNT_TEST_PER_FILE):
                action = random.choice(actions_array)
                if action =="+":
                    key  = generate_key()
                    val = generate_val()
                    tfd.write(f"+ {key} {val}\n")
                    get_val = keys_dict.get(key, -1)
                    if get_val == -1:
                        afd.write(f"OK\n")
                        keys_dict[key] = val
                    else:
                        afd.write(f"Exist\n")
                        
                elif action == "-":
                    key  = generate_key()
                    tfd.write(f"- {key}\n")
                    get_val = keys_dict.pop(key, -1)
                    if get_val ==-1:
                        afd.write(f"NoSuchWord\n")
                    else:
                        afd.write(f"OK\n")
                elif action == "?":
                    key  = generate_key()
                    tfd.write(f"{key}\n")
                    get_val = keys_dict.get(key, -1)
                    if get_val == -1:
                        afd.write(f"NoSuchWord\n")
                    else:
                        afd.write(f"OK: {get_val}\n")

                
                # tfd.write(f"{key}\t{value}\n")


if __name__ == "__main__":
    main()
