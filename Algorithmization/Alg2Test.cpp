#include "pch.h"
#include "CppUnitTest.h"

#include "..\ALG2\ALG2.cpp" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            int n = 10;
            double a = 6.0;
            double b = 8.0;

            double result_top = simpson_rule(function2, a, b, n);
            double result_bottom = simpson_rule(function1, a, b, n);
            double area = result_top - result_bottom;

            Assert::AreEqual(2.208, area, 0.01, L"Ошибка при n=10");
        }

        TEST_METHOD(TestMethod2)
        {
            int n = 50;
            double a = 6.0;
            double b = 8.0;

            double result_top = simpson_rule(function2, a, b, n);
            double result_bottom = simpson_rule(function1, a, b, n);
            double area = result_top - result_bottom;

            Assert::AreEqual(2.208, area, 0.001, L"Ошибка при n=50");
        }

        TEST_METHOD(TestMethod3)
        {
            int n = 100;
            double a = 6.0;
            double b = 8.0;

            double area = simpson_rule(diff, a, b, n);

            Assert::AreEqual(2.208, area, 0.0001, L"Ошибка при n=100");
        }

        TEST_METHOD(TestMethod4)
        {
            int n = 1000;
            double a = 6.0;
            double b = 8.0;

            double area = simpson_rule(diff, a, b, n);

            Assert::AreEqual(2.20801, area, 0.00001, L"Ошибка при n=1000");
        }
    };
}
