/**
 * cel data comnpress/decompress module 
 *  interface implemented source file. 
 *  @package src/cel_compress.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 * @date 2013-09-20
 */
#include "cel_compress.h"
#include "cel_hashmap.h"
#include "cel_string.h"
#include "cel_array.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>


/* {{{ rle - run length encoding.
 * */
#define RLE_BUCKET_MAX_LENGTH 127

/* find a char repeat for more than 2 times
 * 	from a specifie string buffer.
 *
 * @param	inbuff
 * @param	inlen
 * @param	start
 * @param	len
 * @return	idx or -1 (when match none)
 * */
static int next_least3_repeat( 
		char *inbuff, uint_t inlen, 
		uint_t start, uint_t *len )
{
	register uint_t i, j;
	register int p;
	register char val;

	for ( i = start; i < inlen; )
	{
		val = inbuff[i];
		p = 0;
		for ( j = i; j < inlen && p < RLE_BUCKET_MAX_LENGTH
				&& inbuff[j] == val; j++, p++ );

		//find the char repeat for more than 2 times.
		if ( p > 2 ) 
		{
			*len = p;
			return i;
		}

		//plus the offset.
		i += (p > 1) ? p : 1;
	}
	return -1;
}

/* compress the specified string with rle algorithm.
 *
 * @param	inbuff	- the string to encode.
 * @param	inlen	- length of the string.
 * @param	outbuff
 * @param	outlen
 * @return	1 for encode success and 0 for fail.
 * */
CEL_API int cel_rle_encode_string( 
		char *inbuff, uint_t inlen, cel_strbuff_t *sb )
{
	register uint_t i, p, last = 0;
	uint_t len = 0;

	//no-repeat bucket
	register uint_t buckets, t;

	while ( last < inlen && ( p = next_least3_repeat( 
					inbuff, inlen, last, &len ) ) != -1 )
	{
		//Handle the no-repeat chars.
		if ( p > last ) 
		{
			buckets = (p - last) / 127;

			//Handle the buckets.
			t = last;
			for ( i = 0; i < buckets; i++ ) {
				cel_strbuff_append_char(sb, 127, 1);
				cel_strbuff_append_from(sb, &inbuff[t], 127, 1);
				t += 127;
			}

			//Handle the left few chars(less than 127).
			if ( t < p ) {
				cel_strbuff_append_char(sb, ((p-t)&0x7F), 1);
				cel_strbuff_append_from(sb, &inbuff[t], p-t, 1);
			}
		}

		//Handle the repeat chars.
		cel_strbuff_append_char(sb, (len|0x80), 1);
		cel_strbuff_append_char(sb, inbuff[p], 1);

		//change the last offset.
		last = p + len;
	}

	//Handle the last left chars as no-repeat bucket.
	if ( last < inlen ) 
	{
		buckets = (inlen - last) / 127;

		//Handle the buckets.
		t = last;
		for ( i = 0; i < buckets; i++ ) {
			cel_strbuff_append_char(sb, 127, 1);
			cel_strbuff_append_from(sb, &inbuff[t], 127, 1);
			t += 127;
		}

		//Handle the left few chars.
		if ( t < inlen ) {
			cel_strbuff_append_char(sb, ((inlen-t)&0x7F), 1);
			cel_strbuff_append_from(sb, &inbuff[t], inlen-t, 1);
		}
	}

	return 1;
}

/* decompress the specified string
 * 	compress with RLE compress algorithm.
 *
 * @return 1 for decode success or 0.	
 * @see	cel_rle_enckde_string
 * */
CEL_API int cel_rle_decode_string( 
		char *inbuff, uint_t inlen, cel_strbuff_t *sb )
{
	register uint_t i, len;
	register char val;

	for ( i = 0; i < inlen; )
	{
		val = inbuff[i];
		//Count the length
		len = (val & 0x7F);
		if ( len > 127 ) return 0;

		//repeat chars.
		if ( (val & 0x80) != 0 ) {
			cel_strbuff_append_char(sb, inbuff[i+1], len);
			i += 2;
		}
		//no-repeat chars.
		else {
			cel_strbuff_append_from(sb, &inbuff[i+1], len, 1);
			i += (len + 1);
		}
	}

	return 1;
}


/* compress the specified file with RLE compress algorhim.
 *
 * @param	infile	- the file to encode.
 * @param	outfile
 * @return	the length of the encoded file or -1 for failed.
 * */
