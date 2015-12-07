/**
 * cel data compress/decompress module interface header file.
 *     @package src/cel_compress.h.
 *
 * compress algorithm:
 * 1. RLE - Run length encoding.
 * 2. LZW
 * 3. Huffman
 * 4. LZ(LZ77, LZSS)
 *
 * @author chenxin <chenxin619315@gmail.com>
 * @date 2013-09-20
 */
#ifndef cel_compress_h
#define cel_compress_h

#include "cel_api.h"
#include "cel_string.h"
#include "cel_array.h"

/* {{{ rle - run length encoding.
 * */

/* compress the specified string with rle algorithm.
 *
 * @return 0 for false and 1 for encode success.
 * */
//CEL_API uint_t cel_rle_encode_string( char *, uint_t, char *, uint_t );
CEL_API int cel_rle_encode_string( char *, uint_t, cel_strbuff_t * );

/* decompress the specified string
 *     compress with RLE compress algorithm.
 *
 * @return 0 for false and 1 for decode success.    
 * */
CEL_API int cel_rle_decode_string( char *, uint_t, cel_strbuff_t * );

/* compress the specified file with RLE compress algorhim.
 *
 * @return    the length of the encoded file or -1 for failed.
 * */
CEL_API int cel_rle_encode_file( char *, char * );

/* decompress the specified file
 *     encode with RLE compress algorithm.
 *
 * @return    the length of the decoded file or -1 for failed.
 * */
CEL_API int cel_rle_decode_file( char *, char * );
/* end rle
 * }}}
 * */




/*{{{
 * @date 2013-09-30
 * lzw compress module.
 * */

/* encode the specified string with lzw algorithm.
 *
 * @param source string.
 * @param len - length of the source string.
 * @param sb - encode result string buffer.
 * */
CEL_API int cel_lzw_encode_string( char *, uint_t, cel_strbuff_t * );

/* decode the specified string with lzw algorithm.
 *
 * @param source string.
 * @param len - length of the source string.
 * @param sb - encode result string buffer.
 * */
CEL_API int cel_lzw_decode_string( cstring, uint_t, cel_strbuff_t * );

/* encode the specified file with lzw compress algorithm.
 *
 * @param srcfile
 * @param dstfile - encode destination file.
 * */
CEL_API int cel_lzw_encode_file( char *, char * );

/* decode the specified file with lzw compress algorithm.
 *
 * @param srcfile.
 * @param dstfile - decode destination file.
 * */
CEL_API int cel_lzw_decode_file( char *, char * );
/*}}}*/

#endif    /*end ifndef*/
