/**
 * cel lzw comnpress module test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_compress.h"
#include "../cel_array.h"
#include "../cel_string.h"
#include <stdio.h>
#include <string.h>

int main( int argc, char *args[] )
{
    //char * str = "ababcdefefgggghihijklmnmnopqrsoptuvwxyzxyzgg.";
    char *str = "AAAAAAABBBBBBABABABCDCDCDEFGHIJKLMNOPPQOPRSTUVWXYZZL";
    //char * str = "the/rain/in/Spain/falls/mainly/on/the/plain";
    //char * str = "where there is a will and there is a way";
    //char * str = "ababcdefgefg";
    //char * str = "everyone has potential energy, which is easily concealed by habits, blurred by time, by laziness(inertia)";
    //char * str = "http://translate.google.cn/#zh-CN/en/%E5%BE%88%E5%AE%B9%E6%98%93";
    //char * str = "测试lzw压缩算法的测试程序-研究生命起源起源, 研研究生测试算法lzw的程序.";
    uint_t len = strlen(str);
    cel_strbuff_t  *ensb = new_cel_strbuff();
    cel_strbuff_t  *desb = new_cel_strbuff();

    printf("data=%s, size=%d\n", str, len);

    printf("+---test encode: \n");
    int r = cel_lzw_encode_string(str, len, ensb);
    if ( r == 0 ) 
	printf("error: unable to compress the string.");
    else {
	printf("data=%s\n", ensb->buffer);
	printf("size=%d, radio=%.2f\n", ensb->size, ((float)ensb->size)/((float)len));
    }
    putchar('\n');


    printf("+---test decode: \n");
    r = cel_lzw_decode_string(ensb->buffer, ensb->size, desb);
    if ( r == 0 )
        printf("error: unable to decompress the string.");
    else {
	printf("+status: %s\n", strcmp(str, desb->buffer)==0?"ok":"no");
        printf("data=%s\n", desb->buffer);
        printf("B::opacity=%d, size=%d\n", desb->opacity, desb->size);
    }
    putchar('\n');

    free_cel_strbuff(&ensb);
    free_cel_strbuff(&desb);

    return 0;
}
