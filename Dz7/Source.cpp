#include "pch.h"
#include "Header.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

extern "C" MYLIBRARY_API double count_matches(
    int* v1,
    int* v2,
    int size,
    int N
) {
    clock_t start_time = clock();

    for (int i = 0; i < N; ++i) {
        volatile int match_count = 0;

        for (int j = 0; j < size; ++j) {
            if (v1[j] == v2[j]) {
                match_count++;
            }
        }
    }

    clock_t end_time = clock();

    double elapsed_time =
        double(end_time - start_time) / CLOCKS_PER_SEC;

    return elapsed_time;
}