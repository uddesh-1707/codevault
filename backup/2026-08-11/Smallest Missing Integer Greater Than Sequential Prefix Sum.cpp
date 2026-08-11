// Date: 2026-08-11
// Problem: Smallest Missing Integer Greater Than Sequential Prefix Sum
// Link: https://leetcode.com/problems/smallest-missing-integer-greater-than-sequential-prefix-sum/
// Code:

class Solution {
public:
    int missingInteger(vector<int>& a) {
        int n = a.size();

        int sum = a[0];

        for (int i = 1; i < n; i++) {
            if (a[i] == a[i - 1] + 1) {
                sum += a[i];
            } else {
                break;
            }
        }

        vector<bool> ht(1276, false);

        for (int i = 0; i < n; i++) {
            ht[a[i]] = true;
        }

        while (ht[sum]) {
            sum++;
        }

        return sum;
    }
};