#include "pch.h"
#include "CppUnitTest.h"
#include "..\ALG3\ALG3.cpp"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			std::vector<int> massive1 = { 8, 4, 6 };
			std::vector<int> expected1 = { 4, 6, 8 };
			std::vector<int> actual1 = Merge_Sort(massive1);

			std::vector<int> massive2 = { 15, 3, 11, 7, 1 };
			std::vector<int> expected2 = { 1, 3, 7, 11, 15 };
			std::vector<int> actual2 = Merge_Sort(massive2);

			std::vector<int> massive3 = { 20, 5, 100, 13 };
			std::vector<int> expected3 = { 5, 13, 20, 100 };
			std::vector<int> actual3 = Merge_Sort(massive3);

			std::vector<int> massive4 = { 300, 12, 0, 45 };
			std::vector<int> expected4 = { 0, 12, 45, 300 };
			std::vector<int> actual4 = Merge_Sort(massive4);

			std::vector<int> massive5 = { 71, 18, 52, 144 };
			std::vector<int> expected5 = { 18, 52, 71, 144 };
			std::vector<int> actual5 = Merge_Sort(massive5);

			Assert::IsTrue(massive1 == expected1, L"Значения не совпали");
			Assert::IsTrue(massive2 == expected2, L"Значения не совпали");
			Assert::IsTrue(massive3 == expected3, L"Значения не совпали");
			Assert::IsTrue(massive4 == expected4, L"Значения не совпали");
			Assert::IsTrue(massive5 == expected5, L"Значения не совпали");
		}

		TEST_METHOD(TestMethod2)
		{
			std::vector<int> massive1 = { 8, 2, 5 };
			std::vector<int> expected1 = { 2, 5, 8 };
			std::vector<int> actual1 = Radix_Sort(massive1);

			std::vector<int> massive2 = { 19, 6, 2, 13, 4 };
			std::vector<int> expected2 = { 2, 4, 6, 13, 19 };
			std::vector<int> actual2 = Radix_Sort(massive2);

			std::vector<int> massive3 = { 42, 7, 90, 11 };
			std::vector<int> expected3 = { 7, 11, 42, 90 };
			std::vector<int> actual3 = Radix_Sort(massive3);

			std::vector<int> massive4 = { 500, 30, 0, 9 };
			std::vector<int> expected4 = { 0, 9, 30, 500 };
			std::vector<int> actual4 = Radix_Sort(massive4);

			std::vector<int> massive5 = { 64, 120, 33, 17 };
			std::vector<int> expected5 = { 17, 33, 64, 120 };
			std::vector<int> actual5 = Radix_Sort(massive5);

			Assert::IsTrue(massive1 == expected1, L"Значения не совпали");
			Assert::IsTrue(massive2 == expected2, L"Значения не совпали");
			Assert::IsTrue(massive3 == expected3, L"Значения не совпали");
			Assert::IsTrue(massive4 == expected4, L"Значения не совпали");
			Assert::IsTrue(massive5 == expected5, L"Значения не совпали");
		}

		TEST_METHOD(TestMethod3)
		{
			std::vector<int> massive1 = { 10, 1, 6 };
			std::vector<int> expected1 = { 1, 6, 10 };
			std::vector<int> actual1 = Make_Quick_Sort(massive1);

			std::vector<int> massive2 = { 25, 14, 3, 8, 2 };
			std::vector<int> expected2 = { 2, 3, 8, 14, 25 };
			std::vector<int> actual2 = Make_Quick_Sort(massive2);

			std::vector<int> massive3 = { 32, 4, 18, 77 };
			std::vector<int> expected3 = { 4, 18, 32, 77 };
			std::vector<int> actual3 = Make_Quick_Sort(massive3);

			std::vector<int> massive4 = { 600, 21, 0, 15 };
			std::vector<int> expected4 = { 0, 15, 21, 600 };
			std::vector<int> actual4 = Make_Quick_Sort(massive4);

			std::vector<int> massive5 = { 91, 44, 12, 105 };
			std::vector<int> expected5 = { 12, 44, 91, 105 };
			std::vector<int> actual5 = Make_Quick_Sort(massive5);

			Assert::IsTrue(massive1 == expected1, L"Значения не совпали");
			Assert::IsTrue(massive2 == expected2, L"Значения не совпали");
			Assert::IsTrue(massive3 == expected3, L"Значения не совпали");
			Assert::IsTrue(massive4 == expected4, L"Значения не совпали");
			Assert::IsTrue(massive5 == expected5, L"Значения не совпали");
		}
	};
}