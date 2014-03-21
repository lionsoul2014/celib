/**
 * hashmap interface header file src/cel_hashmap.h .
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#ifndef _cel_hashmap_h
#define _cel_hashmap_h

#include "cel_api.h"

/**
 * when you define this marco,
 * means: you want to store the hashcode in the hashmap_node_t for
 * 	your program, of course this will make the hashmap
 * rebuild faster, otherwise will make more allocations.
 *
 * comment it for not store the hashcode.
 */
#define CEL_HASHMAP_STORE_HCODE

//hashmap node struct
struct cel_hashmap_node_struct {
    char *key;
    //value union
    union {
	   void *ptr;
	   int num;
    } value;
    struct cel_hashmap_node_struct *next;
#ifdef CEL_HASHMAP_STORE_HCODE
    uint_t hash;
#endif
};
typedef struct cel_hashmap_node_struct cel_hashmap_node_t;

//cel hashmap entry
typedef struct {
    cel_hashmap_node_t **table;
    uint_t length;
    uint_t size;
    uint_t threshold;
    float factor;
} cel_hashmap_t;

//cel hashmap release callback function pointer
typedef void ( * cel_hashmap_rcb_fn_t ) ( cel_hashmap_node_t * );

//quick lanch macro define
#define cel_hashmap_size( map )         map->size
#define cel_hashmap_length( map )       map->length
#define cel_hashmap_factor( map )       map->factor
#define cel_hashmap_threshold( map )    map->threshold

/*
 * create a default cel hashmap with a default
 * 	length and default factor
 *
 * @return	cel_hashmap_t
 */
//CEL_API cel_hashmap_t new_cel_hashmap();
#define new_cel_hashmap() new_cel_hashmap_opacity( 16, 0.75 )

/*
 * create a cel hashmap with a specified length and
 * 	factory .
 *
 * @param	uint_t
 * @param	float
 * @return	cel_hashmap_t
 */
CEL_API cel_hashmap_t *new_cel_hashmap_opacity( int, float );

/*
 * free the specified cel hashmap.
 * callback function will be invoked for each node,
 *	if it is not null.
 */
CEL_API void free_cel_hashmap( 
	cel_hashmap_t **, cel_hashmap_rcb_fn_t );

/*
 * initialize the specified hashmap
 *
 * @param   cel_hashmap_t *
 * @param   uint_t  the default length of the blocks
 * @param   float   the threshold of the hash map
 * @return  int 1 for success and 0 for failed
 */
CEL_API int cel_hashmap_init( cel_hashmap_t *, int, float );

/*
 * destroy the specified hashmap
 *
 * @param   cel_hashmap_t *
 * @return  1 for success and 0 for fialed
*/
CEL_API int cel_hashmap_destroy( cel_hashmap_t *, cel_hashmap_rcb_fn_t );

/*
 * associated the key with the specified value .
 *
 * @param	char *
 * @param	void *
 * @return	void * the old the value or NULL
 */
CEL_API void *cel_hashmap_put( cel_hashmap_t *, char *, void * );

//remove the mapping associated with the specified key
CEL_API void *cel_hashmap_remove( cel_hashmap_t *, char *, cel_hashmap_rcb_fn_t );

//get the value associated with the specified key.
CEL_API void *cel_hashmap_get( cel_hashmap_t *, char * );

//check the existence of the mapping associated with the specified key.
CEL_API int cel_hashmap_exists( cel_hashmap_t *, char * );

//replace the value associated with the specified key.
CEL_API void *cel_hashmap_set( cel_hashmap_t *, char *, void * );




/* {{{ char int hash block::start.*/
typedef cel_hashmap_t cel_ihashmap_t;

//quick lanch macro define
#define cel_ihashmap_size( map )        map->size
#define cel_ihashmap_length( map )      map->length
#define cel_ihashmap_factor( map )      map->factor
#define cel_ihashmap_threshold( map )   map->threshold

//quick interface to create new ihashmap.
#define new_cel_ihashmap() new_cel_hashmap()
#define new_cel_ihashmap_opacity( opacity, factor )\
    new_cel_hashmap_opacity( opacity, factor )

/*
 * free the specified cel ihashmap.
 */
//CEL_API void free_cel_ihashmap( cel_hashmap_t );
#define free_cel_ihashmap( map, rfunc ) free_cel_hashmap( map, rfunc )

#define cel_ihashmap_init( map, opacity, factor )\
    cel_hashmap_init(map, opacity, factor)
#define cel_ihashmap_destroy( map )\
    cel_hashmap_destroy(map, NULL)

/*
 * associated the key with the specified value .
 *
 * @param	char *
 * @param	int
 * @return	int (0 for fail and 1 for true)
 */
CEL_API int cel_ihashmap_put( cel_ihashmap_t *, char *, int );

//remove the mapping associated with the specified key
CEL_API int cel_ihashmap_remove( cel_ihashmap_t *, char *, cel_hashmap_rcb_fn_t );

//get the value associated with the specified key.
CEL_API int cel_ihashmap_get( cel_ihashmap_t *, char * );

//check the existence of the mapping associated with the specified key.
CEL_API int cel_ihashmap_exists( cel_ihashmap_t *, char * );

//replace the value associated with the specified key.
CEL_API int cel_ihashmap_set( cel_ihashmap_t *, char *, int );
/*}}}*/
#endif	/*end ifndef*/
