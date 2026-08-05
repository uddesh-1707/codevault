// Date: 2026-07-25
// Problem: Reverse Linked List II
// Link: https://leetcode.com/problems/reverse-linked-list-ii/
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
    ListNode* reverseBetween(ListNode* head, int left, int right) {
       if(head->next == NULL || left == right){
        return head;
       }
       ListNode* dummy = new ListNode(0);
       dummy->next = head;

       ListNode* prev_left = dummy;
       // left-1 jumps
       for(int i = 1 ; i < left ; i++){
        prev_left = prev_left->next;
       }
       ListNode* prev = NULL;
       ListNode* curr = prev_left->next;
       for(int i = 0 ; i <= right-left ; i++){
        ListNode* nexxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nexxt;
       }
       ListNode* left_node = prev_left->next;
       prev_left->next = prev;
       left_node->next = curr;
       return dummy->next;


    }
};