// Date: 2026-08-06
// Problem: Smallest Divisible Digit Product I
// Link: https://leetcode.com/problems/smallest-divisible-digit-product-i/
// Code:

class Solution {
public:
    int smallestNumber(int n, int t) {
        int ans;
        for(int i = n ; ; i++){
            int v = i , mul = 1;
            while(v > 0){
                mul *= (v%10);
                v /= 10;
            }
            if(mul%t == 0) {
                ans = i;
                break;
            }
        }
        return ans; 
    }
};