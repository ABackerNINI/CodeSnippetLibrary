#ifndef _DLIST_H_
#define _DLIST_H_

#include <stdlib.h>
#include <stdbool.h>

#ifndef _DLIST_HIDE_NODE_TYPE
#define _DLIST_HIDE_NODE_TYPE 1
#endif

#ifndef _DLIST_HIDE_LIST_TYPE
#define _DLIST_HIDE_LIST_TYPE 1
#endif

#ifndef _DLIST_ENABLE_UDEDF
#define _DLIST_ENABLE_UDEDF 1  // USER_DEF_ELEM_DESTORY_FUNC
#endif

#ifndef _DLIST_ENABLE_ITERATOR
#define _DLIST_ENABLE_ITERATOR 1
#endif

typedef void *DListElemType;

#if (_DLIST_ENABLE_UDEDF)
typedef void (*dlist_elem_destory_func)(DListElemType *);
#endif
typedef void (*dlist_foreach_func)(DListElemType *);
typedef void (*dlist_foreach_index_func)(DListElemType *, size_t);
typedef void (*dlist_foreach_extra_func)(DListElemType *, size_t, void *);

#if (_DLIST_HIDE_NODE_TYPE == 0)
typedef struct _dlist_node_t_ {
    DListElemType data;
    struct _dlist_node_t_ *prev;
    struct _dlist_node_t_ *next;
} dlist_node_t;
#else
struct _dlist_node_t_;
typedef struct _dlist_node_t_ dlist_node_t;
#endif

typedef dlist_node_t *DListNode;
typedef dlist_node_t *dlist_iterator;

typedef struct {
    DListNode first;
    DListNode last;
    size_t size;
} dlist_t;

typedef dlist_t *DList;

/***** INIT-DESTORY FUNC *****/

/* Initialize the List. */
void dlist_init(DList l);
/* Destory the List. */
void dlist_destory(DList l);

/***** MODIFIER FUNC *****/

/* Push element 'val' to the front of the List 'l'. */
void dlist_push_front(DList l, DListElemType val);
/* Push element 'val' to the back of the List 'l'. */
void dlist_push_back(DList l, DListElemType val);
/* Pop one element from the front of the List 'l'. */
void dlist_pop_front(DList l);
/* Pop one element from the back of the List 'l'. */
void dlist_pop_back(DList l);

/***** ACCESS FUNC *****/

/* Return the first element in the List 'l'. */
DListElemType dlist_front(DList l);
/* Return the last element in the List 'l'. */
DListElemType dlist_back(DList l);
/* Return a pointer of the first element in the List 'l'. */
DListElemType *dlist_front_ref(DList l);
/* Return a pointer of the last element in the List 'l'. */
DListElemType *dlist_back_ref(DList l);
/* Return number of elements in List 'l'. */
size_t dlist_size(DList l);
/* Return:
    --- true: if List 'l' is empty.
    --- false: if not. */
bool dlist_empty(DList l);
/* For each element in List 'l', call 'func' with element. */
void dlist_foreach(DList l, dlist_foreach_func func);
/* For each element in List 'l', call 'func' with element and its index. */
void dlist_foreach_index(DList l, dlist_foreach_index_func func);
/* For each element in List 'l', call 'func' with element, its index and
 * 'extra'. */
void dlist_foreach_extra(DList l, dlist_foreach_extra_func func, void *extra);

/***** ITERATOR FUNC *****/

#if (_DLIST_ENABLE_ITERATOR)
dlist_iterator dlist_begin(DList l);
dlist_iterator dlist_end(/* DList l */);
DListElemType dlist_data(dlist_iterator iter);
DListElemType *dlist_data_ref(dlist_iterator iter);
dlist_iterator dlist_prev(dlist_iterator iter);
dlist_iterator dlist_next(dlist_iterator iter);
#define _DLIST_FOREACH(list, iter_name)                           \
    for (iter_name = dlist_begin(list); iter_name != dlist_end(); \
         iter_name = dlist_next(iter_name))
#define DLIST_FOREACH(list, iter_name) \
    DListNode iter_name;               \
    _DLIST_FOREACH(list, iter_name)
#endif

/***** UDEDF FUNC *****/

#if (_DLIST_ENABLE_UDEDF)
void dlist_destory2(DList l, dlist_elem_destory_func func);
void dlist_pop_front2(DList l, dlist_elem_destory_func func);
void dlist_pop_back2(DList l, dlist_elem_destory_func func);
#endif

#endif  //_DLIST_H_