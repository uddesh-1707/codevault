// Date: 2026-07-28
// Problem: Reverse Nodes in k-Group
// Link: https://leetcode.com/problems/reverse-nodes-in-k-group/
// Code:

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* curr = head;
        vector<int> a;
        while(curr != NULL){
            a.push_back(curr->val);
            curr = curr->next;
        }
        int n = a.size();
        for(int i = 0 ; i + k <= n ; i+=k){
            int l = i , r = i+k-1;
            while(l < r){
                swap(a[l],a[r]);
                l++;
                r--;
            }
        }
        int idx = 0;
        curr = head;
        while(curr != NULL){
            curr->val = a[idx];
            idx++;
            curr = curr ->next;
        }
        return head;

    }
};