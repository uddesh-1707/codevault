// Date: 2026-10-14
// Problem: Maximize Sum of Squares of Digits
// Link: https://leetcode.com/problems/maximize-sum-of-squares-of-digits/
// Code:

class Solution {
public:
    string maxSumOfSquares(int num, int sum) {
        // greedy works
        string ans="";
        for (int i = 9; i >= 1; i--) {
            while (sum >= i && ans.size() < num) {
                ans += char(i + '0');
                sum -= i;
            }
        }

        if (sum > 0) return "";

        ans.append(num - ans.size(), '0');
        return ans;
        
    }
};