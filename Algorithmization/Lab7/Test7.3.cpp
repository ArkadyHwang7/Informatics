#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../ALG7.3/ALG7.3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(Test_ElementFound)
        {
            vector<int> nums = { 2, 5, 5, 5, 6, 6, 8, 9, 9, 9 };

            int first = findFirst(nums, 5);
            int last = findLast(nums, 5);

            Assert::AreEqual(1, first);
            Assert::AreEqual(3, last);
        }

        TEST_METHOD(Test_ElementNotFound)
        {
            vector<int> nums = { 2, 5, 5, 5, 6, 6, 8, 9, 9, 9 };

            int first = findFirst(nums, 4);
            int last = findLast(nums, 4);

            Assert::AreEqual(-1, first);
            Assert::AreEqual(-1, last);
        }

        TEST_METHOD(Test_ElementAtEnd)
        {
            vector<int> nums = { 2, 5, 5, 5, 6, 6, 8, 9, 9, 9 };

            int first = findFirst(nums, 9);
            int last = findLast(nums, 9);

            Assert::AreEqual(7, first);
            Assert::AreEqual(9, last);
        }
    };
}