
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "Mem_Imp.hpp"
using namespace Ex5;

static free_block free_block_list_head = { 0, 0 };
static const size_t align_to = 16;

void* Mem_Imp::malloc(size_t size) {
    size = (size + sizeof(size_t) + (align_to - 1)) & ~ (align_to - 1);
    free_block* block = free_block_list_head.next;
    free_block** head = &(free_block_list_head.next);
    while (block != 0) {
        if (block->size >= size) {
            *head = block->next;
            return ((char*)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
}

    block = (free_block*)sbrk(size);
    block->size = size;

    return ((char*)block) + sizeof(size_t);
}

void Mem_Imp::free(void* ptr) {
    free_block* block = (free_block*)(((char*)ptr) - sizeof(size_t));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}

void *Mem_Imp::calloc(size_t n, size_t size)
{
    size_t total_size = n * size;
    void *p = Mem_Imp::malloc(total_size);

    if (!p){return NULL;}

    return memset(p, 0, total_size);
}