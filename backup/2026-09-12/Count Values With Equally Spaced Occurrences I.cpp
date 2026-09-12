// Date: 2026-09-12
// Problem: Count Values With Equally Spaced Occurrences I
// Link: https://leetcode.com/problems/count-values-with-equally-spaced-occurrences-i/
// Code:

class Solution {
public:
    int countSpecialIntegers(vector<int>& a) {
        int n = a.size();
        map<int,vector<int>> mp;
        for(int i = 0 ; i < n ; i++){
            mp[a[i]].push_back(i);
        }
        int ans = 0;
        for(auto [f,s]: mp){
            if(s.size() == 3 && s[1]-s[0] == s[2]-s[1]) ans++;
        }
        return ans;
        
    }
};