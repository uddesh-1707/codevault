// Date: 2026-07-27
// Problem: Sort Array by Increasing Frequency
// Link: https://leetcode.com/problems/sort-array-by-increasing-frequency/
// Code:

class Solution {
public:
    vector<int> frequencySort(vector<int>& a) {
        int n = a.size();
        unordered_map<int,int> mp;
        for(int i = 0 ; i < n ; i++){
            mp[a[i]]++;
        }
        sort(a.begin(),a.end(),[&](int a, int b) {
            return mp[a] == mp[b] ? a > b : mp[a] < mp[b];
        });
        return a;

        
    }
};