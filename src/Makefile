#celib Make file
#author chenxin<chenxin619315@gmail.com>
CC = gcc
#flag for develop
CFLAGS = -g -Wall
#flag for products
FFLAGS = -O2 -Wall -fPIC
#extra library
EXTRA_LIB =
#lib file
SH_FILE = libcelib.so
#install path
LIB_DIR = /usr/lib
INCLUDE_DIR = /usr/include/celib

SOURCE = cel_array.o cel_bitmap.o cel_bloomfilter.o cel_bstree.o cel_hashmap.o cel_hash.o cel_crc32.o cel_md5.o cel_link.o cel_sort.o cel_stdio.o cel_string.o cel_math.o cel_compress.o

all: share

share: $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -fPIC -shared $(EXTRA_LIB) -o $(SH_FILE)

cel_array.o: cel_api.h cel_array.h cel_array.c
	$(CC) $(CFLAGS) -c cel_array.c

cel_bitmap.o: cel_api.h cel_bitmap.h cel_bitmap.c
	$(CC) $(CFLAGS) -c cel_bitmap.c

cel_hash.o: cel_api.h cel_hash.h cel_hash.c
	$(CC) $(CFLAGS) -c cel_hash.c
	
cel_md5.o: cel_api.h cel_hash.h cel_md5.c
	$(CC) $(CFLAGS) -c cel_md5.c

cel_crc32.o: cel_api.h cel_hash.h cel_hash.c cel_crc32.c
	$(CC) $(CFLAGS) -c cel_crc32.c

cel_bloomfilter.o: cel_api.h cel_hash.c cel_hash.h cel_bloomfilter.h cel_bloomfilter.c
	$(CC) $(CFLAGS) -c cel_bloomfilter.c
	
cel_bstree.o: cel_api.h cel_bstree.h cel_bstree.c
	$(CC) $(CFLAGS) -c cel_bstree.c
	
cel_hashmap.o: cel_api.h cel_hash.h cel_hash.c cel_math.h cel_math.c cel_hashmap.h cel_hashmap.c
	$(CC) $(CFLAGS) -c cel_hashmap.c
	
cel_link.o: cel_api.h cel_link.h cel_link.c
	$(CC) $(CFLAGS) -c cel_link.c
	
cel_sort.o: cel_api.h cel_stdio.h cel_stdio.c cel_sort.h cel_sort.c
	$(CC) $(CFLAGS) -c cel_sort.c
	
cel_string.o: cel_api.h cel_string.h cel_string.c
	$(CC) $(CFLAGS) -c cel_string.c
	
cel_math.o: cel_api.h cel_math.h cel_math.c
	$(CC) $(CFLAGS) -c cel_math.c

cel_stdio.o: cel_api.h cel_stdio.h cel_stdio.c
	$(CC) $(CFLAGS) -c cel_stdio.c

cel_compress.o: cel_api.h cel_compress.h cel_compress.c
	$(CC) $(CFLAGS) -c cel_compress.c

#clean object
.PHONY: clean
clean:
	find . -name \*.o | xargs rm -f
	find . -name \*.so | xargs rm -f
	
	

install:
	@if [ -d $(LIB_DIR) ];then\
			cp $(SH_FILE) $(LIB_DIR);\
			sudo chmod a+x $(LIB_DIR)/$(SH_FILE);\
			sudo chmod og-w $(LIB_DIR)/$(SH_FILE);\
			echo "Congratulation, celib install successfully.";\
		else\
			echo "Fail to install celib, $(LIB_DIR) is not exist.";\
	fi
	@if [ ! -d $(INCLUDE_DIR) ];then\
	    mkdir $(INCLUDE_DIR);\
	fi
	@cp *.h $(INCLUDE_DIR);\
	chmod a+r $(INCLUDE_DIR)/*.h;\
	chmod a+x $(INCLUDE_DIR)/*.h;
