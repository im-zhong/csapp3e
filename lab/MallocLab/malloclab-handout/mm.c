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
#include <stdarg.h>
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
// 好的好的 第一步已经实现完了
// 第二步 现在需要实现一个 explicit free list
// 还有一个优化 就是去掉footer tag

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

// list

typedef struct list_node_t {
  struct list_node_t *next, *prev;
} list_node_t;

static inline void list_init_head(list_node_t *head) {
  head->next = head;
  head->prev = head;
}

static inline void list_insert_between(list_node_t *prev, list_node_t *next,
                                       list_node_t *node) {
  prev->next = node;
  node->prev = prev;
  next->prev = node;
  node->next = next;
}

static inline void list_insert_after(list_node_t *head, list_node_t *node) {
  list_insert_between(head, head->next, node);
}

static inline void list_insert_before(list_node_t *head, list_node_t *node) {
  list_insert_between(head->prev, head, node);
}

static inline void list_erase_between(list_node_t *prev, list_node_t *next) {
  prev->next = next;
  next->prev = prev;
}

static inline void list_erase(list_node_t *node) {
  list_erase_between(node->prev, node->next);
}

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

// static void *head = NULL;

static void *heap_ptr = NULL;
// 我们还应该实现一个get_list_head
static inline list_node_t *get_list_head() { return *(list_node_t **)heap_ptr; }

#define DEBUG_FLAG 0
#define CHECK_FLAG 0

void debug_printf(const char *format, ...) {
  if (DEBUG_FLAG) {
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
  }
}

// must write a mm_check
int mm_check(void) {

  if (!CHECK_FLAG) {
    return 1;
  }

  // todo 这个函数也需要改了
  // 我们需要从两个途径来遍历 一个是根据 implicit free list 遍历
  // 一个是根据 explicit free list 来遍历
  // 我只需要遍历整个链表就行了，
  // 然后打印一下各个块的情况，ok
  size_t free_block_count = 0;
  byte_t *block_ptr = (byte_t *)heap_ptr + DWORD;
  word_t header = get_header(block_ptr);
  word_t footer = get_footer(block_ptr);
  size_t block_size = get_size(header);
  while (block_size != 0) {
    assert(header == footer);
    // 输出一些信息
    // 这个快的大小和是否分配
    // debug_printf("%zu/%d/%p:%p, ", block_size, is_allocated(header),
    //              header_ptr(block_ptr), footer_ptr(block_ptr));
    block_ptr = next(block_ptr);
    header = get_header(block_ptr);
    footer = get_footer(block_ptr);
    block_size = get_size(header);

    if (!is_allocated(header)) {
      ++free_block_count;
    }
  }
  // debug_printf("\n");

  // 我需要保证所有free状态的节点都在freelist里面，她们两者应该是一一匹配的
  // 这个要怎么检查呢??

  // 然后我需要遍历一下 explicit free list
  // 我在这里统计free_list节点的数量就可以了
  size_t list_node_count = 0;
  list_node_t *head = get_list_head();
  for (list_node_t *first = head->next; first != head; first = first->next) {
    // first 刚好就是 block_ptr 的地址 只不过需要指针类型转换
    word_t header = get_header((byte_t *)first);
    size_t block_size = get_size(header);
    ++list_node_count;
  }

  assert(free_block_count == list_node_count);
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
    // 因为下一个要被合并了 所以应该从free list中删除
    list_erase((list_node_t *)next(block_ptr));
  }
  // case3
  else if (!is_prev_allocated && is_next_allocated) {
    size += get_size(prev_footer);
    // 我们要合并到上一个，所以删掉我们自己就行了
    list_erase((list_node_t *)block_ptr);
    block_ptr = prev(block_ptr);
  }
  // case4
  else {
    // !is_prev_allocated && !is_next_allocated
    size += get_size(prev_footer);
    size += get_size(next_header);

    // 我们和我们下一个都要合并到上一个 所以删掉我们和下一个就可以了
    list_erase((list_node_t *)next(block_ptr));
    list_erase((list_node_t *)block_ptr);

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

  // insert this new block into our explicit free list
  list_insert_after(get_list_head(), (list_node_t *)brk);

  // 我们扩展一个heap 相当于在最后一个位置插入了一个块
  // 所以 如果前一个块是free的状态 还需要coalease
  return coalease((byte_t *)brk);
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  // 好的好的 现在需要修改结构，我需要把一个双链表节点结构放在这里面
  // 我决定把指向head节点的head指针也放在heap里面 这样在后面实现segregated
  // list的时候 就不用每个size
  // class一个全局变量了，她们都得存放在heap的一个数组里面
  // 那么问题来了，我们为什么不一上来就分配一个page呢？
  // 或许这样编程简单一点？？
  word_t *brk = mem_sbrk(6 * WORD);
  // set heap ptr
  heap_ptr = brk;
  // set list head in the first word
  *(list_node_t **)heap_ptr = (list_node_t *)(brk + 2);

  // begin block
  // header
  *(brk + 1) = MIN_BLOCK_SIZE | 1;
  list_init_head(get_list_head());
  // footer
  *(brk + 4) = MIN_BLOCK_SIZE | 1;

  // end block
  *(brk + 5) = 0 | 1;

  // 然后我们扩充整个head 就像没有内存的时候一样
  extend_heap(CHUNCK_SIZE);
  // 返回值要根据是否成功返回
  // -1: fail
  // 0: success

  mm_check();
  return 0;
}

