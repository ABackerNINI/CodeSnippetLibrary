#include <string.h>
#include <assert.h>
#include "list.h"

/***** LIST NODE TYPE *****/

#if (_LIST_HIDE_NODE_TYPE)
struct _list_node_t_ {
    ListElemType data;
    struct _list_node_t_ *next;
};
#endif

/***** LIST TYPE *****/

#if (_LIST_HIDE_LIST_TYPE)
typedef struct _list_t_ {
    ListNode first;
    ListNode last;
    size_t size;
} list_t;
#endif

/***** STATIC FUNC *****/

static ListNode ListGetNode(List l, size_t pos);
static ListNode ListGetNode2(ListNode node, size_t pos);

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

void list_merge(List dest, List source) {
    if (source->first != NULL) {
        if (dest->first == NULL) {
            dest->first = source->first;
            dest->last = source->last;
            dest->size = source->size;
        } else {
            dest->last->next = source->first;
            dest->last = source->last;
            dest->size += source->size;
        }
        source->first = source->last = NULL;
        source->size = 0;
    }
}

void list_sort(List l, list_elem_cmp_func func) {
    // TODO
}

size_t list_sort_insert(List l, ListElemType val, list_elem_cmp_func func) {
    size_t pos;
    ListNode node, new_node;

    if (l->first == NULL || func(&l->first->data, &val) <= 0) {
        list_push_front(l, val);
        return 0;
    } else if (func(&l->last->data, &val) > 0) {
        list_push_back(l, val);
        return l->size - 1;
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
            return pos;
        }
        ++pos;
        node = node->next;
    }

    assert(0); /* Control should never reach here */
    return l->size + 1;
}

size_t list_sort_insert2(List l, ListElemType val, list_elem_cmp_func func) {
    size_t pos;
    ListNode node, new_node;

    if (l->first == NULL || func(&l->first->data, &val) < 0) {
        list_push_front(l, val);
        return 0;
    } else if (func(&l->last->data, &val) >= 0) {
        list_push_back(l, val);
        return l->size - 1;
    }

    new_node = (ListNode)malloc(sizeof(list_node_t));
    new_node->data = val;

    pos = 1;
    node = l->first;
    while (node->next) {
        if (func(&node->next->data, &val) < 0) {
            new_node->next = node->next;
            node->next = new_node;
            ++l->size;
            return pos;
        }
        ++pos;
        node = node->next;
    }

    assert(0); /* Control should never reach hear */
    return l->size + 1;
}

void list_sort_merge(List dest, List source, list_elem_cmp_func func) {
    // TODO
}

void list_clear(List l) {
    // TODO
}

/***** ACCESS FUNC *****/

ListElemType list_front(List l) { return l->first->data; }

ListElemType list_back(List l) { return l->last->data; }

ListElemType *list_front_ref(List l) { return &l->first->data; }

ListElemType *list_back_ref(List l) { return &l->last->data; }

size_t list_size(List l) { return l->size; }

bool list_empty(List l) { return l->size == 0; }

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

List list_copy(List l) {
    // TODO
}

/***** ITERATOR FUNC *****/

#if (_LIST_ENABLE_ITERATOR)
list_iterator list_begin(List l) { return (list_iterator)l->first; }

list_iterator list_end(/* List l */) { return NULL; }

ListElemType list_data(list_iterator iter) { return iter->data; }

ListElemType *list_data_ref(list_iterator iter) { return &iter->data; }

list_iterator list_next(list_iterator iter) {
    return (list_iterator)iter->next;
}

list_iterator list_find(List l, ListElemType val, list_elem_cmp_func func) {
    ListNode node;

    node = l->first;
    while (node) {
        if (func(&val, &node->data) == 0) {
            return (list_iterator)node;
        }
        node = node->next;
    }
    return NULL;
}

list_iterator list_find2(list_iterator iter, ListElemType val,
                         list_elem_cmp_func func) {
    ListNode node;

    node = iter;
    while (node) {
        if (func(&val, &node->data) == 0) {
            return (list_iterator)node;
        }
        node = node->next;
    }
    return NULL;
}

void list_swap(list_iterator iter1, list_iterator iter2) {
    ListElemType val;

    val = iter1->data;
    iter1->data = iter2->data;
    iter2->data = val;
}

void list_insert_after(List l, list_iterator iter, ListElemType val) {
    ListNode new_node;

    if (iter == NULL) {
        list_push_front(l, val);
    } else if (iter == l->last) {
        list_push_back(l, val);
    } else {
        new_node = (ListNode)malloc(sizeof(list_node_t));
        new_node->data = val;
        new_node->next = iter->next;
        iter->next = new_node;
        ++l->size;
    }
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

void list_clear2(List l, list_elem_destory_func func) {}
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
    } else if (pos == l->size) {
        list_push_back(l, val);
    } else {
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

void list_swap_pos(List l, size_t pos1, size_t pos2) {
    size_t tmp;
    ListNode fa1, fa2;

    if (pos1 == pos2) {
        return;
    }

    if (pos2 < pos1) {
        tmp = pos1;
        pos1 = pos2;
        pos2 = tmp;
    }

    fa1 = ListGetNode(l, pos1 - 1);
    fa2 = ListGetNode2(fa1, pos2 - pos1);

    if (pos1 + 1 == pos2) {
        fa1->next = fa2->next;
        fa2->next = fa2->next->next;
        fa2->next->next = fa2;
        return;
    }

    // TODO
}

#if (_LIST_ENABLE_ITERATOR)
void list_swap2(List l, list_iterator iter1, list_iterator iter2) {
    // TODO
}

void list_insert_before(List l, list_iterator iter, ListElemType val) {
    ListNode node, new_node;

    if (iter == NULL) {
        list_push_back(l, val);
    } else if (iter == l->first) {
        list_push_front(l, val);
    } else {
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
}
#endif

#if (_LIST_ENABLE_UDEDF)
void list_pop_back2(List l, list_elem_destory_func func) {
    func(&l->last->data);
    list_pop_back(l);
}
#endif

/***** STATIC FUNC *****/

static ListNode ListGetNode(List l, size_t pos) {
    return ListGetNode2(l->first, pos);
}

static ListNode ListGetNode2(ListNode node, size_t pos) {
    while (pos--) {
        node = node->next;
    }
    return node;
}
