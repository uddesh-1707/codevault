// Date: 2026-09-19
// Problem: Surrounded Regions
// Link: https://leetcode.com/problems/surrounded-regions/
// Code:

class Solution {
public:
    int n,m;
    bool vis[201][201];
    int dx[4] = {0,0,1,-1};
    int dy[4] = {1,-1,0,0};
    void dfs(int x , int y , vector<vector<char>>& a ){
        if(x < 0 || x >= n || y < 0 || y >= m || vis[x][y] || a[x][y] == 'X') return;
        vis[x][y] = true;
        for(int i = 0 ; i < 4 ; i++){
            int new_x = dx[i] + x;
            int new_y = dy[i] + y;
            dfs(new_x,new_y,a);
        }

    }
    void solve(vector<vector<char>>& a) {
        n = a.size();
        m = a[0].size();
        for(int j = 0 ; j < m ; j++){
            if(a[0][j] == 'O'){
                dfs(0,j,a);
            }
        }
        for(int i = 0 ; i < n ; i++){
            if(a[i][0] == 'O'){
                dfs(i,0,a);
            }
        }
        for(int j = 0 ; j < m ; j++){
            if(a[n-1][j] == 'O'){
                dfs(n-1,j,a);
            }
        }
        for(int i = 0 ; i < n ; i++){
            if(a[i][m-1] == 'O'){
                dfs(i,m-1,a);
            }
        }
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(!vis[i][j] && a[i][j] == 'O'){
                    a[i][j] = 'X';
                }
            }
        }

    }
};