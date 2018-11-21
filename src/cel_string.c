/**
 * cel string module interface implemente functions.
 *     @package src/cel_string.c . <br />
 *
 * @author chenxin <chenxin619315@gmail.com>
 * @date 2013-09-16
 */

#include "cel_string.h"
#include <string.h>

/* {{{ cel string block*/
CEL_API cel_string_t *new_cel_string( cstring str )
{
    uint_t len;
    cel_string_t *ptr = ( cel_string_t * ) 
    cel_malloc( sizeof( cel_string_t ) );
    if ( str == NULL ) return NULL;

    //Count the length of the string.
    len = (uint_t) strlen( str );
    ptr->str = ( cstring ) cel_malloc( len );
    if ( ptr->str == NULL ) {
        cel_free( ptr );
        return NULL;
    }

    //Copy the string to the heap.
    memcpy( ptr->str, str, len );
    ptr->len = len;

    return ptr;
}

CEL_API cel_string_t *cel_string_long( long numeric )
{
    return NULL;
}

CEL_API cel_string_t *cel_string_double( double decimal )
{
    return NULL;
}

CEL_API void free_cel_string( cel_string_t **cstr )
{
    if ( cstr == NULL ) return;
    if ( *cstr != NULL ) {
        cel_free( (*cstr)->str );
        (*cstr)->str = NULL;

        cel_free( *cstr );
        *cstr = NULL;
    }
}
/* }}}*/




/* {{{ cel string buffer area.
 * */
static cstring create_buffer( int opacity )
{
    cstring str = ( cstring ) cel_malloc( opacity );
    if ( str == NULL ) return NULL;
    memset( str, 0x00, opacity );
    return str;
}

/* resize the string buffer to a specified opacity
 *
 * @param    opacity - the new opacity
 * @return    int 1 for success and 0 for failed
 * */
static int resize_buffer( 
    cel_strbuff_t *sb, int opacity )
{
    uint_t length;
    cstring str = create_buffer( opacity );
    if ( str == NULL ) {
        //free_cel_strbuff( &sb );
        return 0;
    }

    length = (opacity > sb->size) ? sb->size : opacity;
    memcpy( str, sb->buffer, length );

    //free the old buffer.
    cel_free( sb->buffer );
    sb->opacity = opacity;
    sb->buffer = str;

    return 1;
}

/* interface to create new cel_strbuff_entry
 *  or to initialize a specified cel_strbuff_entry.
 * */
CEL_API cel_strbuff_t *new_cel_strbuff_opacity( int opacity )
{
    cel_strbuff_t *sb = ( cel_strbuff_t * ) 
    cel_malloc( sizeof( cel_strbuff_t ) );
    if ( sb == NULL ) return NULL;

    sb->buffer = create_buffer( opacity );
    if ( sb->buffer == NULL ) {
        cel_free( sb );
        CEL_ALLOCATE_ERROR("new_cel_strbuff_opacity", opacity);
    }

    //initialize.
    sb->opacity = opacity;
    sb->size = 0;

    return sb;
}

CEL_API cel_strbuff_t *new_cel_strbuff_string( cstring str )
{
    uint_t len, bytes;
    cel_strbuff_t *sb = (cel_strbuff_t *) cel_malloc(sizeof( cel_strbuff_t ));
    if ( sb == NULL ) return NULL;

    //Count the length.
    len = (uint_t) strlen( str );
    bytes = len + _CEL_DEFAULT_STRBUFF_OPACITY_;

    sb->buffer = create_buffer( bytes );
    if ( sb->buffer == NULL ) {
        cel_free( sb );
        CEL_ALLOCATE_ERROR("new_cel_strbuff_string", bytes);
    }

    //initialize.
    sb->opacity = bytes;
    sb->size = len;
    memcpy( sb->buffer, str, len );

    return sb;
}

