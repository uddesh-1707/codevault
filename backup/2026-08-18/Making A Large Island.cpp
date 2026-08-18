// Date: 2026-08-18
// Problem: Making A Large Island
// Link: https://leetcode.com/problems/making-a-large-island/
// Code:

class Solution {
public:
    static const int N = 501;
    int n, m, cnt;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    void bfs(int x, int y, vector<vector<int>>& a, int id) {
        queue<pair<int, int>> q;

        q.push({x, y});
        a[x][y] = id;

        cnt = 0;

        while (!q.empty()) {
            cnt++;

            auto [cx, cy] = q.front();
            q.pop();

            for (int i = 0; i < 4; i++) {
                int nx = cx + dx[i];
                int ny = cy + dy[i];

                if (nx >= 0 && nx < n && ny >= 0 && ny < m && a[nx][ny] == 1) {

                    a[nx][ny] = id;
                    q.push({nx, ny});
                }
            }
        }
    }
    int largestIsland(vector<vector<int>>& a) {
        n = a.size();
        m = a[0].size();

        vector<int> sz(2);

        int id = 2;

        // Find every component
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {

                if (a[i][j] == 1) {

                    bfs(i, j, a, id);

                    sz.push_back(cnt);

                    id++;
                }
            }
        }

        int ans = 0;

        // Try every zero
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {

                if (a[i][j] != 0)
                    continue;

                int cur = 1;

                int seen[4];
                int k = 0;

                // Check 4 neighbours
                for (int d = 0; d < 4; d++) {

                    int ni = i + dx[d];
                    int nj = j + dy[d];

                    if (ni < 0 || ni >= n || nj < 0 || nj >= m)
                        continue;

                    int id = a[ni][nj];

                    if (id == 0)
                        continue;

                    bool already = false;

                    for (int x = 0; x < k; x++) {
                        if (seen[x] == id)
                            already = true;
                    }

                    if (!already) {
                        cur += sz[id];
                        seen[k++] = id;
                    }
                }

                ans = max(ans, cur);
            }
        }
        if (ans == 0)
            return n * m;

        return ans;
    }
};