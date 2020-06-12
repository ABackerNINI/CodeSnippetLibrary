#ifndef _DLIST_H_
#define _DLIST_H_

#include <stdlib.h>
#include <stdbool.h>

#define _DLIST_HIDE_NODE_TYPE 1
#define _DLIST_HIDE_DLIST_TYPE 0
#define _DLIST_ENABLE_UDEDF 1 /* USER_DEF_ELEM_DESTORY_FUNC */
#define _DLIST_ENABLE_ITERATOR 1

/***** DLIST ELEMENT TYPE *****/

typedef void *DListElemType;

/***** DLIST ELEMENT DESTORY FUNC TYPE *****/

#if (_DLIST_ENABLE_UDEDF)
typedef void (*dlist_elem_destory_func)(DListElemType *);
#endif

/***** DLIST NODE TYPE *****/

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

/***** DLIST TYPE *****/

#if (_DLIST_HIDE_DLIST_TYPE == 0)
typedef struct _Dlist_t_ {
    DListNode first;
    DListNode last;
    size_t size;
} dlist_t;
#else
struct _dlist_t_;
typedef struct _dlist_t_ dlist_t;
#endif

typedef dlist_t *DList;

/***** INSERT COMPARE FUNC TYPE *****/

typedef int (*dlist_insert_cmp_func)(DListElemType *, DListElemType *);

/***** FOREACH FUNC TYPE *****/

typedef void (*dlist_foreach_func)(DListElemType *);
typedef void (*dlist_foreach_index_func)(DListElemType *, size_t);
typedef void (*dlist_foreach_extra_func)(DListElemType *, size_t, void *);

/***** ITERATOR TYPE *****/

#if (_DLIST_ENABLE_ITERATOR)
typedef dlist_node_t *dlist_iterator;
#endif

/* ======================================================================== */

/***** INIT-DESTORY FUNC *****/

/* Initialize the DList, call dlist_destory() to clean up. */
void dlist_init(DList l);
/* Destory the DList 'l', free all nodes. */
void dlist_destory(DList l);

/***** NEW-DELETE FUNC *****/

/* Return an initialized DList, call dlist_delete() to clean up. */
DList dlist_new();
/* Destory the DList 'l', free all nodes and free the memory of the DList. */
void dlist_delete(DList l);

/***** MODIFIER FUNC *****/

/* Push element 'val' to the front of the DList 'l'. */
void dlist_push_front(DList l, DListElemType val);
/* Push element 'val' to the back of the DList 'l'. */
void dlist_push_back(DList l, DListElemType val);
/* Pop one element from the front of the DList 'l'. */
void dlist_pop_front(DList l);
/* Pop one element from the back of the DList 'l'. */
void dlist_pop_back(DList l);
/* Insert element 'val' into the DList 'l', and keep the DList in decsending
 * order.
 * Return the position that the element inserted into.
 */
size_t dlist_sort_insert(DList l, DListElemType val,
                         dlist_insert_cmp_func func);

/***** ACCESS FUNC *****/

/* Return the first element in the DList 'l'. */
DListElemType dlist_front(DList l);
/* Return the last element in the DList 'l'. */
DListElemType dlist_back(DList l);
/* Return a pointer of the first element in the DList 'l'. */
DListElemType *dlist_front_ref(DList l);
/* Return a pointer of the last element in the DList 'l'. */
DListElemType *dlist_back_ref(DList l);
/* Return number of elements in DList 'l'. */
size_t dlist_size(DList l);
/* Return:
    --- true: if DList 'l' is empty.
    --- false: if not. */
bool dlist_empty(DList l);
/* For each element in DList 'l', call 'func' with element. */
void dlist_foreach(DList l, dlist_foreach_func func);
/* For each element in DList 'l', call 'func' with element and its index. */
void dlist_foreach_index(DList l, dlist_foreach_index_func func);
/* For each element in DList 'l', call 'func' with element, its index and
 * 'extra'. */
void dlist_foreach_extra(DList l, dlist_foreach_extra_func func, void *extra);
/* For each element in DList 'l' in reverse order, call 'func' with element. */
void dlist_foreach_r(DList l, dlist_foreach_func func);
/* For each element in DList 'l' in reverse order, call 'func' with element and
 * its index. */
void dlist_foreach_index_r(DList l, dlist_foreach_index_func func);
/* For each element in DList 'l' in reverse order, call 'func' with element, its
 * index and 'extra'. */
void dlist_foreach_extra_r(DList l, dlist_foreach_extra_func func, void *extra);
#if (_DLIST_HIDE_NODE_TYPE == 0 && _DLIST_ENABLE_ITERATOR == 0)
/* Macro version of dlist_foreach().
 * Some version of C compilers force all variables putting at the front of a
 * function. */
