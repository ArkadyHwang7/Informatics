#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

int Slice(std::vector<int>& vec, int down, int high)
{
    int main = vec[high];
    int i = down - 1;

    for (int j = down; j <= high - 1; j++)
    {
        if (vec[j] <= main)
        {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}

void Quick_Sort(std::vector<int>& vec, int down, int high) 
{
    if (down < high)
    {
        int main_index = Slice(vec, down, high);

        Quick_Sort(vec, down, main_index - 1);
        Quick_Sort(vec, main_index + 1, high);
    }
}

void Parallel_Quick_Sort(std::vector<int>& vec, int down, int high, int num_threads) {
    if (down < high) {
        if (num_threads <= 1 || (high - down) < 10000) {
            Quick_Sort(vec, down, high);
            return;
        }

        int main_index = Slice(vec, down, high);
        std::thread left_thread(Parallel_Quick_Sort, std::ref(vec), down, main_index - 1, num_threads / 2);
        Parallel_Quick_Sort(vec, main_index + 1, high, num_threads - (num_threads / 2));
        left_thread.join();
    }
}


std::vector<int> make_vector(int& n) {
    std::vector<int> vec(n);
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = std::rand() % 1000000;
    }
    return vec;
}

std::vector<int> make_Parallel_Quick_Sort_sort(std::vector<int>& vec, int num_threads) {
    Parallel_Quick_Sort(vec, 0, vec.size() - 1, num_threads);
    return vec;
}

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return std::make_pair(result, duration.count());
}

int main() {
    std::srand(std::time(nullptr));

    int sizes[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000 };

    for (int n : sizes) {
        std::vector<int> vec = make_vector(n);
        auto [result, execution_time] = measure_execution_time(make_Parallel_Quick_Sort_sort,std::ref(vec), 1);
        std::cout << execution_time << std::endl;
    }

    std::cout << "" << std::endl;

    for (int n : sizes) {
        std::vector<int> vec = make_vector(n);
        auto [result, execution_time] = measure_execution_time(make_Parallel_Quick_Sort_sort,std::ref(vec), 2);
        std::cout << execution_time << std::endl;
    }

    std::cout << "" << std::endl;

    for (int n : sizes) {
        std::vector<int> vec = make_vector(n);
        auto [result, execution_time] = measure_execution_time(make_Parallel_Quick_Sort_sort,std::ref(vec), 4);
        std::cout << execution_time << std::endl;
    }

    std::cout << "" << std::endl;

    for (int n : sizes) {
        std::vector<int> vec = make_vector(n);
        auto [result, execution_time] = measure_execution_time(make_Parallel_Quick_Sort_sort,std::ref(vec), 8);
        std::cout << execution_time << std::endl;
    }

    return 0;
}