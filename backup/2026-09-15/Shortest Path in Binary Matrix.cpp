// Date: 2026-09-15
// Problem: Shortest Path in Binary Matrix
// Link: https://leetcode.com/problems/shortest-path-in-binary-matrix/
// Code:

class Solution {
public:
    int dx[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dy[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size();

        if (grid[0][0] == 1 || grid[n - 1][n - 1] == 1)
            return -1;

        queue<pair<int, pair<int, int>>> q;
        vector<vector<int>> vis(n, vector<int>(n, 0));

        vis[0][0] = 1;
        q.push({1, {0, 0}});

        while (!q.empty()) {
            auto it = q.front();
            q.pop();

            int dis = it.first;
            int row = it.second.first;
            int col = it.second.second;

            if (row == n - 1 && col == n - 1)
                return dis;

            for (int i = 0; i < 8; i++) {
                int newr = row + dx[i];
                int newc = col + dy[i];
                if (newr >= 0 && newc >= 0 && newr < n && newc < n &&
                    grid[newr][newc] == 0 && vis[newr][newc] == 0) {

                    vis[newr][newc] = 1;
                    q.push({dis + 1, {newr, newc}});
                }
            }
        }

        return -1;
    }
};