// Date: 2026-08-02
// Problem: Maximize Pair Strength Using GCD
// Link: https://leetcode.com/problems/maximize-pair-strength-using-gcd/
// Code:

typedef long long ll;
class Solution {
public:
    long long maxPairStrength(vector<int>& a) {
        ll n = a.size() , maxi = -1;
        for(ll i = 0 ; i < n ; i++){
            for(ll j = i+1 ; j < n ; j++){
                ll val = 1LL*__gcd(a[i],a[j])*__gcd(a[i],a[j]);
                maxi = max(maxi,(1LL*a[i]*a[j])/val);
            }
        }
        return maxi;
        
    }
};