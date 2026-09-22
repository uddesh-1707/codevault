// Date: 2026-09-22
// Problem: Top K Frequent Elements
// Link: https://leetcode.com/problems/top-k-frequent-elements/
// Code:

class Solution {
public:
    vector<int> topKFrequent(vector<int>& a, int k) {
        int n = a.size();
        map<int,int> mp;
        for(int i = 0 ; i < n ; i++){
            mp[a[i]]++;
        }
        priority_queue<pair<int,int>> pq;
        vector<int> ans;
        for(auto [f,s] : mp){
            pq.push({s,f});
        }
        for(int i = 0 ; i < k ; i++){
            ans.push_back(pq.top().second);
            pq.pop();
        }
        return ans;

    }
};