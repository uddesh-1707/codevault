// Date: 2026-08-19
// Problem: Rotting Oranges
// Link: https://leetcode.com/problems/rotting-oranges/
// Code:

class Solution {
public:
    int orangesRotting(vector<vector<int>>& a) {
        int n = a.size(), m = a[0].size();
        int fresh = 0;
        queue<pair<int, int>> mq;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] == 2)
                    mq.push({i, j});
                if (a[i][j] == 1)
                    fresh++;
            }
        }
        int time = 0;
        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {1, -1, 0, 0};
        while (!mq.empty() && fresh > 0) {
            int sz = mq.size();
            while (sz--) {
                auto [r, c] = mq.front();
                mq.pop();
                for (int i = 0; i < 4; i++) {
                    int nr = r + dx[i];
                    int nc = c + dy[i];
                    if (nr >= 0 && nr < n && nc >= 0 && nc < m &&
                        a[nr][nc] == 1) {
                        a[nr][nc] = 2;
                        fresh--;
                        mq.push({nr, nc});
                    }
                }
            }
            time++;
        }
        return fresh == 0 ? time : -1;
    }
};