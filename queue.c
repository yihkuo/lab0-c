#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *q = malloc(sizeof(struct list_head));
    if (!q)
        return NULL;
    INIT_LIST_HEAD(q);
    return q;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *cur, *safe;
    list_for_each_safe (cur, safe, head) {
        q_release_element(list_entry(cur, element_t, list));
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *n = malloc(sizeof(element_t));
    if (!n)
        return false;
    n->value = malloc(sizeof(char) * (strlen(s) + 1));
    strncpy(n->value, s, strlen(s) + 1);
    list_add(&(n->list), head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *n = malloc(sizeof(element_t));
    if (!n)
        return false;
    n->value = malloc(sizeof(char) * (strlen(s) + 1));
    strncpy(n->value, s, strlen(s) + 1);
    list_add_tail(&(n->list), head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *h = list_first_entry(head, element_t, list);
    // bufsize =(strlen(h->value)+1);
    // sp = strdup(h->value);
    // sp = malloc(sizeof(char)*bufsize);
    strncpy(sp, h->value, bufsize);
    sp[bufsize - 1] = '\0';
    // list_del(head);
    list_del(&h->list);
    return h;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *h = list_last_entry(head, element_t, list);
    strncpy(h->value, sp, bufsize);
    // list_del(head);
    sp[bufsize - 1] = '\0';
    list_del(&h->list);
    return h;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int count = 0;
    struct list_head *it;
    list_for_each (it, head) {
        count++;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *slow = head->next;
    struct list_head *fast = head->next->next;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next->next;
    }
    element_t *mid = list_entry(slow, element_t, list);
    printf("%s\n", mid->value);
    q_release_element(mid);
    list_del(slow);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *tmp = head;
    struct list_head *cur, *safe;

    list_for_each_safe (cur, safe, head) {
        if (tmp == cur) {
            q_release_element(list_entry(cur, element_t, list));
            list_del(cur);
        } else {
            tmp = cur;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    q_reverseK(head, 2);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *cur, *safe;
    list_for_each_safe (cur, safe, head)
        list_move(cur, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    struct list_head *cur, *safe, *cut;
    LIST_HEAD(tmp);
    int count = 0;

    list_for_each_safe (cur, safe, head) {
        // element_t *t = list_entry(cur,element_t,list);

        list_move(cur, &tmp);
        // printf("%s\n",t->value);
        if (++count == k) {
            cut = safe->prev;
            count = 0;
            list_splice_init(&tmp, cut);
            // tmp.next = &tmp;
            // tmp.prev = &tmp;
        }
    }
    if (count != 0) {
        list_for_each_safe (cur, safe, &tmp) {
            list_move_tail(cur, head);
        }
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
