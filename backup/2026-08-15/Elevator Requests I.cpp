// Date: 2026-08-15
// Problem: Elevator Requests I
// Link: https://leetcode.com/problems/elevator-requests-i/
// Code:

class Solution {
public:
    int elevatorRequests(int n, vector<int>& a) {
        int ans = 0 , curr = 0;
        for(int i = 0 ; i < a.size() ; i++){
            if(curr != a[i]){
                ans += abs(curr-a[i]);
                curr = a[i];
            }
        }
        return ans;
        
    }
};