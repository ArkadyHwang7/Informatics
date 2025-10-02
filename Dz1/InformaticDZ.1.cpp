#include <iostream>
#include <cmath>
#include <Windows.h>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    float x, y;
    cout << "Введите x и y: ";
    cin >> x >> y;

    float R = (-x - sqrt(exp(2.0f) - 4.0f * x * y)) / (2.0f * x);
    float S = log(x) * tan(y);

    cout << "R = " << R << endl;
    cout << "S = " << S << endl;

    float C = fmax(R, S);

    cout << "Максимальное значение C = " << C << endl;

    cout << "\nНажмите Enter, чтобы завершить программу...";
    cin.ignore();      
    cin.get();        

    return 0;
}
