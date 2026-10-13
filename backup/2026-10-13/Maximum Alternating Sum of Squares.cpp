// Date: 2026-10-13
// Problem: Maximum Alternating Sum of Squares
// Link: https://leetcode.com/problems/maximum-alternating-sum-of-squares/
// Code:

class Solution {
public:
    long long maxAlternatingSum(vector<int>& a) {
        // bring large nums at even and small at odd
        int n = a.size();
        for(int i = 0 ; i < n ; i++){
            if(a[i] < 0) a[i] = -a[i];
        }
        sort(a.begin(),a.end());
        int l = 0 , r = n-1 ;
        long long  score = 0;
        while(l < r){
            score += 1LL*(a[r]*a[r]);
            score -= 1LL*(a[l]*a[l]);
            l++;
            r--;
        }
        if(n%2 == 1){
            score += (a[r]*a[r]);
        }
        return score;
        
    }
};