#include <iostream>
#include <cmath>

double simpson_rule(double (*f)(double), double a, double b, int n) {
    if (n % 2 == 1) n++; // Делаем n четным
    double h = (b - a) / n;
    double sum = f(a) + f(b);

    for (int i = 1; i < n; i += 2)
        sum += 4 * f(a + i * h);

    for (int i = 2; i < n - 1; i += 2)
        sum += 2 * f(a + i * h);

    return (h / 3.0) * sum;
}

double function1(double x) {
    return std::sin(0.5 * x) - 0.5;
}

double function2(double x) {
    double sin_x = std::sin(x);
    double cos_x = std::cos(x);
    return (sin_x < cos_x) ? sin_x : cos_x;
}

double diff(double x) {
    return function2(x) - function1(x);
}

int main() {
    double a = 6.0;
    double b = 8.0;

    // Вычисляем площадь 
    double area = simpson_rule(diff, a, b, 1000);

    std::cout << "Приближенное значение площади области: " << area << std::endl;

    return 0;
}