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

#define MIN_BLOCK_SIZE 16

size_t umax(size_t x, size_t y) { return x > y ? x : y; }

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

inline static word_t *prev_footer_ptr(byte_t *block_ptr) {
  return (word_t *)(block_ptr - DWORD);
}

inline static word_t *next_header_ptr(byte_t *block_ptr) {
  return (word_t *)(block_ptr + get_size(get_header(block_ptr)) - WORD);
}

inline static byte_t *prev(byte_t *block_ptr) {
  word_t prev_size = get_size(*prev_footer_ptr(block_ptr));
  return block_ptr - prev_size;
}

inline static byte_t *next(byte_t *block_ptr) {
  return block_ptr + get_size(get_header(block_ptr));
}

static void *head = NULL;

#define DEBUG_FLAG 0

// must write a mm_check
int mm_check(void) {

  if (!DEBUG_FLAG) {
    return 1;
  }

  // 我只需要遍历整个链表就行了，
  // 然后打印一下各个块的情况，ok
  byte_t *block_ptr = head;
  word_t header = get_header(block_ptr);
  word_t footer = get_footer(block_ptr);
  size_t block_size = get_size(header);
  while (block_size != 0) {
    assert(header == footer);
    // 输出一些信息
    // 这个快的大小和是否分配
    printf("%zu/%d/%p:%p, ", block_size, is_allocated(header),
           header_ptr(block_ptr), footer_ptr(block_ptr));
    block_ptr = next(block_ptr);
    header = get_header(block_ptr);
    footer = get_footer(block_ptr);
    block_size = get_size(header);
  }
  printf("\n");

  return 1;
}

// coalease block 一共有四种情况
static byte_t *coalease(byte_t *block_ptr) {

  // 我需要拿到前面一个block的footer和后一个block的header
  word_t prev_footer = *prev_footer_ptr(block_ptr);
  word_t next_header = *next_header_ptr(block_ptr);
  word_t size = get_size(get_header(block_ptr));

  bool is_prev_allocated = is_allocated(prev_footer);
  bool is_next_allocated = is_allocated(next_header);

  // case1
  if (is_prev_allocated && is_next_allocated) {
    // then nothing hapeen
    // just makr our block to be free
  }

  // case2
  else if (is_prev_allocated && !is_next_allocated) {
    // next_size = get_size(next_header)
    size += get_size(next_header);
  }
  // case3
  else if (!is_prev_allocated && is_next_allocated) {
    size += get_size(prev_footer);
    block_ptr = prev(block_ptr);
  }
  // case4
  else {
    // !is_prev_allocated && !is_next_allocated
    size += get_size(prev_footer);
    size += get_size(next_header);
    block_ptr = prev(block_ptr);
  }
  set_header(block_ptr, size);
  set_footer(block_ptr, size);

  return block_ptr;
}

static byte_t *extend_heap(size_t size) {
  // 我们扩展的大小必须也是对齐的，八位对齐，也就是分配偶数个word就行
  // 先不管了
  // 就假设对齐了吧

  word_t *brk = mem_sbrk(size);
  // set header
  *(brk - 1) = size;
  // set footer
  set_footer((byte_t *)brk, size);
  // set epilogue block
  *(footer_ptr((byte_t *)brk) + 1) = 0 | 1;

  // 我们扩展一个heap 相当于在最后一个位置插入了一个块
  // 所以 如果前一个块是free的状态 还需要coalease
  return coalease((byte_t *)brk);
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
  extend_heap(CHUNCK_SIZE);
  // 返回值要根据是否成功返回
  // -1: fail
  // 0: success

  mm_check();
  return 0;
}

// size需要包含header和footer
byte_t *find_block(size_t size) {

  // 我们有一个链表指针，始终指向第一个位置
  // 结束的位置就是epi
  byte_t *block_ptr = head;
  word_t header = get_header(block_ptr);
  size_t block_size = get_size(header);
  while (block_size != 0) {
    // 这样不好，我们可以假设函数参数size就是包含了header和footer的size，这样我们就不用每次都计算一次减法了
    if (!is_allocated(header) && block_size >= size) {

      // 我们就找到了一个能够容纳的block 就返回
      // cao 这里忘了检查是否allocated ...
      return block_ptr;
    }
    block_ptr = next(block_ptr);
    header = get_header(block_ptr);
    block_size = get_size(header);
  }

  return NULL;
}

// 函数参数中的size应该代表什么
// 如果正好代表一个块的size 就非常方便了
void split(byte_t *block_ptr, size_t size) {
  size_t curr_size = get_size(get_header(block_ptr));
  set_header(block_ptr, size | 1);
  set_footer(block_ptr, size | 1);
  block_ptr = next(block_ptr);
  set_header(block_ptr, curr_size - size);
  set_footer(block_ptr, curr_size - size);
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
  // 第一步应该是对size进行一些判断
  if (size == 0) {
    return NULL;
  }

  // 第二步应该是对size进行alignment
  size = ALIGN(size + DWORD);

  // 第三步，需要遍历整个链表，使用firstfir方法，找到一个大小能够容纳size的地址
  // size需要加上header和footer的大小
  byte_t *block_ptr = find_block(size);

  // 第四步如果没有找到，分配一个
  if (!block_ptr) {
    block_ptr = extend_heap(umax(size, CHUNCK_SIZE));
  }

  // 第五步 如果找到了, 可能会split
  // 需要确定是否需要split
  size_t block_size = get_size(get_header(block_ptr));
  if (block_size - size >= MIN_BLOCK_SIZE) {
    split(block_ptr, size);
  } else {
    // mark as allocated
    set_header(block_ptr, block_size | 1);
    set_footer(block_ptr, block_size | 1);
  }

  // mark as allocated

  // printf("malloc %u: ", size);
  // mm_check();

  return block_ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
  if (ptr) {
    coalease(ptr);
  }

  // printf("free %p: ", ptr);
  // mm_check();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;
  size_t newSize;

  if (ptr == NULL) {
    return mm_malloc(size);
  }

  if (size == 0) {
    mm_free(ptr);
    return NULL;
  }

  size_t initial_size = size;

  // case 1: truncate block
  word_t header = get_header(ptr);
  size_t block_size = get_size(header);

  size = ALIGN(size + DWORD);
  newptr = ptr;

  // 艹 这里溢出了
  if (size <= block_size && block_size - size >= MIN_BLOCK_SIZE) {
    split(ptr, size);
    newptr = ptr;
  } else if (size <= block_size) {
  } else {

    // todo
    // 其实还有一种情况不需要复制，就是我们的下一个块是free的，如果加上这个块刚好够的话
    // 就不用动了 就直接合并成一个 然后做一个split就可以了
    byte_t *next_block = next(ptr);
    word_t next_header = get_header(next_block);
    if (!is_allocated(next_header) &&
        block_size + get_size(next_header) >= size) {
      // 这里我们直接合并就可以了，然后再执行一次可选的split
      set_header(ptr, (block_size + get_size(next_header)) | 1);
      set_footer(ptr, (block_size + get_size(next_header)) | 1);

      // 然后还需要执行一次split
      newSize = block_size + get_size(next_header);
      if (newSize - size >= MIN_BLOCK_SIZE) {
        split(ptr, size);
      }
    }

    // 现在我们不得不重新分配一块更大的内存
    newptr = mm_malloc(initial_size);
    if (!newptr) {
      return NULL;
    }

    copySize = block_size - DWORD;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
  }

  // printf("realloc %p: ", ptr);
  // mm_check();

  return newptr;
}
