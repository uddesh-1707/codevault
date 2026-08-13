// Date: 2026-08-13
// Problem: Length of Longest Subarray With at Most K Frequency
// Link: https://leetcode.com/problems/length-of-longest-subarray-with-at-most-k-frequency/
// Code:

class Solution {
public:
    int maxSubarrayLength(vector<int>& a, int k) {
        int n = a.size();
        map<int,int> mp;
        int maxi = 0;
        int j = 0;

        for(int i = 0; i < n; i++){
            mp[a[i]]++;

            while(mp[a[i]] > k){
                mp[a[j]]--;
                j++;
            }

            maxi = max(maxi, i - j + 1);
        }

        return maxi;
    }
};