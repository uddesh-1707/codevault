// Date: 2026-09-06
// Problem: Smallest Stable Index II
// Link: https://leetcode.com/problems/smallest-stable-index-ii/
// Code:

class Solution {
public:
    int firstStableIndex(vector<int>& a, int k) {
        // calculate score in O(1)
        int n = a.size();
        vector<int> pre_max(n,INT_MIN) , suff_min(n,INT_MAX);
        pre_max[0] = a[0];
        for(int i = 1 ; i < n ; i++){
            pre_max[i] = max(pre_max[i-1],a[i]);
        }
        suff_min[n-1] = a[n-1];
        for(int i = n-2 ; i >= 0 ; i--){
            suff_min[i] = min(suff_min[i+1],a[i]);
        }
        int ans = INT_MAX , idx = INT_MAX;
        for(int i = 0 ; i < n ; i++){
            int val = pre_max[i]-suff_min[i];
            if(val <= k){
                ans = min(ans,val);
                idx = min(idx,i);
            }
        }
        return (ans == INT_MAX) ? -1 : idx;
    }
};