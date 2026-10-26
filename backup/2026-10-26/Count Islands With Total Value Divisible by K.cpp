// Date: 2026-10-26
// Problem: Count Islands With Total Value Divisible by K
// Link: https://leetcode.com/problems/count-islands-with-total-value-divisible-by-k/
// Code:

class Solution {
public:
    int dx[4] = {0,0,1,-1};
    int dy[4] = {1,-1,0,0};
    long long sum = 0 , n , m;
    bool vis[1001][1001];
    void dfs(vector<vector<int>>& a , int x , int y , int k){
        if(x < 0 || x >= n || y < 0 || y >= m || vis[x][y] || a[x][y] == 0) return;
        vis[x][y] = true;
        sum = (sum + a[x][y] % k) % k;
        for(int i = 0 ; i < 4 ; i++){
            int nx = dx[i] + x;
            int ny = dy[i] + y;
            dfs(a,nx,ny,k);
        }
    }
    int countIslands(vector<vector<int>>& a, int k) {
        // connected component sum must be divisible by k
        n = a.size();
        m = a[0].size();
        memset(vis, false, sizeof(vis));
        int  ans = 0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(!vis[i][j] && a[i][j] > 0){
                    sum = 0;
                    dfs(a,i,j,k);
                    if(sum == 0) ans++;
                }
            }
        }
        return ans;
    }
};