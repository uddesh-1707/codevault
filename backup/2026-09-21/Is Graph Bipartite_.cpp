// Date: 2026-09-21
// Problem: Is Graph Bipartite?
// Link: https://leetcode.com/problems/is-graph-bipartite/
// Code:

class Solution {
public:
    bool dfs(int u , int color , vector<vector<int>>& adj , vector<int> &vis ){
        vis[u] = color;
        for(int v : adj[u]){
            if(vis[v] == 0){
                if(!dfs(v,-color,adj,vis)) return false;
            } else {
                // check parent and child color is not same
                if(vis[v] == vis[u]) return false;
            }
        }
        return true;

    }
    bool isBipartite(vector<vector<int>>& adj) {
        // can be done using BFs and coloring 
        int n = adj.size();
        vector<int> vis(n,0); // 0 unvisited , 1 red(parent) , -1 yellow(child)
        for(int i = 0 ; i < n ; i++){
            if(vis[i] == 0){
                if(!dfs(i,1,adj,vis)){
                    return false;
                }
            }
        }
        return true; 
    }
};