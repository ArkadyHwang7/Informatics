#include <iostream>
#include <vector>

using namespace std;

int findFirst(const vector<int>& nums, int target) {
    int left = 0;
    int right = (int)nums.size() - 1;
    int answer = -1;

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (nums[middle] == target) {
            answer = middle;
            right = middle - 1;
        }
        else if (nums[middle] < target) {
            left = middle + 1;
        }
        else {
            right = middle - 1;
        }
    }

    return answer;
}

int findLast(const vector<int>& nums, int target) {
    int left = 0;
    int right = (int)nums.size() - 1;
    int answer = -1;

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (nums[middle] == target) {
            answer = middle;
            left = middle + 1;
        }
        else if (nums[middle] < target) {
            left = middle + 1;
        }
        else {
            right = middle - 1;
        }
    }

    return answer;
}

int main() {
    int n;
    cin >> n;

    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];

    int target;
    cin >> target;

    int first = findFirst(nums, target);
    int last = findLast(nums, target);

    if (first == -1) {
        cout << "Element not found in the array" << endl;
    }
    else {
        cout << "The first occurrence of element " << target << " is located at index " << first << endl;
        cout << "The last occurrence of element " << target << " is located at index " << last << endl;
    }

    return 0;
}

