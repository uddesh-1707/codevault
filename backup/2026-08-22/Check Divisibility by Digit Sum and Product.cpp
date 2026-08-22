// Date: 2026-08-22
// Problem: Check Divisibility by Digit Sum and Product
// Link: https://leetcode.com/problems/check-divisibility-by-digit-sum-and-product/
// Code:

class Solution {
public:
    bool checkDivisibility(int n) {
        long long  sum = 0 , prod = 1 , copy = n;
        while(n > 0){
            long long v = 1LL*(n % 10);
            sum += v;
            prod *= v;
            n /= 10;
        }
        return copy % (sum+prod) == 0;
        
    }
};