#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vsf_init()
{
    char message[50] = {0};
    char *ptr = NULL;
    ptr = malloc(20);
    memcpy(ptr, "vsf init done.", strlen("vsf init done."));
    printf("[VSF] %s\n", ptr);
    printf("[VSF] malloc:%p\n", malloc);

    memcpy(message , malloc, 8);
    printf("[VSF] message:%p\n", message);

    free(ptr);
    ptr = NULL;
}
