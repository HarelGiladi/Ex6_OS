#include <iostream>

namespace Ex4
{
    typedef struct free_block
    {
        size_t size;
        struct free_block *next;
    } free_block;

    class Mem_Imp
    {
    public:
        static void *malloc(size_t size);
        static void free(void *ptr);
        
        static void *my_calloc(size_t n, size_t size);
        static void *my_calloc_helper(size_t n, size_t size);
        static void *my_malloc_helper(size_t size);
        static void my_free_helper(void *ptr);
    };
}