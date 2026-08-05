// Date: 2026-07-24
// Problem: Number of Unique XOR Triplets II
// Link: https://leetcode.com/problems/number-of-unique-xor-triplets-ii/
// Code:

class Solution {
public:
    int uniqueXorTriplets(vector<int>& a) {
        // n^2 works
        int n = a.size();
        vector<bool> xors(2048, false);
        for (int i = 0; i < n; i++) {
            // now for pair do something smart
            for (int j = 0; j < n; j++) {
                xors[a[i]^a[j]] = true;
            }
        }
        vector<bool> ans(2048, false);
        for(int i = 0 ; i < 2048 ; i++){
            if(xors[i]){
                for(int j = 0 ; j < n ; j++){
                    ans[i^a[j]] = true;
                }
            }
        }
        int cnt = 0;
        for(int i = 0 ; i < ans.size() ; i++){
            if(ans[i]) cnt++;
        }
        return cnt;
    }
};