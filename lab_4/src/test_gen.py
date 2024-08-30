#! /usr/bin/env python3

import sys
import random
import string

# MIN_TEXT_SIZE = 10000
# MAX_TEXT_SIZE = 10**6

MIN_TEXT_SIZE_DEGREE = 4
MAX_TEXT_SIZE_DEGREE = 6

MIN_PATTERN_SIZE = 3
MAX_PATTERN_SIZE = 9000

MIN_VAlUE = 0
MAX_VALUE = 2**32 -1

MIN_PROBABILITY = 0.1
MAX_PROBABILITY = 0.97

MIN_LINE_SIZE = 10
MAX_LINE_SIZE = 100

def array_create(el_1, el_2, cnt, str_name):
    a =[el_1]

    if cnt != 0:
        add_sum  = round((el_2 - el_1) / cnt, 2)
        for i in range(cnt):
            a.append(round(a[i]+add_sum, 2))
    print("Array ", str_name, ": ", a)
    return a

def int_array_create(el_1, el_2, cnt,  str_name):
    a =[el_1]

    if cnt != 0:
        add_sum  = int((el_2 - el_1) / cnt)
        for i in range(cnt):
            a.append(int(a[i]+add_sum))
    print("Array ", str_name, ": ", a)
    return a


def test_gen(size_p_range, size_t_range, probability_range, dir_name, start_idx):
    p_1, p_2, p_3 = size_p_range
    t_1, t_2, t_3 = size_t_range
    prob_1, prob_2, prob_3 = probability_range

    p_sizes_values = int_array_create(p_1, p_2, p_3, "'Pattern sizes'")
    t_sizes_values = int_array_create(t_1, t_2, t_3, "'Text sizes'")
    prob_values = array_create(prob_1, prob_2, prob_3, "'Probability'")


    patterns =[]
    for i in p_sizes_values:
        tmp_pattern = []
        for j in range(i):
            tmp_pattern.append(random.randint(MIN_VAlUE, MAX_VALUE))
        patterns.append(tmp_pattern)
    # print("Patterns: ", patterns)

    test_cnt = len(p_sizes_values)*len(t_sizes_values)*len(prob_values)
    idx = start_idx
    for i_1 in range(len(p_sizes_values)):
        for i_2 in range(len(t_sizes_values)):
            for i_3 in range(len(prob_values)):
                
                test_file_name = f"{dir_name}/{idx:02d}"
                

                pattern_size = p_sizes_values[i_1]
                text_size = t_sizes_values[i_2]
                probability = prob_values[i_3]

                print(test_file_name, "was created; Cnt of el = ", text_size)

                with open("{0}.t".format(test_file_name), 'w') as output_file, \
                        open("{0}.a".format(test_file_name), "w") as answer_file:
                    for el in patterns[i_1]:
                        output_file.write(f"{el} ")
                    output_file.write(f"\n")

                    total_words_cnt =0
                    diff =0
                    diff_buffer =[]
                    while total_words_cnt < text_size:
                        line_word_cnt = diff
                        if total_words_cnt + line_word_cnt >= text_size:
                            break
                        buffer = diff_buffer.copy()
                        diff_buffer = []
                        next_break = random.randint(MIN_LINE_SIZE, MAX_LINE_SIZE)
                        
                        while (line_word_cnt < next_break):
                            tmp_val = random.randint(0, MAX_LINE_SIZE-line_word_cnt)
                            for _ in range(tmp_val):
                                buffer.append(random.randint(MIN_VAlUE, MAX_VALUE))
                            line_word_cnt+= tmp_val

                            if probability >= round(random.random(), 2):
                                for _ in range(pattern_size):
                                    buffer.append(patterns[i_1][_])
                                line_word_cnt += pattern_size
                        
                        for _ in range(next_break):
                            output_file.write(f"{buffer[_]} ")
                        output_file.write(f"\n")

                        if line_word_cnt > next_break:
                            diff = line_word_cnt - next_break
                            for _ in range(diff):
                                diff_buffer.append(buffer[next_break+_])
                        else:
                            diff =0
                        total_words_cnt += next_break

                    for _ in range(diff):
                        output_file.write(f"{diff_buffer[_]} ")
                    output_file.write(f"\n")
                                
                    idx+=1
                    

    # for el in range(test_cnt):
    #     test_file_name = f"{dir_name}/{idx:02d}"
    #     print(test_file_name, "was created")

    #     with open("{0}.t".format(test_file_name), 'w') as output_file, \
    #             open("{0}.a".format(test_file_name), "w") as answer_file:
    #         output_file.write(f"{' '.join(patterns[])}\n")


    #     idx +=1
    

    return start_idx+test_cnt



def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <test dir>")
        sys.exit(1)
    
    test_dir = sys.argv[1]
    
    p_size_const = random.randint(MIN_PATTERN_SIZE, MAX_PATTERN_SIZE)
    t_size_const = 10**random.randint(MIN_TEXT_SIZE_DEGREE, MAX_TEXT_SIZE_DEGREE)
    probability_const = round(random.uniform(MIN_PROBABILITY, MAX_PROBABILITY), 2)

    print("P_size = ", p_size_const)
    print("T_size = ", t_size_const)
    print("Probability = ", probability_const)


    cur_test_idx = 1
    # Изменяем только вероятность выпадения
    print()
    print("=====Test Probability======")
    cur_test_idx = test_gen((p_size_const, p_size_const, 0), (t_size_const, t_size_const, 0), (MIN_PROBABILITY, MAX_PROBABILITY, 4), test_dir, cur_test_idx )
    # Изменяем только размер паттерна
    print()
    print("=====Test Pattern size change======")
    cur_test_idx = test_gen((MIN_PATTERN_SIZE, MAX_PATTERN_SIZE, 4), (t_size_const, t_size_const, 0), (probability_const, probability_const , 0), test_dir, cur_test_idx )
    # Изменяем только размер текста
    print()
    print("=====Test Text size change======")
    cur_test_idx = test_gen((p_size_const, p_size_const, 0), (10**MIN_TEXT_SIZE_DEGREE, 10**MAX_TEXT_SIZE_DEGREE, 10), (probability_const, probability_const , 0), test_dir, cur_test_idx )

if __name__ == "__main__":
    main()