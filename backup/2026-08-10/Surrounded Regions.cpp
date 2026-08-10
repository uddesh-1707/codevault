// Date: 2026-08-10
// Problem: Surrounded Regions
// Link: https://leetcode.com/problems/surrounded-regions/
// Code:

class Solution {
public:
    static const int N = 51;
    int n, m;
    bool vis[201][201];

    int dx[4] = {-1, +1, 0, 0};
    int dy[4] = {0, 0, -1, +1};

    void dfs(int x, int y, vector<vector<char>>& a) {
        if(x < 0 || x >= n || y < 0 || y >= m ||
           vis[x][y] || a[x][y] == 'X')
            return;

        vis[x][y] = true;

        for(int i = 0; i < 4; i++) {
            int x1 = x + dx[i];
            int y1 = y + dy[i];

            dfs(x1, y1, a);
        }
    }

    void solve(vector<vector<char>>& a) {
        n = a.size();
        m = a[0].size();

        memset(vis, false, sizeof(vis));
        for(int i = 0; i < n; i++) {
            if(a[i][0] == 'O')
                dfs(i, 0, a);

            if(a[i][m - 1] == 'O')
                dfs(i, m - 1, a);
        }

        for(int j = 0; j < m; j++) {
            if(a[0][j] == 'O')
                dfs(0, j, a);

            if(a[n - 1][j] == 'O')
                dfs(n - 1, j, a);
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {

                if(a[i][j] == 'O' && !vis[i][j])
                    a[i][j] = 'X';
            }
        }
    }
};