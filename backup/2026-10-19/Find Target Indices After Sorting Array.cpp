// Date: 2026-10-19
// Problem: Find Target Indices After Sorting Array
// Link: https://leetcode.com/problems/find-target-indices-after-sorting-array/
// Code:

class Solution {
public:
    vector<int> targetIndices(vector<int>& a, int t) {
        int n = a.size();
        sort(a.begin(),a.end());
        int s = lower_bound(a.begin(),a.end(),t) - a.begin();
        int e = upper_bound(a.begin(),a.end(),t) - a.begin();
        if(s == n && e == n) return {};
        vector<int> ans;
        for(int i = s ; i <= e-1 ; i++){
            ans.push_back(i);
        }
        return ans;
        
    }
};