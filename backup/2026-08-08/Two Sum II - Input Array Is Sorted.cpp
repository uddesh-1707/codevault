// Date: 2026-08-08
// Problem: Two Sum II - Input Array Is Sorted
// Link: https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/
// Code:

class Solution {
public:
    vector<int> twoSum(vector<int>& a, int t) {
        int lo = 0 , hi = a.size() - 1;
        while(lo <= hi){
            if(a[lo] + a[hi] == t){
                return {lo+1,hi+1};
            } else if(a[lo] + a[hi] < t){
                lo++;
            } else {
                hi--;
            }
        }
        return {-1,-1};
        
    }
};