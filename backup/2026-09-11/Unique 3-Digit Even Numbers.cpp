// Date: 2026-09-11
// Problem: Unique 3-Digit Even Numbers
// Link: https://leetcode.com/problems/unique-3-digit-even-numbers/
// Code:

class Solution {
public:
    int totalNumbers(vector<int>& a) {
        // nC3 --> calculate with condition
        int freq[10] = {0};
        for (int d : a) freq[d]++;
        int count = 0;

        for(int h = 1; h <= 9; h++){
            if (freq[h] == 0) continue;
            freq[h]--;
            for(int t = 0; t <= 9; t++){
                if (freq[t] == 0) continue;
                freq[t]--;
                for(int u = 0; u <= 8; u += 2){
                    if (freq[u] > 0) count++;
                }
                freq[t]++;
            }
            freq[h]++;
        }
        return count;
    }
};