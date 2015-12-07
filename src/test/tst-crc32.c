/**
 * crc32 test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_hash.h"
#include <stdio.h>
#include <stdint.h>

int main( int argc, char **args )
{
    uint32_t file_crc32;
    uint32_t str_crc32;

    if ( argc != 2 ) {
    printf("Usage: crc32 file path\n");
        return 0;
    }

    printf("+---test cel_crc32_hash: \n");
    str_crc32 = cel_crc32_hash("chenxin619315@gmail.com");
    printf("string crc32: 0x%x\n", str_crc32);
    putchar('\n');

    printf("+---test cel_crc32_file: \n");
    cel_crc32_file(args[1], &file_crc32);
    printf("file crc32: 0x%x\n", file_crc32);
    return 0;
}
