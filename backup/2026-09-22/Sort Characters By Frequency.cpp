// Date: 2026-09-22
// Problem: Sort Characters By Frequency
// Link: https://leetcode.com/problems/sort-characters-by-frequency/
// Code:

class Solution {
public:
    string frequencySort(string s) {
        int n = s.length();
        map<char,int>mp;
        for(int i = 0 ; i < n ; i++){
            mp[s[i]]++;
        }

        auto cmp = [&](char a , char b){
             if (mp[a] != mp[b])
                return mp[a] > mp[b];

            return a < b;
        };
        sort(s.begin(),s.end(),cmp);
        return s;
    }
};