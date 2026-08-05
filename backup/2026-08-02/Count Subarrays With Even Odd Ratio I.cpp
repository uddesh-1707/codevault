// Date: 2026-08-02
// Problem: Count Subarrays With Even Odd Ratio I
// Link: https://leetcode.com/problems/count-subarrays-with-even-odd-ratio-i/
// Code:

class Solution {
public:
    int countRatioSubarrays(vector<int>& ar, int a, int b) {
        // x*b <= y*a
        int n = ar.size() , cnt = 0;
        for(int i = 0 ; i < n ; i++){
            int e = 0 , o = 0;
            if(ar[i]%2 == 0) e++;
            else o++;
            for(int j = i ; j < n ; j++){
                if(i != j){
                    if(ar[j]%2 == 0) e++;
                    else o++;
                }
                if(e*b <= o*a){
                    cnt++;
                }
            }
        }
        return cnt;
    }
};