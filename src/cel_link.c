/**
 * link interface implemented source file src/cel_link.c
 *
 * @author chenxin<chenxin619315@gmail.com>
 */
#include "cel_link.h"

static link_node_t create_link_node( void *, link_node_t _prev, link_node_t _next );
static void insert_before( link_node_t, void * );
static link_node_t get_node( cel_link_t, uint_t );
//static inline void cel_remove_node( link_node_t );

#define cel_remove_node( node )	\
    node->_prev->_next = node->_next;\
node->_next->_prev = node->_prev;

/**
 * create a new cel link list .
 *
 * @return	cel_link_t
 */
CEL_API cel_link_t new_cel_link( void ) 
{
    cel_link_t ptr = ( cel_link_t ) cel_malloc( sizeof( cel_link_entry ) );
    if ( ptr == NULL ) {
	__ALLOCATION_ERROR__
    }

    ptr->size = 0;
    ptr->tail = create_link_node( NULL, NULL, NULL );
    ptr->head = create_link_node( NULL, NULL, ptr->tail );
    ptr->tail->_prev = ptr->head;

    return ptr;
}

/**
 * create a new link_node_entry .
 *
 * @param	value
 * @param	_prev
 * @param	_next
 * @return	link_node_t
 */
static link_node_t create_link_node( void * value, link_node_t _prev, link_node_t _next) 
{
    link_node_t node = ( link_node_t ) cel_malloc( sizeof( link_node_entry ) );
    if ( node == NULL ) {
	__ALLOCATION_ERROR__
    }

    node->value = value;
    node->_prev = _prev;
    node->_next = _next;

    return node;
}

/**
 * free the specified cel link list,
 * 	the remove callback function maybe pass insite to 
 * finish the extra allocation free work .
 *
 * @param	rmfunc
 */
CEL_API void free_cel_link( cel_link_t link, cel_release_callback_fn_t relfunc ) 
{
    link_node_t node;
    for ( node = link->head->_next;
	    node != link->tail;
	    node = node->_next ) {
	//invoke the callback function if it is not NULL
	if ( relfunc != NULL ) relfunc( node->value );
	cel_free( node );
    }

    cel_free( link->head );
    cel_free( link->tail );
    cel_free( link );
}

CEL_API void cel_link_add( cel_link_t link, void * value )
{
    insert_before( link->tail, value );
    link->size++;
}

//add a element from the head .
CEL_API void cel_link_add_first( cel_link_t link, void * value ) 
{
    insert_before( link->head->_next, value );
    link->size++;
}

//append element from the tail
CEL_API void cel_link_add_last( cel_link_t link, void * value ) 
{
    insert_before( link->tail, value );
    link->size++;
}

/**
 * insert a element before the specified node .
 *
 * @param	_old
 * @param	value
 */
static void insert_before( link_node_t _old, void * value )
{
    link_node_t o = create_link_node( value, _old->_prev, _old );
    o->_prev->_next = o;
    o->_next->_prev = o;
}

/**
 * insert the element at a specified position .
 *
 * @param	uint_t
 * @param	void *
 */
CEL_API void cel_link_insert( cel_link_t link, uint_t idx, void * value ) 
{
    link_node_t o = get_node( link, idx );
    if ( o != NULL ) {
	insert_before( o, value );
	link->size++;
    }
}

/**
 * get the node at a specified position .
 *
 * @param	link
 * @param	idx
 */
static link_node_t get_node( cel_link_t link, uint_t idx ) 
{
    register uint_t i;
    link_node_t p;

    if ( idx >= link->size ) return NULL;
    //find the node
    if ( idx > link->size / 2 ) {
	p = link->tail; 
	for ( i = link->size; i > idx; i-- )
	    p = p->_prev;
    } else {
	p = link->head;
	for ( i = 0; i <= idx; i++ )
	    p = p->_next; 
    }

    return p;
}

/**
 * remove the element at a specified position . 
 *
 * @param	uint_t
 * @return	void *
 */
CEL_API void * cel_link_remove( cel_link_t link, uint_t idx ) 
{
    void * v;
    link_node_t o = get_node( link, idx );
    if ( o == NULL ) return NULL;
    //remove the node
    v = o->value;
    cel_remove_node(o); 
    cel_free(o);

    link->size--;
    return v;
}

//static void cel_remove_node( link_node_t node ) 
//{
//    node->_prev->_next = node->_next;
//    node->_next->_prev = node->_prev;
//}

//remove the element from the head
CEL_API void * cel_link_remove_first( cel_link_t link ) 
{
    void * v;
	link_node_t o;

    if ( link->size == 0 ) return NULL;
    o = link->head->_next;
    v = o->value;
    cel_remove_node(o);
    cel_free(o);	//free the link node

    link->size--;
    return v;
}

//remove the element from the tail
CEL_API void * cel_link_remove_last( cel_link_t link ) 
{
    void * v;
	link_node_t o;
    if ( link->size == 0 ) return NULL;
    o = link->tail->_prev;
    v = o->value;
    cel_remove_node(o);
    cel_free(o);	//free the link node

    link->size--;
    return v;
}

/**
 * remove the element equals the given one .
 *
 * @param	void *
 * @param	cel_compare_fn_t
 * @return	void *
 */
CEL_API void * cel_link_remove_obj( 
	cel_link_t link, 
	void * value, 
	cel_compare_fn_t comp ) 
{
    void * v;
    link_node_t o;
    for ( o = link->head->_next;
	    o != link->tail;
	    o = o->_next ) {
	if ( value == o->value 
		|| comp( value, o->value ) == 0 ) {
	    v = o->value;
	    cel_remove_node(o);
	    cel_free(o);
	    link->size--;
	    return v;
	}
    }
    return NULL;
}

//get the element at the specified postion.
CEL_API void * cel_link_get( cel_link_t link, uint_t idx ) 
{
    link_node_t o = get_node( link, idx  );
    if ( o == NULL ) return NULL;
    return o->value;
}

/**
 * set the element at the specified position with
 * 	the specified value .
 *
 * @param	uint_t
 * @param	void *
 * @return	void *
 */
CEL_API void * cel_link_set( cel_link_t link, uint_t idx, void * value ) 
{
    void * v;
    link_node_t old = get_node( link, idx );
    if ( old == NULL ) return NULL;
    //bak the old value
    v = old->value;
    old->value = value;
    return v;
}

//search the element and replace it with a new one.
CEL_API void * cel_link_set_obj( 
	cel_link_t link,
	void * search,
	void * value,
	cel_compare_fn_t comp )
{
    void * v;
    link_node_t o;
    for ( o = link->head->_next;
	    o != link->tail;
	    o = o->_next ) {
	if ( search == o->value 
		|| comp( search, o->value ) == 0 ) {
	    v = o->value;
	    o->value = value;
	    return v;
	}
    }
    return NULL;
}

