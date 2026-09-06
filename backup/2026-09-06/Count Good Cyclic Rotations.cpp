// Date: 2026-09-06
// Problem: Count Good Cyclic Rotations
// Link: https://leetcode.com/problems/count-good-cyclic-rotations/
// Code:

typedef long long ll;
class Solution {
public:
    int countGoodRotations(vector<int>& a) {
        // for each rotation i have to get sum 
        // in O(1)
        int n = a.size();
        if(n == 2){
            int cnt = 0;
            if(a[0] > a[1]) cnt++;
            if(a[1] > a[0]) cnt++;
            return cnt;
        }
        ll left_sum = 0 , right_sum = 0 , ans = 0;
        for(int i = 0 ; i < n/2 ; i++){
            left_sum += a[i];
        }
        for(int i = n/2 ; i < n ; i++){
            right_sum += a[i];
        }
        int i = 0 , j = n/2;
        while(i < n){
           // cout << left_sum << " " << right_sum << '\n';
            if(left_sum > right_sum) ans++;
            left_sum += a[j];
            right_sum -= a[j];
            left_sum -= a[i];
            right_sum += a[i];
            i++;
            j++;
            if(j == n){
                j = 0;
            }
        }
        return ans;
    }
};