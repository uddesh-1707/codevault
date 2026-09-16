// Date: 2026-09-16
// Problem: Number of Islands
// Link: https://leetcode.com/problems/number-of-islands/
// Code:

class Solution {
public:
    static const int N = 301;
    bool vis[N][N];
    int n,m;
    int dx[4] = {-1,1,0,0};
    int dy[4] = {0,0,-1,1};
    void dfs(int x , int y , vector<vector<char>>& a){
        if(x < 0 || x >= n || y < 0 || y >= m || vis[x][y] || a[x][y] == '0') return;
        vis[x][y] = true;
        for(int i = 0 ; i < 4 ; i++){
            int nx = dx[i] + x;
            int ny = dy[i] + y;
            dfs(nx,ny,a);
        }
    }
    int numIslands(vector<vector<char>>& a) {
        n = a.size();
        m = a[0].size();
        memset(vis, false, sizeof(vis));
        int ans = 0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(!vis[i][j] && a[i][j] == '1'){
                    ans++;
                    dfs(i,j,a);
                }
            }
        }
        return ans;
    }
};