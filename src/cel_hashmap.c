/**
 * hashmap implemented functions . 
 *  @package src/cel_hashmap.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "cel_hashmap.h"
#include "cel_math.h"
#include "cel_hash.h"
#include <string.h>

static hashmap_node_t **create_node_blocks( uint_t );
static hashmap_node_t *create_hashmap_node( char *, void *, hashmap_node_t * );
static void rebuild_hash( cel_hashmap_t * );

//hash function marco define
#define cel_hash( str ) cel_bkdr_hash( str )

/*
 * create a default cel hashmap with a default]
 * 	length and default factor
 *
 * @return	cel_hashmap_t
 * A macro define has replace this
 */
//CEL_API cel_hashmap_t *new_cel_hashmap() 
//{
//    return new_cel_hashmap_opacity( 16, 0.75 );
//}

/*
 * create a cel hashmap with a specified length and
 * 	factory .
 *
 * @param	opacity
 * @param	factor
 * @return	cel_hashmap_t
 */
CEL_API cel_hashmap_t *new_cel_hashmap_opacity( 
		int opacity, float factor )
{
	cel_hashmap_t *map = ( cel_hashmap_t * ) 
		cel_malloc( sizeof( cel_hashmap_t ) );
	if ( map == NULL ) 
	{
		CEL_ALLOCATE_ERROR("new_cel_hashmap_opacity", sizeof(cel_hashmap_t));
	}

	if ( cel_hashmap_create(map, opacity, factor) == 0 )
	{
		cel_free(map);
		CEL_ALLOCATE_ERROR("cel_hashmap_create", opacity);
	}

	return map;
}

/*
 * free the specified cel hashmap.
 * callback function will be invoked for each node,
 *	if it is not null.
 */
CEL_API void free_cel_hashmap( 
		cel_hashmap_t **hash, 
		cel_hashmap_rcb_fn_t relfunc )
{
	if ( hash == NULL ) return;
	if ( *hash != NULL )
	{
		cel_hashmap_destroy(*hash, relfunc);
		cel_free( *hash );
		*hash = NULL;
	}
}

/*
 * initialize the specified hashmap
 *
 * @param   cel_hashmap_t *
 * @param   uint_t  the default length of the blocks
 * @param   float   the threshold of the hash map
 * @return  int 1 for success and 0 for failed
 */
CEL_API int cel_hashmap_create( 
		cel_hashmap_t *map, int opacity, float factor )
{
	map->table = create_node_blocks( opacity );

	if ( map->table == NULL ) return 0;

	map->length = opacity;
	map->size = 0;
	map->factor = factor;
	map->threshold = ( uint_t ) ( factor * opacity );

	return 1;
}

/*
 * destroy the specified hashmap
 *
 * @param   cel_hashmap_t *
 * @return  1 for success and 0 for fialed
*/
CEL_API int cel_hashmap_destroy( 
		cel_hashmap_t *hash, 
		cel_hashmap_rcb_fn_t relfunc )
{
	register uint_t idx;
	hashmap_node_t *e, *next;

	if ( hash != NULL )
	{
		for ( idx = 0; 
			idx < hash->length; idx++ ) 
		{
			for ( e = hash->table[idx];
					e != NULL; ) 
			{
				next = e->next;
				if ( relfunc != NULL ) relfunc(e);
				cel_free( e );
				e = next;
			}
		}

		//free the table bucket
		cel_free( hash->table );
		hash->table = NULL;
	}

	return 1;
}


//create a new hashmap node.
static hashmap_node_t **create_node_blocks( uint_t __blocks )
{
	register uint_t i;

	hashmap_node_t **table = ( hashmap_node_t ** ) 
		cel_calloc( sizeof( hashmap_node_t * ), __blocks );
	if ( table == NULL ) return NULL;

	//initialize the bucket
	for ( i = 0; i < __blocks; i++ )
		table[i] = NULL;

	return table;
}

