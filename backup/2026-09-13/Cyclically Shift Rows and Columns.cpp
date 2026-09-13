// Date: 2026-09-13
// Problem: Cyclically Shift Rows and Columns
// Link: https://leetcode.com/problems/cyclically-shift-rows-and-columns/
// Code:

class Solution {
public:
    vector<vector<int>> cyclicShift(int n, vector<vector<int>>& grid, vector<int>& rowShift, vector<int>& colShift) {

        vector<vector<int>> temp =  grid;
        for(int i = 0 ; i < n ; i++){
            int shift = rowShift[i];
            for(int j = 0 ; j < n ; j++){
                grid[i][j] = temp[i][(j+shift)%n];
            }
        }
        temp = grid;
        for(int j = 0 ; j < n ; j++){
            int shift = colShift[j];
            for(int i = 0 ; i < n ; i++){
                grid[i][j] = temp[(i+shift)%n][j];
            }
        }
        return grid;
    }
};