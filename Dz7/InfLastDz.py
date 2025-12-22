import ctypes
from ctypes import CDLL
import random
import time

dll = CDLL('./Dll1.dll')


def python_count_matches(a, b, n):
    start = time.time()
    for _ in range(n):
        sum(1 for x, y in zip(a, b) if x == y)
    return time.time() - start


def cpp_count_matches(a, b, size, n):
    func = dll.count_matches
    func.restype = ctypes.c_double
    func.argtypes = [
        ctypes.POINTER(ctypes.c_int),
        ctypes.POINTER(ctypes.c_int),
        ctypes.c_int,
        ctypes.c_int
    ]

    a_c = (ctypes.c_int * size)(*a)
    b_c = (ctypes.c_int * size)(*b)

    return func(a_c, b_c, size, n)


if __name__ == "__main__":
    SIZE = 1000
    TESTS = [10000, 50000, 100000]

    a = [random.randint(0, 50) for _ in range(SIZE)]
    b = [random.randint(0, 50) for _ in range(SIZE)]

    print("№| Итерации | C++ (с) | Python (с)")
    for idx, i in enumerate(TESTS, start=1):
        print(
            f"{idx:<2} | "
            f"{i:<9} | "
            f"{cpp_count_matches(a, b, SIZE, i):<7.4f} | "
            f"{python_count_matches(a, b, i):<7.4f}"
        )
