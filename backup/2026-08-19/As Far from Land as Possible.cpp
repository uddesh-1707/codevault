// Date: 2026-08-19
// Problem: As Far from Land as Possible
// Link: https://leetcode.com/problems/as-far-from-land-as-possible/
// Code:

class Solution {
public:
    int maxDistance(vector<vector<int>>& a) {
        int n = a.size(), m = a[0].size();
        vector<vector<int>> dist(n, vector<int>(m, -1));
        queue<pair<int, int>> mq;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] == 1) {
                    mq.push({i, j});
                    dist[i][j] = 0;
                }
            }
        }
        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {1, -1, 0, 0};
        while (!mq.empty()) {
            auto [r, c] = mq.front();
            mq.pop();
            for (int i = 0; i < 4; i++) {
                int nr = r + dx[i];
                int nc = c + dy[i];
                if (nr >= 0 && nr < n && nc >= 0 && nc < m &&
                    dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    mq.push({nr, nc});
                }
            }
        }
        // get i,j of cell with maximum value and maintain max manathan distance
        // but with nearest cell
        int ans = -1;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] == 0) {
                    ans = max(ans, dist[i][j]);
                }
            }
        }

        return ans;

    }
};