// Date: 2026-07-27
// Problem: Maximum Product of Two Elements in an Array
// Link: https://leetcode.com/problems/maximum-product-of-two-elements-in-an-array/
// Code:

class Solution {
public:
    int maxProduct(vector<int>& a) {
        int n = a.size();
        sort(a.begin(),a.end());
        return (a[n-1]-1)*(a[n-2]-1);
        
    }
};