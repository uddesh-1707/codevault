// Date: 2026-09-16
// Problem: Number of Provinces
// Link: https://leetcode.com/problems/number-of-provinces/
// Code:

class Solution {
public:
    static const int N = 201;
    bool vis[N];
    int n, m;
    void dfs(int u,vector<vector<int>>& a) {
        vis[u] = true;
        for(int v = 0 ; v < a[u].size() ; v++){
            if(!vis[v] && a[u][v] == 1){
                dfs(v,a);
            }
        }

    }
    int findCircleNum(vector<vector<int>>& a) {
        n = a.size();
        memset(vis, false, sizeof(vis));
        int ans = 0;
        for(int i = 0 ; i < n ; i++){
            if(!vis[i]){
                dfs(i,a);
                ans++;
            }
        }
        return ans;
    }
};