//free the specified string buffer.
CEL_API void free_cel_strbuff( cel_strbuff_t **sb )
{
    if ( sb == NULL ) return;
    if ( *sb != NULL ) {
        cel_strbuff_destroy(*sb);
        cel_free( *sb );
        *sb = NULL;
    }
}

/*
 * create the specified string buffer
 *
 * @param   cel_strbuff_t *
 * @param   uint_t default opacity
 * @param   cstring default string -  could be NULL
 * @return  int 1 for success and 0 for failed
 */
CEL_API int cel_strbuff_init(cel_strbuff_t *sb, int opacity, cstring str)
{
    uint_t len = 0;
    uint_t bytes = opacity;

    //check the initialize string.
    if ( str != NULL ) {
        len = strlen( str );
        bytes += len;
    }

    sb->buffer = create_buffer( bytes );
    if ( sb->buffer == NULL ) {
        return 0;
    }

    //initialize
    sb->opacity = bytes;
    sb->size = len;
    if ( len > 0 ) memcpy( sb->buffer, str, len );

    return 1;
}

/*
 * destroy the specified cel_strbuff_t
 *
 * @param   cel_strbuff_t *
 * @return  int 1 for success and 0 for failed
 */
CEL_API int cel_strbuff_destroy( cel_strbuff_t *sb )
{
    if ( sb != NULL ) {
        cel_free( sb->buffer );
        sb->buffer = NULL;
    }

    return 1;
}

/*
 * append a new string to the buffer.
 *
 * @param cstring - the string to append to.
 * a macro define has replace this.
 * */
//CEL_API void cel_strbuff_append( cel_strbuff_t *sb, cstring str )
//{
//    uint_t len = strlen( str );
//    if ( sb->size + len > sb->opacity )
//    resize_buffer( sb, (sb->size + len) * 2 + 1 );
//
//    //Copy the string insite.
//    memcpy( sb->buffer + sb->size, str, len );
//    sb->size += len;
//}

/* append some bytes from a specified buffer
 *     from a specified index
 *
 * @param   cstring - the buffer to copy byte from
 * @param   len - the number of bytes to copy
 * @param   count - repeat times
 * @return  1 for success and 0 for failed
 * */
CEL_API int cel_strbuff_append_from(
    cel_strbuff_t *sb, cstring src, uint_t len, int count)
{
    while ( count-- > 0 ) {
        //leave a space for the end of the buffer.
        //change '>' '>=' at 2013-10-12
        if ( sb->size + len >= sb->opacity ) {
            if ( resize_buffer( sb, (sb->size + len) * 2 + 1 ) == 0 ) {
                return 0;
            }
        }

        //Copy the string insite.
        if ( len == 1 ) {
            sb->buffer[sb->size++] = src[0];
        } else { 
            memcpy( sb->buffer + sb->size, src, len );
            sb->size += len;
        }
    }

    return 1;
}

/* append a char to the buffer.
 *
 * @param   char
 * @return  1 for success and 0 for failed
 * */
CEL_API int cel_strbuff_append_char(cel_strbuff_t *sb, char ch, int count)
{
    while ( count-- > 0 ) {
        //leave a space for the end of the buffer.
        //change '>' '>=' at 2013-10-12
        if ( sb->size + 1 >= sb->opacity ) {
            if ( resize_buffer( sb, sb->opacity * 2 + 1 ) == 0 ) {
                return 0;
            }
        }

        //append the char
        sb->buffer[sb->size++] = ch;
    }

    return 1;
}

/* insert a new string to the buffer from the specified position.
 *
 * @param uint_t - the start position.
 * @param cstring - the string to insert in.
 *
 * a macro define has replace this.
 * */