CEL_API int cel_rle_encode_file( 
		char * infile, char * outfile )
{
	FILE * infp, * outfp;
	uint_t len;
	int length = 0;
	char inbuff[127];
	cel_strbuff_t *sb = new_cel_strbuff();

	//Open the file.
	if ( (infp = fopen( infile, "rb" )) == NULL 
			|| (outfp = fopen(outfile, "wb")) == NULL )
		return -1;
	else {
		while ( (len = fread( inbuff, 1, 127, infp )) > 0 )
		{
			//encode the buffer
			//return the string after encode.
			cel_rle_encode_string( inbuff, len, sb );
			if ( fwrite( sb->buffer, 1, 
						sb->size, outfp ) != sb->size )
				return -1;
			length += sb->size;
			cel_strbuff_clear(sb);
		}

		fclose(infp);
		fclose(outfp);
	}

	//free the string buffer.
	free_cel_strbuff(&sb);

	return length;
}


/* decompress the specified file
 * 	encode with RLE compress algorithm.
 *
 * @return	the length of the decoded file or -1 for failed.
 * @see	cel_rle_encode_file
 * */
CEL_API int cel_rle_decode_file( 
		char * infile, char * outfile )
{
	FILE * infp, * outfp;
	register char val;
	register int length = 0, len;
	char inbuff[127];

	if ( (infp = fopen( infile, "rb" )) == NULL 
			|| (outfp = fopen( outfile, "wb" )) == NULL )
		return -1;
	else 
	{
		while ( (val = getc(infp)) != EOF ) {
			//Get the data length.
			len = (val & 0x7F);
			if ( len > 127 ) return -1;
			length += len;

			//repeat chars.
			if ( (val & 0x80) != 0 ) {
				//check the EOF.
				if ( (val = getc(infp)) == EOF )
					return -1;

				memset( inbuff, val, len );
				if ( fwrite( inbuff, 1, len, outfp ) != len )
					return -1;
			}
			//no-repeat chars.
			else {
				if ( fread( inbuff, 1, len, infp ) != len )
					return -1;
				if ( fwrite( inbuff, 1, len, outfp ) != len )
					return -1;
			}
		}

		fclose( infp );
		fclose( outfp );
	}

	return length;
}
/* end rle
 * }}}
 * */




/*{{{
 * @date 2013-09-30
 * lzw compress module.
 * */

//lzw encode dictionary hash release callback function.
//free all the key.
static void lzw_encode_rcb( hashmap_node_t *e ) 
{
	cel_free( e->key );
}

/* encode the specified string with lzw algorithm.
 *
 * @param source string.
 * @param len - length of the source string.
 * @param sb - encode result string buffer.
 * */
CEL_API int cel_lzw_encode_string( 
		char * inbuff, uint_t len, cel_strbuff_t *sb )
{
	int dsize = 256, i, v = 0, rlen = 0, bits = CHAR_BIT;
	cel_hashmap_t *map	= new_cel_hashmap_opacity(16, (float)0.80);
	cel_strbuff_t *S 	= new_cel_strbuff_opacity(3);
	cel_intArray_t *ret = new_cel_intArray();

	//initialize the dictionary.
	char Z[2] = {0};
	for ( i = 0; i < dsize; i++ ) {
		if ( sprintf( Z, "%c", i ) == -1 ) return 0;
		cel_ihashmap_put(map, strdup(Z), i);
	}

	//encode loop
	for ( i = 0; i < len; i++ ) 
	{
		Z[0] = inbuff[i];
		cel_strbuff_append_char(S, Z[0], 1);

		//check the dictionary.
		if ( ! cel_ihashmap_exists(map, S->buffer) ) 
		{
			//add the S+Z to dictionary.
			cel_ihashmap_put(map, strdup(S->buffer), dsize++);

			//remove the Z from S as the output.
			cel_strbuff_remove(S, S->size - 1, 1);
			cel_intArray_add(ret, 
					cel_ihashmap_get(map, S->buffer));
			//printf("+%d\n", cel_ihashmap_get(map, S->buffer));

			//reset the S to Z.
			cel_strbuff_clear(S);
			cel_strbuff_append_char(S, Z[0], 1);
		}
	}

	//Handle the left buffer.
	//if ( ! cel_strbuff_empty(S) )
	cel_intArray_add(ret, cel_ihashmap_get(map, S->buffer));

	free_cel_hashmap(&map, lzw_encode_rcb);
	free_cel_strbuff(&S);


	//convert the encode codes to binary string.
	dsize = 256;
	for ( i = 0; i < ret->size; i++ ) 
	{
		//printf("v=%d, rlen=%d, bits=%d\n", v, rlen, bits);
		v = ( v << bits ) + ret->items[i];
		rlen += bits;
		dsize++;

		//check the range
		if ( dsize > ( 1 << bits ) ) bits++;

		//handle the bits large than CHAR_BIT
		while ( rlen > 7 ) 
		{
			rlen -= CHAR_BIT;
			cel_strbuff_append_char(sb, (v >> rlen), 1);
			v &= ( 1 << rlen ) - 1;
		}
	}

	//check and handle the left bits.
	if ( rlen > 0 )
		cel_strbuff_append_char(sb, 
				v << (CHAR_BIT - rlen), 1);

	return 1;
}


