// Date: 2026-09-26
// Problem: Transform Array Using Pair Operations
// Link: https://leetcode.com/problems/transform-array-using-pair-operations/
// Code:

class Solution {
public:
    bool canTransform(vector<int>& s, vector<int>& t) {
        // we can always do it just check for last palce
        int n = s.size();
        vector<long long > a(s.begin(),s.end());
        for(int i = 1 ; i < n ; i++){
            long long p = 1LL*a[i] + a[i-1] - t[i-1];
            a[i] = p;
            a[i-1] = t[i-1];
        }
        for(int i = 0 ; i < n ; i++){
            //cout << s[i] << " " << t[i] << '\n';
            if(a[i] != t[i]) return false;
        }
        return true;
    }
};