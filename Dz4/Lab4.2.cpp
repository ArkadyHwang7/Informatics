#include <iostream>
#include <set>
#include <windows.h>

void intersection(std::set<int> A, std::set<int> B) {
    std::set<int> a_b; 
    a_b.clear();

    for (int a : A) {
        if (B.find(a) != B.end()) {
            a_b.insert(a);
        }
    }

    if (a_b.empty()) {
        std::cout << "Пересечения множеств нет." << std::endl;
        return;
    }

    int sum = 0;
    std::cout << "Пересечение множеств A и B: ";
    for (int ch : a_b) {
        std::cout << ch << " ";
        sum += ch;
    }
    std::cout << std::endl;

    std::cout << "Сумма элементов пересечения: " << sum << std::endl;
}

int main() {
    std::set<int> A;
    std::set<int> B;
    int x;

    std::cout << "Введите элементы множества A (напишите 0 после окончания):" << std::endl;
    while (std::cin >> x && x != 0) {
        A.insert(x);
    }

    std::cout << "Введите элементы множества B (напишите 0 после окончания):" << std::endl;
    while (std::cin >> x && x != 0) {
        B.insert(x);
    }

    intersection(A, B);

    std::cin.ignore();
    std::cin.get();

    return 0;
}