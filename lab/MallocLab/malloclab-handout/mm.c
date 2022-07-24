/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

// 接下来的事情就很简单了，实现一个书上描述的最优的分配器
// 再优化的话，可能需要查阅一些论文了
// segrated list 延迟coalesce
// 但是每个链表的数据大小要怎么分呢？这也是一个问题，或许每个大小都应该有一个链表吧
// 专业一点 叫做 size class 这可能是一个主要的设计参数了
// 如果我的系统可以根据这个参数进行编程就好了
// 这样我就不用重新写一遍了

// 不过，第一步 先实现一个简单的implicit free list

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "zhong",
    /* First member's full name */
    "zhong",
    /* First member's email address */
    "im.zhong@outlook.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WORD 4
#define DWORD 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

// 大小可以是一个page 4K = 2^12 = 1 << 12
#define CHUNCK_SIZE (1 << 12)

typedef char byte_t;
typedef unsigned word_t;
// 实现一系列辅助函数
inline static bool is_allocated(word_t word) { return word & 1; }
inline static bool is_free(word_t word) { return !is_allocated(word); }

inline static word_t *header_ptr(byte_t *block_ptr) {
  return (word_t *)(block_ptr - WORD);
}

inline static word_t get_header(byte_t *block_ptr) {
  return *header_ptr(block_ptr);
}

// 可能用户会传header 也可能是footer
// 而且我们还应该有更多函数
inline static size_t get_size(word_t word) { return word & ~(ALIGNMENT - 1); }

inline static word_t *footer_ptr(byte_t *block_ptr) {
  return (word_t *)(block_ptr + get_size(get_header(block_ptr)) - DWORD);
}

inline static word_t get_footer(byte_t *block_ptr) {
  return *footer_ptr(block_ptr);
}

inline static void set_header(byte_t *block_ptr, word_t word) {
  *header_ptr(block_ptr) = word;
}

inline static void set_footer(byte_t *block_ptr, word_t word) {
  *footer_ptr(block_ptr) = word;
}

static void *head = NULL;

// todo coalease block 一共有四种情况
static void coalease(byte_t *block_ptr) {}

static void extend_heap(size_t size) {
  // 我们扩展的大小必须也是对齐的，八位对齐，也就是分配偶数个word就行
  // 先不管了
  // todo 就假设对齐了吧
  word_t *brk = mem_sbrk(size);
  // set header
  *(brk - 1) = size;
  // set footer
  set_footer((byte_t *)brk, size);
  // set epilogue block
  *(footer_ptr((byte_t *)brk) + 1) = 0 | 1;

  // 我们扩展一个heap 相当于在最后一个位置插入了一个块
  // 所以 如果前一个块是free的状态 还需要coalease
  coalease((byte_t *)brk);
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  word_t *brk = mem_sbrk(4 * WORD);
  *brk = 0;
  *(brk + 1) = ALIGNMENT | 1;
  *(brk + 2) = ALIGNMENT | 1;
  *(brk + 3) = 0 | 1;
  head = brk + 2;

  // 然后我们扩充整个head 就像没有内存的时候一样
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
  if (size < copySize)
    copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}

// must write a mm_check
int mm_check(void) { return 1; }