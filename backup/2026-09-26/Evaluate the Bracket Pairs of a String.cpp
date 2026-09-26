// Date: 2026-09-26
// Problem: Evaluate the Bracket Pairs of a String
// Link: https://leetcode.com/problems/evaluate-the-bracket-pairs-of-a-string/
// Code:

class Solution {
public:
    string evaluate(string s, vector<vector<string>>& a) {
        int n = a.size();
        unordered_map<string,string> mp;
        for(int i = 0 ; i < n ; i++){
            mp[a[i][0]] = a[i][1];
        }
        // for(auto [f,s] : mp){
        //     cout << f << " " << s << '\n';
        // }
        string ans="";
        for(int i = 0 ; i < s.length() ; i++){
            if(s[i] == '('){
                string curr;
                i++;
                while(i < s.length() && s[i] != ')'){
                    curr += s[i];
                    i++;
                }
                //cout << curr << '\n';
                if(mp.find(curr) != mp.end()){
                    ans += mp[curr];
                } else{
                    ans += '?';
                }
            } else{
                 ans += s[i];
            }
        }
        return ans;
        
    }
};