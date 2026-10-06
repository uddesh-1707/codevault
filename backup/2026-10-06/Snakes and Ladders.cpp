// Date: 2026-10-06
// Problem: Snakes and Ladders
// Link: https://leetcode.com/problems/snakes-and-ladders/
// Code:

class Solution {
public:
    int n, moves;
    unordered_map<int, vector<int>> mp;
    bool vis[21][21];
    int bfs(int x, int y, int pos) {
        queue<pair<int, int>> q;
        vis[x][y] = true;
        q.push({pos, 0});
        while (!q.empty()) {
            auto [u, moves] = q.front();
            q.pop();
            if (u == n * n)
                return moves;
            for (int v = u + 1; v <= min(u + 6, n * n); v++) {
                int next = v;
                if (mp[v][2] != -1) next = mp[v][2];
                int n_x = mp[next][0];
                int n_y = mp[next][1];
                if (!vis[n_x][n_y]) {
                    vis[n_x][n_y] = true;
                    q.push({next, moves + 1});
                }
            }
        }
        return -1;
    }
    int snakesAndLadders(vector<vector<int>>& a) {
        n = a.size();
        moves = 0;
        // start -> (n-1,0) , end -> (0,0)
        int key = 1;
        for (int i = n - 1; i >= 0; i--) {
            if ((n - 1 - i) % 2 == 0) {
                for (int j = 0; j < n; j++) {
                    mp[key++] = {i, j, a[i][j]};
                }
            } else {
                for (int j = n - 1; j >= 0; j--) {
                    mp[key++] = {i, j, a[i][j]};
                }
            }
        }
        return bfs(n - 1, 0, 1);
    }
};