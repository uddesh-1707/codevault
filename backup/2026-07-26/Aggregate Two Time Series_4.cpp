// Date: 2026-07-26
// Problem: Aggregate Two Time Series
// Link: https://leetcode.com/problems/aggregate-two-time-series/
// Code:

class Solution {
public:
    vector<vector<int>> aggregateTimeSeries(vector<vector<int>>& s1,
                                            vector<vector<int>>& s2) {
        int n = s1.size() , m = s2.size() , i = 0 , j = 0;
        vector<vector<int>> ans;
        while(i < n && j < m){
            if(s1[i][0] == s2[j][0]){
                ans.push_back({s1[i][0],s1[i][1]+s2[j][1]});
                i++;
                j++;
            } else if(s1[i][0] < s2[j][0]){
                ans.push_back({s1[i][0],s1[i][1]+s2[j][1]});
                i++;
            } else{
                ans.push_back({s2[j][0],s1[i][1]+s2[j][1]});
                j++;
            }
        }
        while(i < n){
            ans.push_back({s1[i][0],s1[i][1]});
                i++;
        }
        while(j < m){
            ans.push_back({s2[j][0],s2[j][1]});
                j++;
        }
        return ans;
    }
};