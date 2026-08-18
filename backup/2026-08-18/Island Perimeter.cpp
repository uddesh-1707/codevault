// Date: 2026-08-18
// Problem: Island Perimeter
// Link: https://leetcode.com/problems/island-perimeter/
// Code:

class Solution {
public:
    static const int N = 101;
    int n, m;
    bool vis[N][N];
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    void dfs(vector<vector<int>>& a, int x, int y) {
        if (x < 0 || y < 0 || x >= n || y >= m || vis[x][y] || a[x][y] == 0) {
            return;
        }
        vis[x][y] = true;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            dfs(a, nx, ny);
        }
    }
    int islandPerimeter(vector<vector<int>>& a) {
        n = a.size();
        m = a[0].size();
        memset(vis, false, sizeof(vis));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] == 1 && !vis[i][j]) {
                    dfs(a, i, j);
                }
            }
        }
        int ans = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (vis[i][j]) {
                    for (int k = 0; k < 4; k++) {
                        int nx = i + dx[k];
                        int ny = j + dy[k];
                        if(nx < 0 || nx >= n || ny < 0 || ny >= m){
                            ans++;
                        } else if(!vis[nx][ny]){
                            ans++;
                        }
                    }
                }
            }
        }
        return ans;
    }
};