#define _DLIST_FOREACH(dlist, node_name)                \
    for (node_name = (dlist)->first; node_name != NULL; \
         node_name = node_name->next)
/* Macro version of dlist_foreach(). */
#define DLIST_FOREACH(dlist, node_name) \
    DListNode node_name;                \
    _DLIST_FOREACH(dlist, node_name)
/* Macro version of dlist_foreach_r().
 * Some version of C compilers force all variables putting at the front of a
 * function. */
#define _DLIST_FOREACH_R(dlist, node_name)             \
    for (node_name = (dlist)->last; node_name != NULL; \
         node_name = node_name->prev)
/* Macro version of dlist_foreach_r(). */
#define DLIST_FOREACH_R(dlist, node_name) \
    DListNode node_name;                  \
    _DLIST_FOREACH_R(dlist, node_name)
#endif

/***** ITERATOR FUNC *****/

#if (_DLIST_ENABLE_ITERATOR)
/* Return iterator of the first element in the DList 'l'. */
dlist_iterator dlist_begin(DList l);
/* Return iterator indicates the end of DList. */
dlist_iterator dlist_end(/* DList l */);
/* Return iterator of the last element in the DList 'l', the first element in
 * reverse order. */
dlist_iterator dlist_rbegin(DList l);
/* Return iterator indicates the end of DList in reverse order. */
dlist_iterator dlist_rend(/* DList l */);
/* Return data of the iterator 'iter'. */
DListElemType dlist_data(dlist_iterator iter);
/* Return a pointer of the data of the iterator 'iter'. */
DListElemType *dlist_data_ref(dlist_iterator iter);
/* Return iterator points to the prev iterator of the iterator 'iter'. */
dlist_iterator dlist_prev(dlist_iterator iter);
/* Return iterator points to the next iterator of the iterator 'iter'. */
dlist_iterator dlist_next(dlist_iterator iter);
/* Insert one element into the DList after 'iter'.
 * If 'iter' is `NULL`, insert the element to the front of the DList. */
void dlist_insert_after(DList l, dlist_iterator iter, DListElemType val);
/* Insert one element into the DList 'l' before 'iter'.
 * If 'iter' is `NULL`, insert the element to the back of the DList. */
void dlist_insert_before(DList l, dlist_iterator iter, DListElemType val);
#ifndef _DLIST_FOREACH
/* Macro version of dlist_foreach().
 * Some version of C compilers force all variables putting at the front of a
 * function. */
#define _DLIST_FOREACH(dlist, iter_name)                           \
    for (iter_name = dlist_begin(dlist); iter_name != dlist_end(); \
         iter_name = dlist_next(iter_name))
/* Macro version of dlist_foreach(). */
#define DLIST_FOREACH(dlist, iter_name) \
    DListNode iter_name;                \
    _DLIST_FOREACH(dlist, iter_name)
/* Macro version of dlist_foreach_r().
 * Some version of C compilers force all variables putting at the front of a
 * function. */
#define _DLIST_FOREACH_R(dlist, iter_name)                           \
    for (iter_name = dlist_rbegin(dlist); iter_name != dlist_rend(); \
         iter_name = dlist_prev(iter_name))
/* Macro version of dlist_foreach_r(). */
#define DLIST_FOREACH_R(dlist, iter_name) \
    DListNode iter_name;                  \
    _DLIST_FOREACH_R(dlist, iter_name)
#endif
#endif

/***** UDEDF FUNC *****/

#if (_DLIST_ENABLE_UDEDF)
/* Destory the DList 'l', free all nodes. */
void dlist_destory2(DList l, dlist_elem_destory_func func);
/* Destory the DList 'l', free all nodes and free the memory of the DList. */
void dlist_pop_front2(DList l, dlist_elem_destory_func func);
/* Pop one element from the front of the DList 'l'. */
void dlist_pop_back2(DList l, dlist_elem_destory_func func);
#endif

/***** NOT-RECOMMENDED FUNC *****/

/* Insert one element into the DList at position 'pos'.
 * !!!Bad choice!!! Use dlist_insert_after() or dlist_insert_before() instead if
 * dlist iterator is enabled.
 */
void dlist_insert(DList l, size_t pos, DListElemType val);
/* Return the element at position 'pos' in the DList 'l'.
 * !!!Bad choice!!! Use dlist iterator instead if dlist iterator is enabled. */
DListElemType dlist_at(DList l, size_t pos);
/* Return pointer of the element at position 'pos' in the DList 'l'.
 * !!!Bad choice!!! Use dlist iterator instead if dlist iterator is enabled. */
DListElemType *dlist_at_ref(DList l, size_t pos);

#endif  //_DLIST_H_
