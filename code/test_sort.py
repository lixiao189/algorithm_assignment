from os import system
from os import popen
from random import randint


def execCmd(cmd):
    # execute command, and return the output
    r = popen(cmd)
    text = r.read()
    r.close()
    return text[:-1]


for i in range(6):
    with open('./data/test_input', 'w') as f:
        # generate data
        n = randint(10 ** i, 10 ** i + 10)
        f.write('{}\n'.format(n))

        for k in range(n):
            number = randint(10, 40)
            f.write('{}\n'.format(number))

    print("n = 10 ^ {}".format(i))

    # Test bubble sort
    print("Bubble sort:")
    execCmd('clang++ bubble.cpp -o prog')
    print(execCmd('time ./prog < ./data/test_input'))

    # Test merge sort
    print("Merge sort:")
    execCmd('clang++ merge.cpp -o prog')
    print(execCmd('time ./prog < ./data/test_input'))

    print("----------------------------")
