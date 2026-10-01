// Date: 2026-09-30
// Problem: Maximum Nesting Depth of Two Valid Parentheses Strings
// Link: https://leetcode.com/problems/maximum-nesting-depth-of-two-valid-parentheses-strings/
// Code:

class Solution {
public:
    vector<int> maxDepthAfterSplit(string s) {
        int n = s.size() , var = 0;
        vector<int> ans;
        for(int i = 0 ; i < n ; i++){
            if(s[i] == '('){
                ans.push_back(var%2);
                var++;
            } else {
                var--;
                ans.push_back(var%2);
            }
        }
        return ans;
    }
};