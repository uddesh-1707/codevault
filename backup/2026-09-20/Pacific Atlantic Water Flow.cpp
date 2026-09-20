// Date: 2026-09-20
// Problem: Pacific Atlantic Water Flow
// Link: https://leetcode.com/problems/pacific-atlantic-water-flow/
// Code:

class Solution {
public:
    static const int N = 201;
    int n, m;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    bool vis_p[N][N], vis_a[N][N];
    void dfs(int x, int y, int type, vector<vector<int>>& a) {
        if (x < 0 || y < 0 || x >= n || y >= m)
            return;
        if (type == 1 && vis_p[x][y])
            return;
        if (type == 0 && vis_a[x][y])
            return;
        if (type == 1) {
            vis_p[x][y] = true;
        } else {
            vis_a[x][y] = true;
        }
        for (int i = 0; i < 4; i++) {
            int new_x = dx[i] + x;
            int new_y = dy[i] + y;
            if (new_x >= 0 && new_x < n && new_y >= 0 && new_y < m &&
                a[new_x][new_y] >= a[x][y]) {

                dfs(new_x, new_y, type, a);
            }
        }
    }
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& a) {
        n = a.size();
        m = a[0].size();
        for (int j = 0; j < m; j++) {
            if (!vis_p[0][j])
                dfs(0, j, 1, a);
            if (!vis_a[0][j])
                dfs(n - 1, j, 0, a);
        }
        for (int i = 0; i < n; i++) {
            if (!vis_p[i][0])
                dfs(i, 0, 1, a);
            if (!vis_a[i][m - 1])
                dfs(i, m - 1, 0, a);
        }
        vector<vector<int>> ans;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (vis_p[i][j] && vis_a[i][j]) {
                    ans.push_back({i, j});
                }
            }
        }
        return ans;
    }
};