//CEL_API void cel_strbuff_insert( 
//    cel_strbuff_t *sb, uint_t idx, cstring str )
//{
//    if ( idx > sb->size  ) return;
//
//    uint_t len = strlen( str );
//    if ( sb->size + len > sb->opacity )
//    resize_buffer( sb, ( sb->size + len ) * 2 + 1);
//
//    //Copy the old mem back len step.
//    //do not use the memcpy here.
//    int i, l = (int) idx;
//    for ( i = sb->size - 1; i >= l; i-- )
//    sb->buffer[i+len] = sb->buffer[i];
//
//    //Copy the new string start from idx.
//    memcpy( sb->buffer + idx, str, len );
//    sb->size += len;
//}

/* insert part of buffer to the buffer
 *
 * @param   uint_t - start position
 * @param   cstring - source buffer
 * @param   uint_t - number of bytes to copy
 * @return  1 for success and 0 for failed
 * */
CEL_API int cel_strbuff_insert_from(
    cel_strbuff_t *sb, uint_t idx, cstring src, uint_t len, int count)
{
    while ( count-- > 0 ) {
        int i, l = (int) idx;    
        if ( idx > sb->size  ) break;

        //leave a space for the end of the buffer.
        //change '>' '>=' at 2013-10-12
        if ( sb->size + len >= sb->opacity ) {
            if ( resize_buffer( sb, ( sb->size + len ) * 2 + 1) == 0 ) {
                return 0;
            }
        }

        //Copy the old mem back len step.
        //do not use the memcpy here.
        for ( i = sb->size - 1; i >= l; i-- ) {
            sb->buffer[i+len] = sb->buffer[i];
        }

        //Copy the new string start from idx.
        memcpy( sb->buffer + idx, src, len );
        sb->size += len;
        idx += len;
    }

    return 1;
}

/* insert a char to the buffer from a specified index position.
 *
 * @param   char
 * @return  1 for success and 0 for failed
 * */
CEL_API int cel_strbuff_insert_char(
    cel_strbuff_t *sb, uint_t idx, char ch, int count)
{
    while ( count-- > 0 ) {
        if ( idx > sb->size ) return 0;

        //leave a space for the end of the buffer.
        //change '>' '>=' at 2013-10-12
        if ( sb->size + 1 >= sb->opacity ) {
            if ( resize_buffer( sb, sb->size * 2 + 1 ) == 0 ) {
                return 0;
            }
        }

        sb->buffer[sb->size++] = ch;
        idx++;
    }

    return 1;
}

/* remove specified char from a specified position from the buffer
 *
 * @param   uint_t - the start index
 * @param   uint_t - number of chars to remove
 * @return  1 for success and 0 for failed
 * */
CEL_API int cel_strbuff_remove(cel_strbuff_t *sb, uint_t idx, uint_t len)
{
    uint_t i;
    if ( idx >= sb->size  ) return 0;

    //Recount the length.
    if ( len > sb->size - idx ) {
        len = sb->size - idx;
    }

    //Copy the buffer.
    for ( i = idx + len; i < sb->size; i++ ) {
        sb->buffer[i-len] = sb->buffer[i];
    }

    //clear the old buffer part.
    memset( sb->buffer + sb->size - len , 0x00, len );
    sb->size -= len;

    return 1;
}

//clear the specified string buffer.
CEL_API int cel_strbuff_clear( cel_strbuff_t *sb )
{
    memset( sb->buffer, 0x00, sb->size );
    sb->size = 0;

    return 1;
}
/* end of string buffer.
 * }}}
 * */




/* {{{ utf-8 unicode convert.
 * */
/* check the specified word token to store encode in utf-8.
 *
 * @param ch
 * */
CEL_API uint_t cel_utf8_charbytes( char ch )
{
    register uint_t t = 0;

    //if ( ( ch & 0x80 ) == 0 ) return 1;
    if ( ch >= 0 ) return 1;
    
    for ( ; ( ch & 0x80 ) != 0; ch <<= 1 ) {
        t++;
    }

    return t;
}

/* convert the specified utf8 word to unicode.
 *
 * @param word
 * */
