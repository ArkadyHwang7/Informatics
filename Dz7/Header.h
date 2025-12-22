#pragma once

#ifdef DLL1_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif

extern "C" {
    MYLIBRARY_API double count_matches(
        int* v1,
        int* v2,
        int size,
        int N
    );

}
