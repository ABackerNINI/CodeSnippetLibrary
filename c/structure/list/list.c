#include "list.h"

#include <string.h>

void list_init(List l) {
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
}

void list_destory(List l) {
    ListNode node, next;

    node = l->first;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
}

void list_destory2(List l, list_elem_destory_func func) {
    ListNode node, next;

    if (func) {
        node = l->first;
        while (node) {
            next = node->next;
            func(&node->data);
            free(node);
            node = next;
        }
    } else {
        node = l->first;
        while (node) {
            next = node->next;
            free(node);
            node = next;
        }
    }
}

void list_push_front(List l, ListElemType val) {
    ListNode node;

    node = (ListNode)malloc(sizeof(list_node_t));
    node->data = val;
    node->next = l->first;

    if (l->first == NULL) {
        l->first = l->last = node;
    } else {
        l->first = node;
    }
    ++l->size;
}

void list_push_back(List l, ListElemType val) {
    ListNode node;

    node = (ListNode)malloc(sizeof(list_node_t));
    node->data = val;
    node->next = NULL;

    if (l->first == NULL) {
        l->first = l->last = node;
    } else {
        l->last->next = node;
        l->last = node;
    }
    ++l->size;
}

void list_pop_front(List l) {
    ListNode node;

    node = l->first;
    l->first = l->first->next;
    if (l->first == NULL) {
        l->last = NULL;
    }
    free(node);
    --l->size;
}

void list_pop_front2(List l, list_elem_destory_func func) {
    func(&l->first->data);
    list_pop_front(l);
}

void list_pop_back(List l) {
    ListNode node;

    if (l->first == l->last) {
        list_pop_front(l);
    } else {
        node = l->first;
        while (node->next != l->last) {
            node = node->next;
        }
        free(l->last);
        node->next = NULL;
        l->last = node;
        --l->size;
    }
}

void list_pop_back2(List l, list_elem_destory_func func) {
    func(&l->last->data);
    list_pop_back(l);
}

ListElemType list_front(List l) { return l->first->data; }

ListElemType list_back(List l) { return l->last->data; }

ListElemType *list_front_ref(List l) { return &l->first->data; }

ListElemType *list_back_ref(List l) { return &l->last->data; }

size_t list_size(List l) { return l->size; }

bool list_empty(List l) { return list_size(l) == 0; }

list_iterator list_begin(List l) { return l->first; }

list_iterator list_end(/* List l */) { return NULL; }

ListElemType list_data(list_iterator iter) { return iter->data; }

ListElemType *list_data_ref(list_iterator iter) { return &iter->data; }

list_iterator list_next(list_iterator iter) { return iter->next; }

void list_foreach(List l, list_foreach_func func) {
    list_iterator iter;

    _LIST_FOREACH(l, iter) { func(list_data_ref(iter)); }
}

void list_foreach_index(List l, list_foreach_index_func func) {
    size_t index;
    list_iterator iter;

    index = 0;
    _LIST_FOREACH(l, iter) { func(list_data_ref(iter), index++); }
}
