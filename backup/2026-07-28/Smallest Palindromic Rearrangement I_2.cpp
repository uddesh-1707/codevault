// Date: 2026-07-28
// Problem: Smallest Palindromic Rearrangement I
// Link: https://leetcode.com/problems/smallest-palindromic-rearrangement-i/
// Code:

class Solution {
public:
    string smallestPalindrome(string s) {
        int n = s.length();
        vector<int> freq(26,0);
        for(int i = 0 ; i < n ; i++){
            freq[s[i]-'a']++;
        }
        string ans;
        for(int i = 0 ; i < n ; i++){
            ans +=  '.';
        }
        // only 1 char will be odd number of times , other will be even 
        // number of times
        int lo = 0 , hi = n-1;
        for(int i = 0 ; i < 26 ; i++){
            if(freq[i] % 2 == 0){
                while(freq[i] > 0){
                    ans[lo] = i+'a';
                    ans[hi] = i+'a';
                    lo++;
                    hi--;
                    freq[i] -= 2;
                }
            } else {
                while(freq[i] > 1){
                    ans[lo] = i+'a';
                    ans[hi] = i+'a';
                    lo++;
                    hi--;
                    freq[i] -= 2;
                }
            }
        }
        for(int i = 0 ; i < 26 ; i++){
            if(freq[i] > 0){
                ans[lo] = i+'a';
            }
        }
        return ans;


        
    }
};