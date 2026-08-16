// Date: 2026-08-16
// Problem: Maximum Gap Between Stations
// Link: https://leetcode.com/problems/maximum-gap-between-stations/
// Code:

class Solution {
public:
    int maximumGap(string s, string st) {
        int n = s.length() , m = st.length();
        if(n == 1){
            return 0;
        }
        vector<int> ok(n);
        int lo = 0 , hi = m-1,ans=0;
        // go from right
        for(int i = n-1 ; i >= 0 ; i--){
            while(st[hi] != s[i]){
                hi--;
            }
            ok[i] = hi;
            hi--;
        }
        // left
        for(int i = 0 ; i+1 < n ; i++){
            while(st[lo] != s[i]) lo++;
            ans = max(ans,ok[i+1]-lo);
            lo++;
            
        } 
        return ans;
        
        
    }
};