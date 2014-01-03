/**
 * format change test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include <stdio.h>

int main ( int argc, char *args[] )
{
    char buff[64];
    if ( sprintf(buff, "%d%c", 32, 'A') != -1 )
	printf("%s", buff);
    putchar('\n');
    return 0;
}