// 因为我们是32位的，所以刚好可以放在一个word里面

byte_t *first_fit(size_t size) {
  list_node_t *head = get_list_head();
  for (list_node_t *first = head->next; first != head; first = first->next) {
    // first 刚好就是 block_ptr 的地址 只不过需要指针类型转换
    word_t header = get_header((byte_t *)first);
    size_t block_size = get_size(header);
    if (!is_allocated(header) && size <= block_size) {
      return (byte_t *)first;
    }
  }
  return NULL;
}

// 放弃实现 next fit
// static list_node_t *next_fit_head = NULL;
// byte_t *next_fit(size_t size) {
//   list_node_t *head = next_fit_head;
//   list_node_t *first = head->next;
//   // 还有一种情况，如果我们最开始指向的next_fit_head被拿走了怎么办
//   // 或者free了之后 结构变了 next fit 还是不太适合链表结构啊 就不实现了
//   for (; first != head; first = first->next) {
//     // first 刚好就是 block_ptr 的地址 只不过需要指针类型转换
//     word_t header = get_header((byte_t *)first);
//     size_t block_size = get_size(header);
//     if (!is_allocated(header) && size <= block_size) {
//       return (byte_t *)first;
//     }
//   }
//   // 更新head 好让
//   next_fit_head = first;
//   return NULL;
// }

// size需要包含header和footer
byte_t *find_block(size_t size) {

  // 我们有一个链表指针，始终指向第一个位置
  // 结束的位置就是epi
  // 那合理就必须得全部修改了
  // 因为现在我们只需要遍历explict free list
  return first_fit(size);
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
  // split出来的这个块 我得再加到list里面
  list_insert_after(get_list_head(), (list_node_t *)block_ptr);
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
// 我用objdump -d 看了一下 果然 find_block被inline了 怪不得 gprof里面看不到呢
// 这个函数里面唯一的循环就是 find_block 而影响find_block性能的因素就是
// 没有使用segrated fit 所以接下来就实现这个！
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

  list_erase((list_node_t *)block_ptr);

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

  debug_printf("malloc %zu: \n", size);
  mm_check();

  return block_ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
  if (ptr) {
    // 现在free需要把节点添加到explicit free list链表头
    list_insert_after(get_list_head(), (list_node_t *)ptr);
    coalease(ptr);
  }

  debug_printf("free %p: \n", ptr);
  mm_check();
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
    // size > block_size

    // todo
    // 其实还有一种情况不需要复制，就是我们的下一个块是free的，如果加上这个块刚好够的话
    // 就不用动了 就直接合并成一个 然后做一个split就可以了
    byte_t *next_block = next(ptr);
    word_t next_header = get_header(next_block);
    if (!is_allocated(next_header) &&
        block_size + get_size(next_header) >= size) {
      // 这里我们直接合并就可以了，然后再执行一次可选的split
      list_erase((list_node_t *)next_block);
      set_header(ptr, (block_size + get_size(next_header)) | 1);
      set_footer(ptr, (block_size + get_size(next_header)) | 1);
      // 合并之后，我就需要把下个块从free list中删除

      // 然后还需要执行一次split
      newSize = block_size + get_size(next_header);
      if (newSize - size >= MIN_BLOCK_SIZE) {
        split(ptr, size);
      }
    } else {
      // 现在我们不得不重新分配一块更大的内存
      newptr = mm_malloc(initial_size);
      if (!newptr) {
        return NULL;
      }

      copySize = block_size - DWORD;
      memcpy(newptr, oldptr, copySize);
      mm_free(oldptr);
    }
  }

  debug_printf("realloc %p: \n", ptr);
  mm_check();

  return newptr;
}
