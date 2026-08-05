// Date: 2026-08-01
// Problem: Widest Possible Fence
// Link: https://leetcode.com/problems/widest-possible-fence/
// Code:

class Solution {
public:
    int maximumWidth(vector<int>& a) {

        int n = a.size();
        map<int,int> freq,mp;
        for(int i = 0 ; i < n ; i++) freq[a[i]]++;
        vector<int> dis;
        for(auto [f,s] : freq){
            dis.push_back(f);
            mp[f] += s;
        }
        for(int i = 0 ; i < dis.size() ; i++){
            for(int j = i ; j < dis.size() ; j++){
                int sum = dis[i]+dis[j];
                if(i == j){
                    mp[sum] += freq[dis[i]]/2;
                } else {
                    mp[sum] += min(freq[dis[i]],freq[dis[j]]);
                 }
            }
            
        }
        int ans = 0;
        for(auto it : mp){
            ans = max(ans,it.second);
        }
        return ans;
    }
};