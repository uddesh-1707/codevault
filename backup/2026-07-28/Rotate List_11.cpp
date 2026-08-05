// Date: 2026-07-28
// Problem: Rotate List
// Link: https://leetcode.com/problems/rotate-list/
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
    ListNode* rotateRight(ListNode* head, int k) {
        if(k == 0 || head == NULL){
            return head;
        }
        ListNode* curr = head;
        int n = 0;
        vector<int> a , t;
        while(curr != NULL){
            n++;
            a.push_back(curr->val);
            curr = curr->next;

        }
        t.resize(n);
        k %= n;
        for(int i = 0 ; i < n ; i++){
            t[(i+k)%n] = a[i];
        }
        curr = head;
        int idx = 0;
        while(curr != NULL){
            curr->val = t[idx];
            idx++;
            curr = curr->next;
        }
        return head;


        
    }
};