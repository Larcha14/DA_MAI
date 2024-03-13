#! /usr/bin/env python3

import sys
import random
import string

test_dir = 'tests'
benchmark_dir = 'prod_tests'
TEST_CNT= 10


CNT_TEST_PER_FILE = 1000

def generate_kv():
    key = random.randint(0,65535)
    r_v = random.randint(0,64)
    value =  ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(r_v) )
    return key, value

def main():
    if len(sys.argv)!= 2:
        print(f"Usage: {sys.argv[0]} <test path>")
        sys.exit(1)
    test_dir = sys.argv[1]
    for num in range(1, TEST_CNT+1):
        filename = f"{test_dir}/{num:02}"
        with open(f"{filename}.t", 'w') as tfd, \
             open(f"{filename}.a", 'w') as afd:
            arr = []
            for _ in range(CNT_TEST_PER_FILE):
                key, value = generate_kv()
                arr.append((key, value))
                tfd.write(f"{key}\t{value}\n")
            # print(key, value)
            arr.sort(key=lambda x: x[0])
            for _ in range(len(arr)-1):
                if len(arr[_][1]) != 0:
                    afd.write(f"{arr[_][0]}\t{arr[_][1]}\n")
            if len(arr[len(arr)-1][1]) != 0:
                afd.write(f"{arr[len(arr)-1][0]}\t{arr[len(arr)-1][1]}")
    for num in range(3):
            n = 10**(3+num)
            filename= f"{benchmark_dir}/{num:02}"
            with open(f"{filename}.txt", 'w') as tfd:
                for _ in range(n):
                    key, value = generate_kv()
                    tfd.write(f"{key}\t{value}\n")


if __name__ == "__main__":
    main()
