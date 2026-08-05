// Date: 2026-08-04
// Problem: Find Missing Elements
// Link: https://leetcode.com/problems/find-missing-elements/
// Code:

class Solution {
public:
    vector<int> findMissingElements(vector<int>& a) {
        int n = a.size();
        sort(a.begin(),a.end());
        vector<int> ans;
        int val = a[0];
        for(int i = 1 ; i < n ; i++){
            while(val+1 != a[i]){
                val++;
                ans.push_back(val);
            }
            val = a[i];
        }
        return ans;

        
    }
};
