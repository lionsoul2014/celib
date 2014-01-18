/**
 * cel bitmap test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_bitmap.h"
#include <stdio.h>

int main( int argc, char *args[] )
{
    int a[] = {3, 5, 9, 6, 10, 30, 16, 18};
    int i, length = 8;

    //cel_bitmap_t *bitmap = new_cel_bitmap(30);
    cel_bitmap_t bit, *bitmap;
    bitmap = &bit;
    cel_bitmap_create(&bit, 30);

    //set the bits
    for ( i = 0; i < length; i++ ) 
    {
	   cel_bitmap_set( bitmap, a[i] );
    }

    printf("+---bitmap bucket sort: \n");

    //get the bits
    for ( i = 0; i < bitmap->length; i++ ) 
    {
    	if ( cel_bitmap_get(bitmap, i) == 1 ) 
        {
    	    //printf("1");
    	    printf("%3d", i);
    	} /*else {
    	    printf("0");
    	}*/
    }
    putchar('\n');

    cel_bitmap_remove( bitmap, 3 );
    for ( i = 0; i < bitmap->length; i++ ) 
    {
    	if ( cel_bitmap_get(bitmap, i) == 1 ) 
        {
    	    printf("%3d", i);
    	}
    }
    putchar('\n');

    //free_cel_bitmap( &bitmap );
    cel_bitmap_destroy(bitmap);

    return 0;
}
