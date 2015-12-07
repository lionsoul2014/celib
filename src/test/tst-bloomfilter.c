/**
 * cel bloom filter test program.
 *
 * @author    chenxin <chenxin619315@gmail.com>
 */
#include "../cel_bloomfilter.h"
#include <stdio.h>

int main( int argc, char **args )
{
    uint_t i;

    //exists sets
    char *str[] = {
        "father", "mother", "chenxin", "roe", "jcseg", "friso", "robbe", "lionsoul"
    };

    //Not exists sets
    char *str1[] = {
        "lvs", "tankwar", "syrian", "opert", "php"
    };
    
    //cel bloom
    //there is 6 elements and we keep the m/n = 32
    //and we will get the smallest False positive rate.
    //cel_bloomfilter_t *bloom = new_cel_bloomfilter(8 * 32, 8);

    cel_bloomfilter_t bf, *bloom;
    bloom = &bf;
    cel_bloomfilter_create(&bf, 8 * 32, 8);

    printf("+---test add elements: \n");
    for ( i = 0; i < 8; i++ ) 
    {
       cel_bloomfilter_add(bloom, str[i]);
    }
    printf("size=%d\n", cel_bloomfilter_size(bloom));
    putchar('\n');

    printf("+---test check exists elements: \n");
    for ( i = 0; i < 8; i++ ) 
    {
           printf("exists(%s)=%d\n", str[i], cel_bloomfilter_exists(bloom, str[i])); 
    }
    putchar('\n');

    printf("+---test check not exists elements: \n");
    for ( i = 0; i < 5; i++ ) 
    {
       printf("exists(%s)=%d\n", str1[i], cel_bloomfilter_exists(bloom, str1[i]));
    }
    putchar('\n');

    //free the bloom filter.
    //free_cel_bloomfilter(&bloom);
    cel_bloomfilter_destroy(bloom);

    return 0;
}