/*
 * hash rebuild mapping .
 * 
 * @param	hash
 */
static void rebuild_hash( cel_hashmap_t *hash )
{
	register uint_t i;
	uint_t idx;

	uint_t length = cel_next_prime(hash->length * 2 + 1);
	hashmap_node_t **_table = create_node_blocks( length );
	hashmap_node_t *e, *next;

	if ( _table == NULL )
	{
		return;
	}
	
	//printf("rebuild-hash: %d\n", length);

	for ( i = 0; i < hash->length; i++ ) 
	{
		//for ( e = hash->table[i];
		//   	e != NULL; ) {
		//    //recount the hash
		//    idx = cel_hash(e->key) % length;
		//    tmp = e->next;
		//    e->next = _table[idx];
		//    _table[idx] = e;
		//    e = tmp;
		//}
		e = *(hash->table + i);
		if ( e != NULL ) 
		{
			do 
			{
				next = e->next;
#ifdef  CEL_HASHMAP_STORE_HCODE
				idx = e->hash % length;
#else
				//recount the hash code 
				idx = cel_hash(e->key) % length;
#endif
				e->next = _table[idx];
				_table[idx] = e;
				e = next;
			} while ( e != NULL );
		}
	}

	//free the old bucket
	cel_free( hash->table );
	hash->table = _table;
	hash->length = length;
	hash->threshold = (uint_t) (hash->factor * length);
}

/*
 * associated the key with the specified value .
 *
 * @param	key	
 * @param	value	
 * @return	void *
 */
CEL_API void *cel_hashmap_put( 
		cel_hashmap_t *hash,
		char *key, void *value )
{
	void * v;
	hashmap_node_t *e;
	uint_t hcode = cel_hash(key);
	uint_t idx = hcode % hash->length;

	for ( e = *(hash->table + idx);
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key 
				|| strcmp( key, e->key ) == 0 ) 
		{
			v = e->value.ptr;
			e->value.ptr = value;
			return v;
		}
	}

	hash->table[idx] = 
		create_hashmap_node( key, value, hash->table[idx] );

#ifdef CEL_HASHMAP_STORE_HCODE 
	e = hash->table[idx];
	e->hash = hcode;
	//printf("hash: %u\n", hcode);
#endif
	hash->size++;
	if ( hash->size > hash->threshold ) 
	{
		//rebuild the hashmap
		rebuild_hash(hash);
	}

	return NULL;
}

/*
 * create new hashmap node.
 *
 * @param	key
 * @param	value
 * @param	next
 */
static hashmap_node_t *create_hashmap_node( 
		char *key, void *value,
		hashmap_node_t *next )
{
	hashmap_node_t *node = ( hashmap_node_t * ) 
		cel_malloc( sizeof( hashmap_node_t ) );
	if ( node == NULL ) 
	{
		CEL_ALLOCATE_ERROR("create_hashmap_node", sizeof(hashmap_node_t));
	}

	node->key = key;
	node->value.ptr = value;
	node->next = next;

	return node;
}

//remove the mapping associated with the specified key
CEL_API void *cel_hashmap_remove( 
		cel_hashmap_t *hash, char *key, 
		cel_hashmap_rcb_fn_t rfunc )
{
	void * v;
	hashmap_node_t *e, *prev = NULL;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx]; 
			e != NULL; 
			prev = e, e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			if ( prev == NULL ) 
				hash->table[idx] = e->next;
			else {
				prev->next = e->next;
			}
			v = e->value.ptr;

			//invoke the callback function if it is not NULL.
			if ( rfunc != NULL ) rfunc( e );
			//free the removed node
			cel_free( e );
			hash->size--;
			return v;
		}
	}

	return NULL;
}

//get the value associated with the specified key.
CEL_API void * cel_hashmap_get( 
		cel_hashmap_t *hash, char *key )
{
	hashmap_node_t *e;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx];
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			return e->value.ptr;
		}
	}

	return NULL;
}

