// Date: 2026-07-26
// Problem: Maximum Product of Three Numbers
// Link: https://leetcode.com/problems/maximum-product-of-three-numbers/
// Code:

class Solution {
public:
    int maximumProduct(vector<int>& a) {
        int n = a.size();
        sort(a.begin(),a.end());
        return max(a[0]*a[1]*a[n-1],a[n-1]*a[n-2]*a[n-3]);
        
    }
};