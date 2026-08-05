// Date: 2026-08-01
// Problem: Minimum Initial Strength to Defeat All Monsters
// Link: https://leetcode.com/problems/minimum-initial-strength-to-defeat-all-monsters/
// Code:

typedef long long ll;
class Solution {
public:
    long long minInitialStrength(vector<int>& m, vector<vector<int>>& a) {
        // use difference array to add how much bonus will be getting on index 
        // and so some checks
        ll n = m.size();
        vector<ll> diff(n+1,0);
        for(auto it : a){
            diff[it[0]] += it[2];
            if(it[1] + 1 < n) diff[it[1]+1] -= it[2];
        } 
        for(ll i = 1 ; i < n ; i++){
            diff[i] += diff[i-1];
        }
        ll start = 0 , ans = 0;
        for(ll i = 0 ; i < n ; i++){
            if(diff[i] < m[i]){
                start = max(start,ans+m[i]-diff[i]);
            }
            ans += m[i];
        }
        return start;
        
    }
};