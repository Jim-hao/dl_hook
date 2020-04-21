#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dlfcn.h>
#include <unistd.h>

extern  int stub_init();
extern  void vsf_init();

int main(int argc,  char *argv[])
{
    getchar();

    int num = argc > 2 ? 0 : 1;
    char *getcmd = argv[num];
    int a[10] = {0};    a[10] = 518;
    void *handle = NULL;
    int (*real_malloc)(int);


    if ((getcmd != NULL) && (strncmp(getcmd, "x86", 3) == 0))
    {
        printf("[MAIN] try dlopen x86 libc.so.6\n");
        handle = dlopen("libc.so.6", RTLD_LAZY);
    }
    else
    {
        handle = dlopen("libc-2.24.so", RTLD_LAZY);
    }
   
    /* 获取dlsym拿到的malloc地址 */
    printf("[MAIN] malloc:%p\n", malloc);    
    *(int  **)&real_malloc = dlsym(handle, "malloc");
    printf("[MAIN] realmalloc:%p\n",  real_malloc);
    
    /* 使用一次malloc  */
    void *ptr=NULL;
    ptr = malloc(20);
    memset(ptr, 0, 20);
    printf("[MAIN] malloc after use:%p\n", malloc);    
    free(ptr);

    dlclose(handle);
 
    /* */
    stub_init();

    /* 第二次使用 */
    ptr = malloc(20);
    memset(ptr, 0, 20);
    printf("[MAIN] malloc after use twice:%p\n", malloc);    
    free(ptr);

    vsf_init();

    return 0;
}
