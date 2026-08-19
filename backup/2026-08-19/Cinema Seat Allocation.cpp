// Date: 2026-08-19
// Problem: Cinema Seat Allocation
// Link: https://leetcode.com/problems/cinema-seat-allocation/
// Code:

class Solution {
public:
    int maxNumberOfFamilies(int n, vector<vector<int>>& r) {
        map<int, set<int>> mp;

        for (auto& x : r)
            mp[x[0]].insert(x[1]);

        int ans = 2 * (n - mp.size());

        for (auto& [row, seats] : mp) {
            bool left = true;
            bool middle = true;
            bool right = true;

            for (int x : seats) {
                if (x >= 2 && x <= 5)
                    left = false;
                if (x >= 4 && x <= 7)
                    middle = false;
                if (x >= 6 && x <= 9)
                    right = false;
            }

            if (left && right)
                ans += 2;
            else if (left || middle || right)
                ans += 1;
        }

        return ans;
    }
};