// Date: 2026-07-30
// Problem: Minimum Number of Pushes to Type Word I
// Link: https://leetcode.com/problems/minimum-number-of-pushes-to-type-word-i/
// Code:

class Solution {
public:
    int minimumPushes(string s) {
        int n = s.length();
        vector<char> f(26,0);
        for(int i = 0 ; i < n ; i++){
            f[s[i]-'a']++;
        }
        sort(f.rbegin(),f.rend());
        int cnt = 0;
        for(int i = 0 ; i < 26; i++){
            if(i >= 0 && i <= 7){
                cnt += f[i]*1;
            } else if(i >= 8 && i <= 15){
                cnt += f[i]*2;
            } else if(i >= 16 && i <= 23) {
                cnt += f[i]*3;
            } else{
                cnt += f[i]*4;
            }
        }
        return cnt;
        
    }
};