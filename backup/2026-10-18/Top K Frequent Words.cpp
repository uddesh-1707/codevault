// Date: 2026-10-18
// Problem: Top K Frequent Words
// Link: https://leetcode.com/problems/top-k-frequent-words/
// Code:

class Solution {
public:
    struct cmp {
        bool operator()(pair<int, string>& a, pair<int, string>& b) {
            if (a.first == b.first)
                return a.second > b.second;

            return a.first < b.first;
        }
    };
    vector<string> topKFrequent(vector<string>& a, int k) {
        int n = a.size();
        unordered_map<string, int> mp;
        for (int i = 0; i < n; i++) {
            mp[a[i]]++;
        }
        priority_queue<pair<int, string>, vector<pair<int, string>>, cmp> pq;
        for (auto [f, s] : mp) {
            pq.push({s, f});
        }
        vector<string> ans;
        for (int i = 0; i < k; i++) {
            ans.push_back(pq.top().second);
            pq.pop();
        }
        return ans;
    }
};