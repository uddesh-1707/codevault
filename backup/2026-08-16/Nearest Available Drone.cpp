// Date: 2026-08-16
// Problem: Nearest Available Drone
// Link: https://leetcode.com/problems/nearest-available-drone/
// Code:

class Solution {
public:
    int nearestDrone(vector<vector<int>>& a, vector<int>& t) {
        int n = a.size() , ans = INT_MAX , idx = -1;;
        for(int i = 0 ; i < n ; i++){
            int dis = abs(a[i][0]-t[0]) + abs(a[i][1]-t[1]);
            if(dis < ans && dis <= a[i][2]){
                ans = dis;
                idx = i;
            }
        }
        return idx;
    }
};