//lzw decode dictionary hash release callback function.
//free all the key.
static void lzw_decode_rcb( hashmap_node_t *e ) 
{
	cel_free( e->key );
	cel_free( e->value.ptr );
}

/* decode the specified string with lzw algorithm.
 *
 * @param source string.
 * @param len - length of the source string.
 * @param sb - encode result string buffer.
 * */
CEL_API int cel_lzw_decode_string( 
		cstring inbuff, uint_t len, cel_strbuff_t *sb )
{
	int dsize = 256, i;
	int v = 0, rlen = 0;
	uint_t bits = CHAR_BIT;
	char Z[4] = {0}, key[sizeof(int)] = {0};
	char * entry;

	cel_intArray_t *ret = new_cel_intArray_opacity(len);
	cel_hashmap_t *map;
	cel_strbuff_t *S, *T;

	//Convert the binary string to lzw codes.
	for ( i = 0; i < len; i++ ) 
	{
		v = ( v << CHAR_BIT ) + ((uchar_t)inbuff[i]);
		rlen += CHAR_BIT;
		if ( rlen >= bits ) 
		{
			rlen -= bits;
			cel_intArray_add(ret, (v >> rlen));
			v &= (1 << rlen) - 1;
			dsize++;
			if (  dsize > (1 << bits)) bits++;
		}
	}
	

	//decode the lzw codes.
	map = new_cel_hashmap_opacity(16, (float)0.80);
	S   = new_cel_strbuff();
	T   = new_cel_strbuff();
	dsize = 256;

	//initialize the table.
	for ( i = 0; i < dsize; i++ ) 
	{
		if ( sprintf(key, "%d", i) == -1 ) return 0;
		if ( sprintf(Z , "%c", i) == -1 ) return 0;
		cel_hashmap_put(map, strdup(key), strdup(Z));
	}

	/* input the first item from ret, then
	 * continue from the second one.
	 * so, the first dsize++ is useless.
	 * */
	dsize--;

	//decode loop
	for ( i = 0; i < ret->size; i++ ) 
	{
		if ( sprintf(key, "%d", ret->items[i]) == -1 ) 
			return 0;
		//printf("i=%d, key=%s, ", i, key);

		if ( ! cel_hashmap_exists(map, key) ) 
		{
			cel_strbuff_clear(T);
			cel_strbuff_append(T, S->buffer, 1);
			cel_strbuff_append_char(T, S->buffer[0], 1);
			entry = T->buffer; 					//S + S[0]
		}
		else 
			entry = (char *)cel_hashmap_get( map, key );

		//append the entry to the result.
		cel_strbuff_append(sb, entry, 1);
		//printf("entry=%s\n", entry);
		//printf("%s\n\n", sb->buffer);

		//add the entry to the dictionary.
		if ( sprintf(key, "%d", dsize++) == -1 ) return 0;
		cel_strbuff_append_char(S, entry[0], 1);//S + entry[0]
		cel_hashmap_put(map, strdup(key), strdup(S->buffer));

		//set S = entry.
		cel_strbuff_clear(S);
		cel_strbuff_append(S, entry, 1);
	}

	//free the allocation.
	free_cel_strbuff(&S);
	free_cel_strbuff(&T);
	free_cel_hashmap(&map, lzw_decode_rcb);

	return 1;
}

/* encode the specified file with lzw compress algorithm.
 *
 * @param srcfile
 * @param dstfile - encode destination file.
 * */
CEL_API int cel_lzw_encode_file( char * srcfile, char * dstfile )
{
	return 1;
}

/* decode the specified file with lzw compress algorithm.
 *
 * @param srcfile.
 * @param dstfile - decode destination file.
 * */
CEL_API int cel_lzw_decode_file( char * srcfile, char * dstfile )
{
	return 1;
}
/*}}}*/
