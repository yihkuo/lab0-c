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
    // n->value = malloc(sizeof(char) * (strlen(s) + 1));
    // strncpy(n->value, s, strlen(s) + 1);
    char *temp = strdup(s);
    if (temp)
        n->value = temp;
    else {
        free(n);
        free(temp);
        return false;
    }
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
    // n->value = malloc(sizeof(char) * (strlen(s) + 1));
    char *temp = strdup(s);
    if (temp)
        n->value = temp;
    else {
        free(n);
        free(temp);
        return false;
    }
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
    strncpy(sp, h->value, bufsize);
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
    struct list_head *fast = head->next;
    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    element_t *mid = list_entry(slow, element_t, list);
    list_del(slow);
    q_release_element(mid);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *node, *safe;
    bool last_dup = false;
    list_for_each_safe (node, safe, head) {
        element_t *cur = list_entry(node, element_t, list);
        if (strcmp(cur->value, list_entry(safe, element_t, list)->value) == 0) {
            list_del(node);
            q_release_element(cur);
            last_dup = true;
        } else if (last_dup) {
            list_del(node);
            q_release_element(cur);
            last_dup = false;
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
    if (!head || list_empty(head))
        return;
    struct list_head *cur, *safe;
    list_for_each_safe (cur, safe, head) {
        list_move(cur, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;
    struct list_head *cur, *safe, *cut;
    LIST_HEAD(tmp);
    int count = 0;

    list_for_each_safe (cur, safe, head) {
        list_move(cur, &tmp);
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

void swap(struct list_head **node)
{
    struct list_head *left = *node;
    struct list_head *right = left->next;
    left->next = right->next;
    right->next = left;
    right->prev = left->prev;
    left->prev = right;
    *node = right;
}

int merge(struct list_head *l, struct list_head *r, bool descend)
{
    if (!l || !r)
        return 0;
    LIST_HEAD(temp);
    int cnt = 0;
    while (!list_empty(l) && !list_empty(r)) {
        element_t *node_l = list_first_entry(l, element_t, list);
        element_t *node_r = list_first_entry(r, element_t, list);
        if ((strcmp(node_l->value, node_r->value) <= 0) ^ descend) {
            list_move_tail(&node_l->list, &temp);
        } else {
            list_move_tail(&node_r->list, &temp);
        }
        cnt++;
    }
    cnt += q_size(l) + q_size(r);
    list_splice(&temp, l);
    list_splice_tail_init(r, l);
    return cnt;
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *mid, *left, *right;
    left = right = head;
    do {
        left = left->next;
        right = right->prev;
    } while (left != right && left->next != right);
    mid = left;
    LIST_HEAD(second);
    list_cut_position(&second, mid, head->prev);

    q_sort(head, descend);
    q_sort(&second, descend);
    merge(head, &second, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    int cnt = 1;
    element_t *cur = list_last_entry(head, element_t, list);
    while (cur->list.prev != head) {
        element_t *pre = list_last_entry(&cur->list, element_t, list);
        if (strcmp(pre->value, cur->value) > 0) {
            list_del(&pre->list);
            q_release_element(pre);
        } else {
            cur = pre;
            cnt++;
        }
    }
    return cnt;
}
/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    int cnt = 1;
    element_t *cur = list_last_entry(head, element_t, list);
    while (cur->list.prev != head) {
        element_t *pre = list_last_entry(&cur->list, element_t, list);
        if (strcmp(pre->value, cur->value) < 0) {
            list_del(&pre->list);
            q_release_element(pre);
        } else {
            cur = pre;
            cnt++;
        }
    }
    return cnt;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return q_size(list_first_entry(head, queue_contex_t, chain)->q);
    int queue_size = 0;
    queue_contex_t *first, *second;
    first = list_first_entry(head, queue_contex_t, chain),
    second = list_entry(first->chain.next, queue_contex_t, chain);
    while (second->q) {
        queue_size = merge(first->q, second->q, descend);
        second->q = NULL;
        list_move_tail(&second->chain, head);
        second = list_entry(first->chain.next, queue_contex_t, chain);
    }
    return queue_size;
}
