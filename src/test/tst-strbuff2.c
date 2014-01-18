/**
 * cel string buffer test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_string.h"
#include <stdio.h>

int main( int argc, char *args[] )
{
    cel_strbuff_t *sb = new_cel_strbuff();
    char * str = "ababcdefefgggghihijklmnmnopqrsoptuvwxyzxyzgg";

    //apend the string.
    int i;
    for ( i = 0; i < 44; i++ )
	   cel_strbuff_append_char(sb, str[i], 1);
    
    cel_strbuff_append(sb, ".", 1);
    printf("length=%d, size=%d\n", sb->opacity, sb->size);
    printf("buffer=%s\n", sb->buffer);

    free_cel_strbuff(&sb);

    return 0;
}
