// Date: 2026-08-18
// Problem: Max Area of Island
// Link: https://leetcode.com/problems/max-area-of-island/
// Code:

class Solution {
public:
    static const int N = 51;
    int n, m, cnt;
    bool vis[N][N];
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    void dfs(vector<vector<int>>& a, int x, int y) {
        if (x < 0 || y < 0 || x >= n || y >= m || vis[x][y] || a[x][y] == 0) {
            return;
        }
        vis[x][y] = true;
        cnt++;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            dfs(a, nx, ny);
        }
    }
    int maxAreaOfIsland(vector<vector<int>>& a) {
        // max size of connected component
        n = a.size();
        m = a[0].size();
        memset(vis, false, sizeof(vis));
        int ans = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] == 1) {
                    cnt = 0;
                    dfs(a, i, j);
                    ans = max(ans, cnt);
                }
            }
        }
        return ans;
    }
};