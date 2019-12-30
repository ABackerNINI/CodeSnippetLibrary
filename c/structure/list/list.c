#include <string.h>
#include "list.h"

#if (_LIST_HIDE_NODE_TYPE)
struct _list_node_t_ {
    ListElemType data;
    struct _list_node_t_ *next;
};
#endif

#if (_LIST_HIDE_LIST_TYPE)
typedef struct _list_t_ {
    ListNode first;
    ListNode last;
    size_t size;
} list_t;
#endif

/***** INIT-DESTORY FUNC *****/

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

/***** NEW-DELETE FUNC *****/

List list_new() {
    List l;

    l = (List)malloc(sizeof(list_t));
    list_init(l);

    return l;
}

void list_delete(List l) {
    list_destory(l);
    free(l);
}

/***** MODIFIER FUNC *****/

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

size_t list_sort_insert(List l, ListElemType val, list_insert_cmp_func func) {
    size_t pos;
    ListNode node, new_node;

    if (l->first == NULL || func(&l->first->data, &val) <= 0) {
        list_push_front(l, val);
        return 0;
    } else if (func(&l->last->data, &val) > 0) {
        list_push_back(l, val);
        return list_size(l) - 1;
    }

    new_node = (ListNode)malloc(sizeof(list_node_t));
    new_node->data = val;

    pos = 1;
    node = l->first;
    while (node->next) {
        if (func(&node->next->data, &val) <= 0) {
            new_node->next = node->next;
            node->next = new_node;
            ++l->size;
            break;
        }
        ++pos;
        node = node->next;
    }

    return pos;
}

/***** ACCESS FUNC *****/

ListElemType list_front(List l) { return l->first->data; }

ListElemType list_back(List l) { return l->last->data; }

ListElemType *list_front_ref(List l) { return &l->first->data; }

ListElemType *list_back_ref(List l) { return &l->last->data; }

size_t list_size(List l) { return l->size; }

bool list_empty(List l) { return list_size(l) == 0; }

void list_foreach(List l, list_foreach_func func) {
    ListNode node;

    node = l->first;
    while (node) {
        func(&node->data);
        node = node->next;
    }
}

void list_foreach_index(List l, list_foreach_index_func func) {
    size_t index;
    ListNode node;

    index = 0;
    node = l->first;
    while (node) {
        func(&node->data, index++);
        node = node->next;
    }
}

void list_foreach_extra(List l, list_foreach_extra_func func, void *extra) {
    size_t index;
    ListNode node;

    index = 0;
    node = l->first;
    while (node) {
        func(&node->data, index++, extra);
        node = node->next;
    }
}

/***** ITERATOR FUNC *****/

#if (_LIST_ENABLE_ITERATOR)
list_iterator list_begin(List l) { return l->first; }

list_iterator list_end(/* List l */) { return NULL; }

ListElemType list_data(list_iterator iter) { return iter->data; }

ListElemType *list_data_ref(list_iterator iter) { return &iter->data; }

list_iterator list_next(list_iterator iter) { return iter->next; }

void list_insert_after(List l, list_iterator iter, ListElemType val) {
    ListNode new_node;

    if (iter == NULL) {
        list_push_front(l, val);
        return;
    } else if (iter == l->last) {
        list_push_back(l, val);
        return;
    }

    new_node = (ListNode)malloc(sizeof(list_node_t));
    new_node->data = val;
    new_node->next = iter->next;
    iter->next = new_node;
    ++l->size;
}
#endif

/***** UDEDF FUNC *****/

#if (_LIST_ENABLE_UDEDF)
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

void list_delete2(List l, list_elem_destory_func func) {
    list_delete2(l, func);
    free(l);
}

void list_pop_front2(List l, list_elem_destory_func func) {
    func(&l->first->data);
    list_pop_front(l);
}

void list_pop_back2(List l, list_elem_destory_func func) {
    func(&l->last->data);
    list_pop_back(l);
}
#endif

/***** NOT-RECOMMENDED FUNC *****/

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

void list_insert(List l, size_t pos, ListElemType val) {
    ListNode node, new_node;

    if (pos == 0) {
        list_push_front(l, val);
        return;
    } else if (pos == list_size(l)) {
        list_push_back(l, val);
        return;
    }

    node = l->first;
    while (--pos) {
        node = node->next;
    }

    new_node = (ListNode)malloc(sizeof(list_node_t));
    new_node->data = val;
    new_node->next = node->next;
    node->next = new_node;
    ++l->size;
}

ListElemType list_at(List l, size_t pos) {
    ListNode node;

    node = l->first;
    while (pos--) {
        node = node->next;
    }
    return node->data;
}

ListElemType *list_at_ref(List l, size_t pos) {
    ListNode node;

    node = l->first;
    while (pos--) {
        node = node->next;
    }
    return &node->data;
}

#if (_LIST_ENABLE_ITERATOR)
void list_insert_before(List l, list_iterator iter, ListElemType val) {
    ListNode node, new_node;

    if (iter == NULL) {
        list_push_back(l, val);
        return;
    } else if (iter == l->first) {
        list_push_front(l, val);
        return;
    }

    new_node = (ListNode)malloc(sizeof(list_node_t));
    new_node->data = val;

    node = l->first;
    while (node->next != iter) {
        node = node->next;
    }

    new_node->next = node->next;
    node->next = new_node;
    ++l->size;
}
#endif

#if (_LIST_ENABLE_UDEDF)
void list_pop_back2(List l, list_elem_destory_func func) {
    func(&l->last->data);
    list_pop_back(l);
}
#endif
