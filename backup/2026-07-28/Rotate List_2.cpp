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
        if(k == 0 || head == NULL || head->next == NULL){
            return head;
        }
        ListNode* curr = head;
        int n = 1;
        while(curr->next != NULL){
            n++;
            curr = curr->next;

        }
        curr->next = head;
        ListNode* newcurr = head;
        k %= n;
        int steps = n-k;
        for(int i = 1 ; i < steps ; i++){ // steps-1
            newcurr = newcurr->next;
        }
        ListNode* newnode = newcurr->next;
        newcurr->next = NULL;
        return newnode;
    }
};