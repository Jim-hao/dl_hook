# Written by Jim
# 2020-4-8

#ARCH=arm64
ARCH=x86

src = ./main.c
stub = ./stub_malloc.c
vsf = ./vsf.c
target = test.out

ifeq ($(ARCH), arm64)
CC=aarch64-himix100-linux-gcc
else ifeq ($(ARCH), x86)
CC=gcc
endif

ifeq ($(ARCH), arm64)
LDPRE_LD = -g -lpthread  -ldl-2.24  -L. -lstub_malloc -lvsf  -O0    #-fsanitize=address
else ifeq ($(ARCH), x86)
LDPRE_LD = -g -lpthread  -ldl  -L. -lstub_malloc -lvsf -O0 
endif

lib:
	$(CC) -W -shared -fPIC $(stub) -o libstub_malloc.so  
	$(CC) -W -shared -fPIC $(vsf) -o libvsf.so  
all:
	$(CC)  $(src) $(LDPRE_LD) -o  $(target)
cp:
	cp ./*.c       /home/ubuntu14/Downloads/samba/dlmalloc/  -rf
	cp ./Makefile  /home/ubuntu14/Downloads/samba/dlmalloc/  -rf

clean:
	rm ./*.out   ./*~   ./*.so   ./*.a
