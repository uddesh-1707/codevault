// Date: 2026-10-15
// Problem: Subarrays Distinct Element Sum of Squares I
// Link: https://leetcode.com/problems/subarrays-distinct-element-sum-of-squares-i/
// Code:

class Solution {
public:
    int sumCounts(vector<int>& a) {
        int n = a.size(),sum=0;
        for(int i = 0 ; i < n ; i++){
            set<int>s;
            for(int j = i ; j < n ; j++){
                s.insert(a[j]);
                sum += (s.size()*s.size());
            }
        }
        return sum;
        
    }
};