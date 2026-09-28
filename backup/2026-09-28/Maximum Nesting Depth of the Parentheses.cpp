// Date: 2026-09-28
// Problem: Maximum Nesting Depth of the Parentheses
// Link: https://leetcode.com/problems/maximum-nesting-depth-of-the-parentheses/
// Code:

class Solution {
public:
    int maxDepth(string s) {
        int n = s.length() , cnt = 0 , ans = 0;
        for(int i = 0 ; i < n ; i++){
            if(s[i] == '('){
                cnt++;
            } else if(s[i] == ')'){
                cnt--;
            }
            ans = max(ans,cnt);
        }
        return ans;
    }
};