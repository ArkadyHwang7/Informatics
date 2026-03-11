#include "pch.h"
#include "CppUnitTest.h"
#include <set>

#include "..\ALG1\ALG1.cpp"

std::set<int> RemoveFromSet(std::set<int> mn, int index);

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Alg1Test
{
	TEST_CLASS(Alg1Test)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			std::set<int> input = { 10,20,30,40,50 };
			std::set<int> expected = { 20,30,40,50 };

			std::set<int> result = RemoveFromSet(input, 0);

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestMethod2)
		{
			std::set<int> input = { 10,20,30,40,50 };
			std::set<int> expected = { 10,20,30,40 };

			std::set<int> result = RemoveFromSet(input, 4);

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestMethod3)
		{
			std::set<int> input = { 10,20,30,40,50 };
			std::set<int> expected = { 10,20,40,50 };

			std::set<int> result = RemoveFromSet(input, 2);

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestMethod4)
		{
			std::set<int> input = { 10,20,30,40,50 };

			std::set<int> result = RemoveFromSet(input, -1);

			Assert::IsTrue(result == input);
		}

		TEST_METHOD(TestMethod5)
		{
			std::set<int> input = { 10,20,30,40,50 };

			std::set<int> result = RemoveFromSet(input, 10);

			Assert::IsTrue(result == input);
		}

		TEST_METHOD(TestMethod6)
		{
			std::set<int> input = { 10,20,30,40,50 };

			std::set<int> result = RemoveFromSet(input, 5);

			Assert::IsTrue(result == input);
		}

		TEST_METHOD(TestMethod7)
		{
			std::set<int> input = { };
			std::set<int> expected = { };

			std::set<int> result = RemoveFromSet(input, 0);

			Assert::IsTrue(result == expected);
		}

	};
}