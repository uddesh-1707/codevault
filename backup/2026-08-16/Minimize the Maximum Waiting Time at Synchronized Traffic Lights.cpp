// Date: 2026-08-16
// Problem: Minimize the Maximum Waiting Time at Synchronized Traffic Lights
// Link: https://leetcode.com/problems/minimize-the-maximum-waiting-time-at-synchronized-traffic-lights/
// Code:

class Solution {
public:
    bool check(int mid ,int p, vector<int>& at , int maxi){
        // assign all at to lights
        // think to maximize the WT

        for(int i = 0 ; i < at.size() ; i++){
            if(at[i] < maxi) continue;
            if(p-at[i] > mid) return false;
        }
        return true;
    }
    int minPenalty(int p, vector<int>& l, vector<int>& at) {
        // so it is BS on answers
        for(auto &it : at){
            it %= p;
        }
        int maxi = *max_element(l.begin(),l.end());
        int lo = 0 , hi = p-1 , ans = -1; // doubt here for hi
        // first one 0000111
        while(lo <= hi){
            int mid = lo + (hi-lo)/2;
            if(check(mid,p,at,maxi)){
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        return ans;
    }
};