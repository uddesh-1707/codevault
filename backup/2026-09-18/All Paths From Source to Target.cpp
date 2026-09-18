// Date: 2026-09-18
// Problem: All Paths From Source to Target
// Link: https://leetcode.com/problems/all-paths-from-source-to-target/
// Code:

class Solution {
public:
    vector<vector<int>> ans;
    vector<int> path;
    int n;
    void rec(int u ,vector<vector<int>>& a){
        path.push_back(u);
        if(u == n-1){
            ans.push_back(path);
        }
        for(int v : a[u]){
            rec(v,a);
            //path.pop_back();
        }
        path.pop_back();
    }
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& a) {
        // looks like backtracking problem to print all paths
        n = a.size();
        rec(0,a);
        return ans;
    }
};