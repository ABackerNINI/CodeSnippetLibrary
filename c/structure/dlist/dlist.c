#include <string.h>
#include "dlist.h"

#if (_DLIST_HIDE_NODE_TYPE)
struct _dlist_node_t_ {
    DListElemType data;
    struct _dlist_node_t_ *prev;
    struct _dlist_node_t_ *next;
};
#endif

void dlist_init(DList l) {
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
}

void dlist_destory(DList l) {
    DListNode node, next;

    node = l->first;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
}

void dlist_push_front(DList l, DListElemType val) {
    DListNode node;

    node = (DListNode)malloc(sizeof(dlist_node_t));
    node->data = val;
    node->prev = NULL;
    node->next = l->first;

    if (l->first == NULL) {
        l->first = l->last = node;
    } else {
        l->first->prev = node;
        l->first = node;
    }
    ++l->size;
}

void dlist_push_back(DList l, DListElemType val) {
    DListNode node;

    node = (DListNode)malloc(sizeof(dlist_node_t));
    node->data = val;
    node->prev = l->last;
    node->next = NULL;

    if (l->first == NULL) {
        l->first = l->last = node;
    } else {
        l->last->next = node;
        l->last = node;
    }
    ++l->size;
}

void dlist_pop_front(DList l) {
    DListNode node;

    node = l->first;
    l->first = l->first->next;
    if (l->first == NULL) {
        l->last = NULL;
    } else {
        l->first->prev = NULL;
    }
    free(node);
    --l->size;
}

void dlist_pop_back(DList l) {
    DListNode node;

    node = l->last;
    l->last = l->last->prev;
    if (l->last == NULL) {
        l->first = NULL;
    } else {
        l->last->next = NULL;
    }
    free(node);
    --l->size;
}

DListElemType dlist_front(DList l) { return l->first->data; }

DListElemType dlist_back(DList l) { return l->last->data; }

DListElemType *dlist_front_ref(DList l) { return &l->first->data; }

DListElemType *dlist_back_ref(DList l) { return &l->last->data; }

size_t dlist_size(DList l) { return l->size; }

bool dlist_empty(DList l) { return dlist_size(l) == 0; }

void dlist_foreach(DList l, dlist_foreach_func func) {
    DListNode node;

    node = l->first;
    while (node) {
        func(&node->data);
        node = node->next;
    }
}

void dlist_foreach_index(DList l, dlist_foreach_index_func func) {
    size_t index;
    DListNode node;

    index = 0;
    node = l->first;
    while (node) {
        func(&node->data, index++);
        node = node->next;
    }
}

void dlist_foreach_extra(DList l, dlist_foreach_extra_func func, void *extra) {
    size_t index;
    DListNode node;

    index = 0;
    node = l->first;
    while (node) {
        func(&node->data, index++, extra);
        node = node->next;
    }
}

#if (_DLIST_ENABLE_ITERATOR)
dlist_iterator dlist_begin(DList l) { return l->first; }

dlist_iterator dlist_end(/* DList l */) { return NULL; }

DListElemType dlist_data(dlist_iterator iter) { return iter->data; }

DListElemType *dlist_data_ref(dlist_iterator iter) { return &iter->data; }

dlist_iterator dlist_prev(dlist_iterator iter) { return iter->prev; }

dlist_iterator dlist_next(dlist_iterator iter) { return iter->next; }
#endif

#if (_DLIST_ENABLE_UDEDF)
void dlist_destory2(DList l, dlist_elem_destory_func func) {
    DListNode node, next;

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

void dlist_pop_front2(DList l, dlist_elem_destory_func func) {
    func(&l->first->data);
    dlist_pop_front(l);
}

void dlist_pop_back2(DList l, dlist_elem_destory_func func) {
    func(&l->last->data);
    dlist_pop_back(l);
}
#endif
