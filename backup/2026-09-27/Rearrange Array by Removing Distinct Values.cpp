// Date: 2026-09-27
// Problem: Rearrange Array by Removing Distinct Values
// Link: https://leetcode.com/problems/rearrange-array-by-removing-distinct-values/
// Code:

class Solution {
public:
    vector<int> rearrangeArray(vector<int>& a) {
        int n = a.size();
        map<int, int> mp;
        for (int i = 0; i < n; i++) {
            mp[a[i]]++;
        }
        vector<int> ans;
        while (mp.size() > 0) {
            for (auto it = mp.begin(); it != mp.end();) {
                ans.push_back(it->first);
                it->second--;
                if (it->second == 0) {
                    it = mp.erase(it);
                } else {
                    ++it;
                }
            }
        }
        return ans;
    }
};