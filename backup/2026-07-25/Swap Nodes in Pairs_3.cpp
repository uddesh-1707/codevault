// Date: 2026-07-25
// Problem: Swap Nodes in Pairs
// Link: https://leetcode.com/problems/swap-nodes-in-pairs/
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
    ListNode* swapPairs(ListNode* head) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* before_prev = dummy;
        while (before_prev->next != NULL && before_prev->next->next != NULL) {
            ListNode* prev = before_prev->next;
            ListNode* curr = prev->next;
            ListNode* nexxt = curr->next;
            curr->next = prev;
            prev->next = nexxt;
            before_prev->next = curr;
            before_prev = prev;
        }
        ListNode* ans =  dummy->next;
        delete dummy;
        return ans;

    }
};