CEL_API uint_t cel_utf8_to_ucode( cstring word )
{
    //Count the bytes the word take.
    uint_t bytes = cel_utf8_charbytes( *word );

    //unicode serial.
    uint_t code = 0;

    register uchar_t *bit = ( uchar_t * ) &code;
    register char b1, b2, b3/*, b4, b5, b6*/;

    switch ( bytes ) 
    {
    case 1:
        *bit = *word;
        break;
    case 2:
        b1 = *(word);
        b2 = *(word + 1);

        *bit     = (b1 << 6) + (b2 & 0x3F);
        *(bit+1) = (b1 >> 2) & 0x07;
        break;
    case 3:
        b1 = *(word);
        b2 = *(word + 1);
        b3 = *(word + 2);

        *bit     = (b2 << 6) + (b3 & 0x3F);
        *(bit+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
        break;
    //ignore the ones that are larger than 3 bytes;
    }

    return code;
}

/* convert the specified unicode to utf-8 word and store them in the buffer.
 *
 * @param   ucode
 * @param   buffer
 * @return  int - the bytes it take.
 * */
CEL_API int cel_ucode_to_utf8(uint_t u, cstring __word)
{
    if ( u <= 0x0000007F ) {
        //U-00000000 - U-0000007F
        //0xxxxxxx
        *__word     = ( u & 0x7F );
        return 1;
    } else if ( u >= 0x00000080 && u <= 0x000007FF ) {
        //U-00000080 - U-000007FF
        //110xxxxx 10xxxxxx
        *( __word + 1 ) = ( u & 0x3F) | 0x80;
        *__word         = ((u >> 6) & 0x1F) | 0xC0;
        return 2;
    } else if ( u >= 0x00000800 && u <= 0x0000FFFF ) {
        //U-00000800 - U-0000FFFF
        //1110xxxx 10xxxxxx 10xxxxxx
        *( __word + 2 ) = ( u & 0x3F) | 0x80;
        *( __word + 1 ) = ((u >>  6) & 0x3F) | 0x80;
        *__word         = ((u >> 12) & 0x0F) | 0xE0;
        return 3;
    } else if ( u >= 0x00010000 && u <= 0x001FFFFF ) {
        //U-00010000 - U-001FFFFF
        //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *( __word + 3 ) = ( u & 0x3F) | 0x80;
        *( __word + 2 ) = ((u >>  6) & 0x3F) | 0x80;
        *( __word + 1 ) = ((u >> 12) & 0x3F) | 0x80;
        *__word         = ((u >> 18) & 0x07) | 0xF0;
        return 4;
    } else if ( u >= 0x00200000 && u <= 0x03FFFFFF ) {
        //U-00200000 - U-03FFFFFF
        //111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *( __word + 4 ) = ( u & 0x3F) | 0x80;
        *( __word + 3 ) = ((u >>  6) & 0x3F) | 0x80;
        *( __word + 2 ) = ((u >> 12) & 0x3F) | 0x80;
        *( __word + 1 ) = ((u >> 18) & 0x3F) | 0x80;
        *__word         = ((u >> 24) & 0x03) | 0xF8;
        return 5;
    } else if ( u >= 0x04000000 && u <= 0x7FFFFFFF ) {
        //U-04000000 - U-7FFFFFFF
        //1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *( __word + 5 ) = ( u & 0x3F) | 0x80;
        *( __word + 4 ) = ((u >>  6) & 0x3F) | 0x80;
        *( __word + 3 ) = ((u >> 12) & 0x3F) | 0x80;
        *( __word + 2 ) = ((u >> 18) & 0x3F) | 0x80;
        *( __word + 1 ) = ((u >> 24) & 0x3F) | 0x80;
        *__word         = ((u >> 30) & 0x01) | 0xFC;
        return 6;
    }

    return 0;
}
/* end of utf-8 unicoe convert.
 * }}}
 * */