//check the existence of the mapping associated with the specified key.
CEL_API int cel_hashmap_exists( 
		cel_hashmap_t *hash, char *key )
{
	hashmap_node_t *e;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx];
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			return 1;
		}
	}

	return 0;
}

//replace the value associated with the specified key.
CEL_API void *cel_hashmap_set( 
		cel_hashmap_t *hash,
		char *key, void *value )
{
	void * v;
	hashmap_node_t *e;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx];
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			v = e->value.ptr;
			e->value.ptr = value;
			return v;
		}
	}

	return NULL;
}




/* {{{ char int hashmap block::start*/

/*
 * create new hashmap node.
 *
 * @param	key
 * @param	value
 * @param	next
 */
static hashmap_node_t *create_ihashmap_node( 
		char *key,
		int value,
		hashmap_node_t *next )
{
	hashmap_node_t *node = ( hashmap_node_t * ) \
			cel_malloc( sizeof( hashmap_node_t ) );
	if ( node == NULL ) 
	{
		CEL_ALLOCATE_ERROR("create_ihashmap_node", sizeof(hashmap_node_t));	
	}

	node->key = key;
	node->value.num = value;
	node->next = next;

	return node;
}

/*
 * associated the key with the specified value .
 *
 * @param	char *
 * @param	int
 * @return	int (0 for fail and 1 for true)
 */
CEL_API int cel_ihashmap_put( 
		cel_ihashmap_t *hash, char *key, int value )
{
	hashmap_node_t *e;
	uint_t hcode = cel_hash(key);
	uint_t idx = hcode % hash->length;

	for ( e = *(hash->table + idx);
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key 
				|| strcmp( key, e->key ) == 0 ) 
		{
			e->value.num = value;
			return 1;
		}
	}

	hash->table[idx] = create_ihashmap_node( 
			key, value, hash->table[idx] );
#ifdef CEL_HASHMAP_STORE_HCODE 
	e = hash->table[idx];
	e->hash = hcode;
#endif
	hash->size++;
	if ( hash->size > hash->threshold ) {
		//rebuild the hashmap
		rebuild_hash(hash);
	}

	return 1;
}

//remove the mapping associated with the specified key
CEL_API int cel_ihashmap_remove( 
		cel_ihashmap_t *hash, char *key, 
		cel_hashmap_rcb_fn_t rfunc )
{
	hashmap_node_t *e, *prev = NULL;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx]; 
			e != NULL; 
			prev = e, e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			if ( prev == NULL ) 
				hash->table[idx] = e->next;
			else 
			{
				prev->next = e->next;
			}
			//invoke the release callback function.
			if ( rfunc != NULL ) rfunc( e );
			//free the removed node
			cel_free( e );
			hash->size--;
			return 1;
		}
	}

	return 0;
}

//get the value associated with the specified key.
CEL_API int cel_ihashmap_get( 
		cel_ihashmap_t *hash, char *key )
{
	hashmap_node_t *e;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx];
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			return e->value.num;
		}
	}

	return 0;
}

//check the existence of the mapping associated with the specified key.
CEL_API int cel_ihashmap_exists( 
		cel_ihashmap_t *hash, char *key )
{
	hashmap_node_t *e;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx];
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			return 1;
		}
	}

	return 0;
}

//replace the value associated with the specified key.
CEL_API int cel_ihashmap_set( 
		cel_ihashmap_t *hash, char *key, int value )
{
	hashmap_node_t *e;
	uint_t idx = cel_hash( key ) % hash->length;

	for ( e = hash->table[idx];
			e != NULL;
			e = e->next ) 
	{
		if ( key == e->key
				|| strcmp( key, e->key ) == 0 ) 
		{
			e->value.num = value;
			return 1;
		}
	}

	return 0;
}
/* }}}*/
