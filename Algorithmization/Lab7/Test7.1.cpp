#include "pch.h"
#include "CppUnitTest.h"
#include "../ALG7.1/ALG7.1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(Test_SameTree)
        {
            TreeNode* first = new TreeNode(1);
            first->left = new TreeNode(2);
            first->right = new TreeNode(3);

            TreeNode* second = new TreeNode(1);
            second->left = new TreeNode(2);
            second->right = new TreeNode(3);

            Assert::IsTrue(areTwins(first, second));

            deleteTree(first);
            deleteTree(second);
        }

        TEST_METHOD(Test_DifferentStructure)
        {
            TreeNode* first = new TreeNode(1);
            first->left = new TreeNode(3);

            TreeNode* second = new TreeNode(1);
            second->right = new TreeNode(3);

            Assert::IsFalse(areTwins(first, second));

            deleteTree(first);
            deleteTree(second);
        }

        TEST_METHOD(Test_DifferentValues)
        {
            TreeNode* first = new TreeNode(1);
            first->left = new TreeNode(2);

            TreeNode* second = new TreeNode(1);
            second->left = new TreeNode(5);

            Assert::IsFalse(areTwins(first, second));

            deleteTree(first);
            deleteTree(second);
        }
    };
}