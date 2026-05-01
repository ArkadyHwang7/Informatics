#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>

template <typename Result>
struct Time_Result
{
    Result result;
    long long execution_time;
};

template <typename Func, typename... Args>
auto Measure_Execution_Time(Func&& func, Args&&... args)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = func(args...);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return Time_Result<decltype(result)>{ result, duration.count() };
}

std::vector<int> Merge_Sort(std::vector<int>& vec)
{
    if (vec.size() > 1)
    {
        int mid = vec.size() / 2;

        std::vector<int> left(vec.begin(), vec.begin() + mid);
        std::vector<int> right(vec.begin() + mid, vec.end());

        Merge_Sort(left);
        Merge_Sort(right);

        int i = 0;
        int j = 0;
        int k = 0;

        while ((i < left.size()) && (j < right.size()))
        {
            if (left[i] <= right[j])
            {
                vec[k] = left[i];
                i++;
            }
            else
            {
                vec[k] = right[j];
                j++;
            }

            k++;
        }

        while (i < left.size())
        {
            vec[k] = left[i];
            i++;
            k++;
        }

        while (j < right.size())
        {
            vec[k] = right[j];
            j++;
            k++;
        }
    }

    return vec;
}

std::vector<int> Union_Baskets(std::vector<std::vector<int>>& baskets)
{
    std::vector<int> union_vec;

    for (std::vector<int>& basket : baskets)
    {
        union_vec.insert(union_vec.end(), basket.begin(), basket.end());
    }

    return union_vec;
}

void Sort_Rad(std::vector<int>& vec, int& rad)
{
    std::vector<std::vector<int>> baskets(10);

    for (int value : vec)
    {
        int basket_index = (value / rad) % 10;
        baskets[basket_index].push_back(value);
    }

    vec = Union_Baskets(baskets);
}

std::vector<int> Radix_Sort(std::vector<int>& vec)
{
    if (vec.empty())
    {
        return vec;
    }

    int max_value = *std::max_element(vec.begin(), vec.end());
    int rad = 1;

    while (max_value / rad > 0)
    {
        Sort_Rad(vec, rad);
        rad = rad * 10;
    }

    return vec;
}

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

std::vector<int> Make_Vector(int n)
{
    std::vector<int> vec(n);

    for (int i = 0; i < vec.size(); ++i)
    {
        vec[i] = std::rand() % 1000000;
    }

    return vec;
}

std::vector<int> Make_Quick_Sort(std::vector<int>& vec)
{
    if (!vec.empty())
    {
        Quick_Sort(vec, 0, vec.size() - 1);
    }

    return vec;
}

int main()
{
    std::srand(std::time(nullptr));

    int sizes[] = { 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000 };

    for (int n : sizes)
    {
        std::vector<int> vec = Make_Vector(n);
        auto [result, execution_time] = Measure_Execution_Time(Merge_Sort, vec);
        std::cout << execution_time << std::endl;
    }

    std::cout << "" << std::endl;

    for (int n : sizes)
    {
        std::vector<int> vec = Make_Vector(n);
        auto [result, execution_time] = Measure_Execution_Time(Radix_Sort, vec);
        std::cout << execution_time << std::endl;
    }

    std::cout << "" << std::endl;

    for (int n : sizes)
    {
        std::vector<int> vec = Make_Vector(n);
        auto [result, execution_time] = Measure_Execution_Time(Make_Quick_Sort, vec);
        std::cout << execution_time << std::endl;
    }

    return 0;
}