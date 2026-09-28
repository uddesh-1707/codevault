// Date: 2026-09-28
// Problem: Keys and Rooms
// Link: https://leetcode.com/problems/keys-and-rooms/
// Code:

class Solution {
public:
    static const int N = 1e3 + 1;
    bool vis[N];
    void dfs(int src , vector<vector<int>>& adj) {
        queue<int> q;
        q.push(src);
        vis[src] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!vis[v]) {
                    q.push(v);
                    vis[v] = true;
                }
            }
        }
    }
    bool canVisitAllRooms(vector<vector<int>>& adj) {
        int n = adj.size();
        dfs(0,adj);
        for(int i = 0 ; i < n ; i++){
            if(!vis[i]) return false;
        }
        return true;
    }
};