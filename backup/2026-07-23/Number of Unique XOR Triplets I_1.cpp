// Date: 2026-07-23
// Problem: Number of Unique XOR Triplets I
// Link: https://leetcode.com/problems/number-of-unique-xor-triplets-i/
// Code:

class Solution {
public:
    int uniqueXorTriplets(vector<int>& a) {
        int n = a.size();
        if(n == 1){
            return 1;
        }else if(n == 2){
            return 2;
        } else if(n == 3){
            return 4;
        } else {
            int v = log2(n);
            return (1 << (v+1));
        }
        
    }
};