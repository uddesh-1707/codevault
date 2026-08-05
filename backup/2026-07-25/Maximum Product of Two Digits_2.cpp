// Date: 2026-07-25
// Problem: Maximum Product of Two Digits
// Link: https://leetcode.com/problems/maximum-product-of-two-digits/
// Code:

class Solution {
public:
    int maxProduct(int n) {
        vector<int> a;
        int v = n;
        while(v > 0){
            a.push_back(v%10);
            v /= 10;
        }
        int ans = INT_MIN;
        for(int i = 0 ; i < a.size() ; i++){
            for(int j = i+1 ; j < a.size() ; j++){
                ans = max(ans,a[i]*a[j]);
            }
        }
        return ans;
        
    }
};