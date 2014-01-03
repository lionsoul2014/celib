/**
 * cel string module test program. <br />
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_string.h"

int main( int argc, char * args[] )
{
    uint_t i;
    cstring str[] = {"a", "bc", "defg", "hijklmn", "opqrstyvwxyz"};
    uint_t length = 5;
    cel_strbuff_t sb;

    //sb = new_cel_strbuff_opacity(10);
    //sb = new_cel_strbuff();
    sb = new_cel_strbuff_string("{");
    
    //cel_strbuff_entry strbuff;
    //sb = &strbuff;
    //cel_strbuff_init( sb, 10, "{" );

    printf("sb->opacity=%d, sb->length=%d\n", sb->opacity, sb->length);

    printf("+---test append: \n");
    for ( i = 0; i < length; i++ ) 
    {
	cel_strbuff_append(sb, str[i], 1);
	printf("append(\"%-15s\")=%-15s\n", str[i], sb->buffer);
    }
    cel_strbuff_append_char(sb, '1', 2);
    printf("append('1')=%s\n", sb->buffer);
    
    cel_strbuff_append_char( sb, '2', 1 );
    printf("append('2')=%s\n", sb->buffer);

    cel_strbuff_append_char( sb, '}', 1 );
    printf("append('}')=%s\n", sb->buffer);

    printf("sb->opacity=%d, sb->length=%d\n", sb->opacity, sb->length);
    putchar('\n');

    printf("+---test insert: \n");
    cel_strbuff_insert( sb, 0, "#!", 1 );
    printf("insert(0, \"#1\")=%s\n", sb->buffer);

    i = sb->length / 2;
    cel_strbuff_insert( sb, i, "(@)", 1 );
    printf("insert(%d, \"(@)\")=%s\n", i, sb->buffer);

    i = sb->length;
    cel_strbuff_insert( sb, i, "!#", 1 );
    printf("insert(%d, \"!#\")=%s\n", i, sb->buffer);

    i = sb->length;
    cel_strbuff_insert_char( sb, sb->length, '+', 2 );
    printf("insert(%d, '+')=%s\n", i, sb->buffer );
    printf("sb->opacity=%d, sb->length=%d\n", sb->opacity, sb->length);
    putchar('\n');

    printf("+---test remove: \n");
    cel_strbuff_remove( sb, 0, 2 );
    printf("remove(0, 2)=%s\n", sb->buffer);

    i = sb->length / 2;
    cel_strbuff_remove( sb, i, 3 );
    printf("remove(%d, 3)=%s\n", i, sb->buffer );

    i = sb->length;
    cel_strbuff_remove( sb, i - 4, 4 );
    printf("remove(%d, 2)=%s\n", i, sb->buffer );

    printf("sb->opacity=%d, sb->length=%d\n", sb->opacity, sb->length);
    putchar('\n');

    //Free the string buffer.
    free_cel_strbuff( sb );

    return 0;
}
