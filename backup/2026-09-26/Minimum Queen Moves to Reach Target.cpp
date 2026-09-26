// Date: 2026-09-26
// Problem: Minimum Queen Moves to Reach Target
// Link: https://leetcode.com/problems/minimum-queen-moves-to-reach-target/
// Code:

class Solution {
public:
    int minQueenMoves(vector<int>& s, vector<int>& t) {
        // its simple
        int x1=s[0] , y1=s[1] , x2 = t[0] , y2= t[1];
        if(s == t) return 0;
        if(x1 == x2 || y1 == y2 || abs(x1-x2) == abs(y1-y2)){
            return 1;
        } else  {
            return 2;
        }
    }
};