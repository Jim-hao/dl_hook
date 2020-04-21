#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "link.h"
#include <elf.h>
#include <stdlib.h>

#define ROUND_UP(x, align) (((int) (x) + (align - 1)) & ~(align - 1))
#define ROUND_DOWN(x, align) ((int)(x) & ~(align - 1))
#define ROUND_SIZE  4*1024

#if 0
/* 注册的回调结构体 */
struct dl_phdr_info
{
    ElfW(Addr) dlpi_addr;
    const char *dlpi_name;
    const ElfW(Phdr) *dlpi_phdr; /* 数组 */
    ElfW(Half) dlpi_phnum;       /* 数组元素个数 */

    /* Note: Following members were introduced after the first
       version of this structure was available.  Check the SIZE
       argument passed to the dl_iterate_phdr callback to determine
       whether or not each later member is available.  */
 
    /* Incremented when a new object may have been added.  */
    __extension__ unsigned long long int dlpi_adds;
    /* Incremented when an object may have been removed.  */
    __extension__ unsigned long long int dlpi_subs;
 
    /* If there is a PT_TLS segment, its module ID as used in
       TLS relocations, else zero.  */
    size_t dlpi_tls_modid;

    /* The address of the calling thread's instance of this module's
       PT_TLS segment, if it has one and it has been allocated
       in the calling thread, otherwise a null pointer.  */
    void *dlpi_tls_data;
};

/* elf文件头结构体 */
typedef struct
{
   unsigned char e_ident[EI_NIDENT];     /* Magic number and other info */
   Elf64_Half    e_type;                 /* Object file type */
   Elf64_Half    e_machine;              /* Architecture */
   Elf64_Word    e_version;              /* Object file version */
   Elf64_Addr    e_entry;                /* Entry point virtual address */
   Elf64_Off     e_phoff;                /* Program header table file offset      */
   Elf64_Off     e_shoff;                /* Section header table file offset      */
   Elf64_Word    e_flags;                /* Processor-specific flags */
   Elf64_Half    e_ehsize;               /* ELF header size in bytes */
   Elf64_Half    e_phentsize;            /* Program header table entry size   */
   Elf64_Half    e_phnum;                /* Program header table entry count      */
   Elf64_Half    e_shentsize;            /* Section header table entry size  */
   Elf64_Half    e_shnum;                /* Section header table entry count      */
   Elf64_Half    e_shstrndx;             /* Section header string table index      */
} Elf64_Ehdr;
#endif



static int  callback(struct dl_phdr_info *info, size_t size, void *data)
{
     int j = 0;

//     if (j <= 1)
     {
        j++;
        printf("dlpi_addr:%p\n", info->dlpi_addr);

     	printf("name=%s (%d segments)\n", info->dlpi_name,
                                       info->dlpi_phnum);

//      if (strncmp(info->dlpi_name, "test.out", strlen("test.out")) == 0 
//         || (info->dlpi_name == NULL))
//	{
//             for (j = 0; j < info->dlpi_phnum; j++)
//             {
//                 printf("\t\t header %2d: address=%10p\n", 
//	            j, (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
//             }
         
//	 }
     }   

     
     return 0;
}


void get_elf_head()
{ 
    /* 1.根据加载地址获取elf头信息 */
    FILE *fp = NULL;
    Elf64_Ehdr elfheader={0};

    fp = fopen("test.out", "rw");
    fread(&elfheader, sizeof(Elf64_Ehdr), 1, fp);  
    #if 0
    printf("[stub] Elf File e_entry: %lx\n", elfheader.e_entry);
    printf("[stub] Elf File e_phoff: %lx\n", elfheader.e_phoff);
    printf("[stub] Elf File e_shoff: %lx\n", elfheader.e_shoff);
    printf("[stub] Elf File e_flags: %lx\n", elfheader.e_flags);
    printf("[stub] Elf File e_ehsize:%hx\n", elfheader.e_ehsize);
    printf("[stub] Elf File e_phnum: %hx\n", elfheader.e_phnum);
    printf("[stub] Elf File e_shnum: %hx\n", elfheader.e_shnum);
    #endif
    fclose(fp);
    fp = NULL;
    
    
    /* 2.获取加载地址 */
    void *ptr          = elfheader.e_entry;
    void *ptr_tmp      = ptr - ROUND_SIZE;
    void *ptr_align_up = ROUND_UP(ptr_tmp, ROUND_SIZE);  
    printf("[stub] ptr=%p, ptr-4K=%p, ptr_align_up=%p\n", ptr, ptr_tmp, ptr_align_up);   

  
    /* 3.根据真正的加载地址获取进程的头文件信息 */
    char headbuf[100] = {0};
    memset(&elfheader, 0, sizeof(elfheader));
    memcpy(&elfheader, ptr_align_up, sizeof(elfheader));

    printf("[stub] process e_entry: %lx\n", elfheader.e_entry);
    printf("[stub] process e_phoff: %lx\n", elfheader.e_phoff);
    printf("[stub] process e_shoff: %lx\n", elfheader.e_shoff);
    printf("[stub] process e_flags: %lx\n", elfheader.e_flags);
    printf("[stub] process e_ehsize:%hx\n", elfheader.e_ehsize);
    printf("[stub] process e_phnum: %hx\n", elfheader.e_phnum);
    printf("[stub] process e_shnum: %hx\n", elfheader.e_shnum);
}


void get_elf_section()
{

}


int  stub_init()
{
    void *ptr = NULL;

    printf("[STUB] first malloc address:%p\n", malloc);
    ptr = malloc(20);
    memcpy(ptr, "in stub",  strlen("in stub"));
    printf("[STUB] second malloc address:%p\n", malloc);
    free(ptr);
    ptr = NULL;

    /* 获取elf头, 目的为了获取段表 */
    get_elf_head();

    /* 解析出段表 */
    get_elf_section();

    /* 获取每个动态库的加载地址  */
//    dl_iterate_phdr(callback, NULL);
    
    //exit(EXIT_SUCCESS);
    return  0;
}


