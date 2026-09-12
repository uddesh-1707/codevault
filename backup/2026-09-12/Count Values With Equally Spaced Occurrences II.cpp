// Date: 2026-09-12
// Problem: Count Values With Equally Spaced Occurrences II
// Link: https://leetcode.com/problems/count-values-with-equally-spaced-occurrences-ii/
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
            if(s.size() >= 3 ){
                bool ok = false;
                for(int i = 1 ; i + 1 < s.size() ; i++){
                    if(s[i]-s[i-1] != s[i+1]-s[i]) ok = true;
                }
                if(ok == false) ans++;
            }
        }
        return ans;
    }
};