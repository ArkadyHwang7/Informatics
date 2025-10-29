#include <iostream>

void ArrayB(int* A, int* B, int size, int* new_size) {
    *new_size = 0;
    for (int i = 0; i < size; i++) {
        if (A[i] > 10 && A[i] < 50) {
            B[*new_size] = A[i];
            (*new_size)++;
        }
    }
}

int main() {
    int* A = new int[10];
    int* B = new int[10];
    int new_size;

    std::cout << "Введите 10 элементов массива A:" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cin >> A[i];
    }

    ArrayB(A, B, 10, &new_size);

    std::cout << "Элементы массива A: ";
    for (int i = 0; i < 10; i++) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Элементы массива B (меньше 50 и больше 10): ";
    for (int i = 0; i < new_size; i++) {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;

    delete[] A;
    delete[] B;

    return 0;

}
