// Date: 2026-08-18
// Problem: Find the Largest Almost Missing Integer
// Link: https://leetcode.com/problems/find-the-largest-almost-missing-integer/
// Code:

class Solution {
public:
    int largestInteger(vector<int>& nums, int k) {
        int n = nums.size();

        unordered_map<int, int> cnt;
        for (int i = 0; i + k <= n; i++) {
            unordered_set<int> st;

            for (int j = i; j < i + k; j++) {
                st.insert(nums[j]);
            }
            for (int x : st) {
                cnt[x]++;
            }
        }

        int ans = -1;

        for (auto& [x, freq] : cnt) {
            if (freq == 1) {
                ans = max(ans, x);
            }
        }

        return ans;
    }
};