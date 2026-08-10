// Date: 2026-08-10
// Problem: Max Area of Island
// Link: https://leetcode.com/problems/max-area-of-island/
// Code:

class Solution {
public:
    static const int N = 51;
    int n,m,connected;;
    bool vis[N][N];
    int dx[4] = {-1,+1,0,0};
    int dy[4] = {0,0,-1,+1};
    void dfs(int x , int y , vector<vector<int>>& a){
        if(x >= n || x < 0 || y < 0 || y >= m || vis[x][y] || a[x][y] == 0) return;
        vis[x][y] = true;
        connected++;
        for(int i = 0 ; i < 4 ; i++){
            int x1 = x+dx[i];
            int y1 = y+dy[i];
            dfs(x1,y1,a);
        }
    }
    int maxAreaOfIsland(vector<vector<int>>& a) {
        n = a.size();
        m = a[0].size();
        memset(vis,false,sizeof(vis));
        int ans = 0;
        connected=0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(!vis[i][j] && a[i][j] == 1){
                    connected=0;
                    dfs(i,j,a);
                    ans = max(ans,connected);
                }
            }
        }
        return ans;

    }
};