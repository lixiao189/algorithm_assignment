from os import system
from os import popen
from random import randint


def execCmd(cmd):
    # execute command, and return the output
    r = popen(cmd)
    text = r.read()
    r.close()
    return text[:-1]


for i in range(100):
    with open('./data/test_input', 'w') as f:
        # generate data
        n, m = randint(5, 10), randint(2, 10)
        f.write('{} {}\n'.format(n, m))

        for i in range(n):
            p, w = randint(10, 40), randint(5, 10)
            f.write('{} {}\n'.format(p, w))

        for i in range(m):
            W = randint(5, 10)
            f.write('{}\n'.format(W))

    # Test greedy
    execCmd('clang++ greedy.cpp -o prog')
    result1 = int(execCmd('./prog < ./data/test_input'))
    print('greedy ok')

    # Test neighborhood search
    execCmd('clang++ improving_search.cpp -o prog')
    result2 = int(execCmd('./prog < ./data/test_input'))
    print('improving_search ok')

    # Test tabu search
    execCmd('clang++ tabu.cpp -o prog')
    result3 = int(execCmd('./prog < ./data/test_input'))
    print('tabu ok')

    if (result1 < result2 and result2 < result3):
        print(result1, result2, result3)
        print("-------------")
    if (result1 > result2 or result2 > result3 or result1 > result3):
        print("Wrong Answer")
        break
