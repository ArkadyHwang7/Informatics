#include <iostream>
#include <set>
#include <chrono>

std::set<int> RemoveFromSet(std::set<int> mn, int index) {
    if (index < 0 || index >= mn.size()) {
        return mn;
    }

    std::set<int>::iterator element = mn.begin();

    for (int i = 0; i < index; i++) {
        ++element;
    }

    mn.erase(element);
    return mn;
}

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now(); // начало измерения времени
    auto result = func(std::forward<Args>(args)...); // выполнение функции
    auto end_time = std::chrono::high_resolution_clock::now(); // конец измерения

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return std::make_pair(result, duration.count());
}

int main() {
    std::set<int> mySet;

    for (int i = 0; i < 10000; i++) {
        mySet.insert(i);
    }

    auto [result, execution_time] = measure_execution_time(RemoveFromSet, mySet, 2);

    std::cout << "Результат: " << result.size() << std::endl;
    std::cout << "Время выполнения: " << execution_time << " мс" << std::endl;

    return 0;
}