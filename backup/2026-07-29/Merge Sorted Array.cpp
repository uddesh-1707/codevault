// Date: 2026-07-29
// Problem: Merge Sorted Array
// Link: https://leetcode.com/problems/merge-sorted-array/
// Code:

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        for (int j = 0, i = m; j<n; j++){
            nums1[i] = nums2[j];
            i++;
        }
        sort(nums1.begin(),nums1.end());
    }
};