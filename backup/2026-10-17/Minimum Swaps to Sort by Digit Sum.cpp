// Date: 2026-10-17
// Problem: Minimum Swaps to Sort by Digit Sum
// Link: https://leetcode.com/problems/minimum-swaps-to-sort-by-digit-sum/
// Code:

class Solution {
public:
    int minSwaps(vector<int>& a) {
        // it's just simulation
        int n = a.size();
        vector<pair<int, int>> vp(n);
        unordered_map<int,int> mp;
        for (int i = 0; i < n; i++) {
            int v = a[i], sum = 0;
            while (v > 0) {
                sum += (v % 10);
                v /= 10;
            }
            vp[i] = {a[i], sum};
            mp[a[i]] = i;
        }
        sort(vp.begin(), vp.end(), [](auto& x, auto& y) {
            if (x.second != y.second)
                return x.second < y.second;
            return x.first < y.first;
        });
        int ans = 0;
        for(int i = 0 ; i < n ; i++){
            int after_sort = vp[i].first;
            int before_sort = a[i];

            if(after_sort == before_sort) continue;

            int idx = mp[after_sort];

            a[i] = after_sort;
            a[idx] = before_sort;

            mp[after_sort] = i;
            mp[before_sort] = idx;

            ans++;
        }
        return ans;
    }
};