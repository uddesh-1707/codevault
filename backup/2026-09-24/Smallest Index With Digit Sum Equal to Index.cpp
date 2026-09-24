// Date: 2026-09-24
// Problem: Smallest Index With Digit Sum Equal to Index
// Link: https://leetcode.com/problems/smallest-index-with-digit-sum-equal-to-index/
// Code:

class Solution {
public:
    int smallestIndex(vector<int>& a) {
        int n = a.size() , idx = -1;
        for(int i = 0 ; i < n ; i++){
            int c = a[i],sum=0;
            while(c > 0){
                sum += c%10;
                c /= 10;
            }
            if(sum == i){
                idx = i;
                break;
            }
        }
        return idx;        
    }
};