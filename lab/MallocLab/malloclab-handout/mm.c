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
#include <sys/time.h>
#include <time.h>
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

// 第三步，设计一个segregated list 系统
// 简单来说 就是实现一个函数 输入size 输出对应的链表数组的下标
// 这样的话，具体的策略可以修改 而不用修改实现代码

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

// 那这样的话，我需要实现一个辅助函数，通过查表实先2的幂的log

#define MIN_BLOCK_SIZE 16

// 实现一个计时函数 我需要观察 find_block为什么这么慢
void performance() {
  struct timespec begin;
  clock_gettime(CLOCK_MONOTONIC, &begin);

  // do something
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);
  end.tv_nsec - begin.tv_nsec;
}

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

// static inline unsigned ulog2(unsigned x) {
//   float fx = (float)x;
//   unsigned ux = *(unsigned *)&fx;
//   return ((ux >> 23) & 0xff) - 127;
// }

size_t umax(size_t x, size_t y) { return x > y ? x : y; }

// 添加一个新的全局变量 表示heap_size
size_t heap_size = 0;

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
static list_node_t *segregated_lists = NULL;
// 我们还应该实现一个get_list_head
// static inline list_node_t *get_list_head() { return *(list_node_t
// **)heap_ptr; }

#define MAX_LINEAR_SIZE 512
// 这个参数没有用了
// #define MAX_SEGRETATED_SIZE 4096
// 最后一部分手算吧 反正只需要修改这些宏就可以了
// (MAX_SEGRETATED_SIZE/MAX_LINEAR_SIZE) = 2^k
#define SEGREGATED_LIST_SIZE (MAX_LINEAR_SIZE / 8)

// todo 又想到了一个点子
// 我们可以规划一块专门的小内存区域 这块区域不进行 coalease
// 不用于大内存分配 同时 所占据的空间也应该较小 这样不至于特别浪费
// #define CACHE_MALLOC_SIZE 4096
#define SMALL_SIZE (4096 / 2)

// #define MAX_LINEAR_SIZE 1024
// #define MAX_SEGRETATED_SIZE 4096
// #define SEGREGATED_LIST_SIZE ((MAX_LINEAR_SIZE / 8) + 2)

// #define MAX_LINEAR_SIZE 256
// #define MAX_SEGRETATED_SIZE 4096
// #define SEGREGATED_LIST_SIZE ((MAX_LINEAR_SIZE / 8) + 4)

// #define MAX_LINEAR_SIZE 64
// #define MAX_SEGRETATED_SIZE 4096
// #define SEGREGATED_LIST_SIZE ((MAX_LINEAR_SIZE / 8) + 6)

// todo
// 接下来就是实现关键的这两个函数 其实基本上就算实现完了
// 剩下的代码几乎不需要改动 只需要把调用 get_list_head的地方改成新的函数就行
// segregated free list
// 128个item不算多 覆盖整张page 4K
static inline size_t size_class(size_t size) {

  // 那么我们就必须要进行一个assert了
  assert((size & 7) == 0);
  assert(size >= MIN_BLOCK_SIZE);
  if (size > MAX_LINEAR_SIZE) {
    return SEGREGATED_LIST_SIZE - 1;
  } else {
    return (size >> 3) - 2;
  }
}

static inline list_node_t *get_list_head(size_t size) {
  size_t index = size_class(size);
  // 首先获得数组的首地址，
  return segregated_lists + index;
}

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

// 两个区域的begin和end指针可以写成函数
static inline byte_t *small_begin() {
  return (byte_t *)heap_ptr + SEGREGATED_LIST_SIZE * sizeof(list_node_t);
}

static inline byte_t *small_end() { return (byte_t *)heap_ptr + SMALL_SIZE; }

static inline byte_t *large_begin() { return (byte_t *)heap_ptr + SMALL_SIZE; }

static inline byte_t *large_end() { return (byte_t *)heap_ptr + heap_size; }

static inline bool is_small(list_node_t *node) {
  return (byte_t *)node - (byte_t *)heap_ptr < SMALL_SIZE;
}

static inline bool is_large(list_node_t *node) {
  return (byte_t *)node - (byte_t *)heap_ptr > SMALL_SIZE;
}

// implicit 本身也是一个链表 所以我们给定beginblock的地址和endblock的地址
// 我们就可以包装一些逻辑
void check_contiguous_free(byte_t *begin, byte_t *end) {
  assert(*(word_t *)begin == 1);
  assert(*(word_t *)(end - WORD) == 1);
  assert(begin < end);

  word_t header = 0;
  bool prev_allocated = true;
  for (byte_t *block = begin + DWORD; block != end; block = next(block)) {
    header = get_header(block);
    assert(!(!prev_allocated && !is_allocated(header)));
    prev_allocated = is_allocated(header);
  }
}

// 遍历整个数据结构，统计所有byte数据 检查header和footer是否一致
// 并且返回最终的大小和free block以及
void check_implicit_list(byte_t *begin, byte_t *end, size_t *size,
                         size_t *free_block) {
  assert(*(word_t *)begin == 1);
  assert(*(word_t *)(end - WORD) == 1);
  assert(begin < end);

  word_t header = *(word_t *)begin;
  word_t footer = 0;
  size_t block_size = 0;
  *size = 0;
  *free_block = 0;
  debug_printf("%p:%zu,%d", begin, get_size(header), is_allocated(header));
  for (byte_t *block = begin + DWORD; block != end; block = next(block)) {
    header = get_header(block);
    footer = get_footer(block);
    assert(header == footer);

    block_size = get_size(header);
    assert(block_size >= MIN_BLOCK_SIZE);
    *size += get_size(header);

    if (!is_allocated(header)) {
      ++(*free_block);
    }
    debug_printf("|%p:%zu,%d", block, block_size, is_allocated(header));
  }
  header = *(word_t *)(end - WORD);
  debug_printf("|%p:%zu,%d\n", end, get_size(header), is_allocated(header));
  assert((end - begin - 8) == *size);
}

void check_segregated_lists(size_t *free_node) {
  word_t header = 0;
  size_t block_size = 0;
  *free_node = 0;
  list_node_t *head = NULL;
  int i = 0;
  // 前面的链表节点都必须在 small region里
  for (; i < SEGREGATED_LIST_SIZE - 1; ++i) {
    head = &segregated_lists[i];
    for (list_node_t *work = head->next; work != head; work = work->next) {
      header = get_header((byte_t *)work);
      block_size = get_size(header);
      assert(!is_allocated(header));
      // assert(is_small(work));
      assert(size_class(block_size) == i);
      ++(*free_node);
    }
  }

  // 遍历最后一个链表 所有的节点都必须large region里面
  head = &segregated_lists[i];
  for (list_node_t *work = head->next; work != head; work = work->next) {
    header = get_header((byte_t *)work);
    block_size = get_size(header);
    assert(!is_allocated(header));
    assert(is_large(work));
    ++(*free_node);
  }
}

void check_small() {
  // 遍历small region 不能有某个块的大小 超过规定的大小
  byte_t *begin = small_begin();
  byte_t *end = small_end();
  for (byte_t *block = begin + DWORD; block != end; block = next(block)) {
    assert(get_size(get_header(block)) <= MAX_LINEAR_SIZE);
  }
}

// must write a mm_check
void mm_check(void) {
  if (!CHECK_FLAG) {
    return;
  }

  // 现在check分成三部分
  // 第一部分 check small region
  size_t small_size = 0;
  size_t small_free_block = 0;
  check_implicit_list(small_begin(), small_end(), &small_size,
                      &small_free_block);
  check_small();

  // 第二部分 check large region
  size_t large_size = 0;
  size_t large_free_block = 0;
  check_implicit_list(large_begin(), large_end(0), &large_size,
                      &large_free_block);
  check_contiguous_free(large_begin(), large_end());

  size_t check_heap_size =
      small_size + large_size +                    // small region, large region
      SEGREGATED_LIST_SIZE * sizeof(list_node_t) + // segregated region
      4 * WORD;                                    // 4 begin and end block
  assert(check_heap_size == heap_size);

  // 第三部分 check segregated region 及其链表 和
  // 我们前面的一些统计数据是否一致
  size_t free_node = 0;
  check_segregated_lists(&free_node);
  assert(small_free_block + large_free_block == free_node);
}

// 确定一条原则，所有的size参数表示的都是整个block的大小
// todo 根据size找到合适的list然后插入
static inline void segregated_lists_insert(list_node_t *node, size_t size) {
  // 除非这里的size错了
  size_t block_size = get_size(get_header((byte_t *)node));
  assert(size == block_size);
  // 我们这需要根据size和small
  // 这里再改一下 还是改成根据size class 来插入 只不过coalease的时候
  // 只能根据region决定是否可以coalease
  list_insert_after(&segregated_lists[size_class(size)], node);
  // if (is_small(node)) {
  //   list_insert_after(&segregated_lists[size_class(size)], node);
  // } else {
  //   list_insert_after(&segregated_lists[SEGREGATED_LIST_SIZE - 1], node);
  // }
}

// coalease block 一共有四种情况
static byte_t *coalease(byte_t *block_ptr) {

  // 如果我们在 small_region  直接退出就好了
  if (is_small((list_node_t *)block_ptr)) {
    return block_ptr;
  }

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
    // 因为我们自身的size也变了 所以我们自身也要从size中删除 然后再添加回去
  }
  // case3
  else if (!is_prev_allocated && is_next_allocated) {

    // 我们需要判断上一个是否在小内存区
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
  // 我们不能简单的直接改header的size了 因为现在
  // size的大小会影响他在那个链表里面
  list_erase((list_node_t *)block_ptr);
  set_header(block_ptr, size);
  set_footer(block_ptr, size);
  // todo 添加
  segregated_lists_insert((list_node_t *)block_ptr, size);

  return block_ptr;
}

static byte_t *extend_heap(size_t size) {
  // 我们扩展的大小必须也是对齐的，八位对齐，也就是分配偶数个word就行
  // 先不管了
  // 就假设对齐了吧

  word_t *brk = mem_sbrk(size);
  heap_size += size;
  // set header
  *(brk - 1) = size;
  // set footer
  set_footer((byte_t *)brk, size);
  // set epilogue block
  *(footer_ptr((byte_t *)brk) + 1) = 0 | 1;

  // 如果是扩展的部分 那么必然是插入到最后一个链表的
  // 不过这样自己选择也是对的 因为扩展heap的时候 size至少是 CHUNCK_SIZE
  // insert this new block into our explicit free list
  list_insert_after(get_list_head(size), (list_node_t *)brk);

  // 我们扩展一个heap 相当于在最后一个位置插入了一个块
  // 所以 如果前一个块是free的状态 还需要coalease
  // extend heap 放弃coalease
  return coalease((byte_t *)brk);
}

// 写一个测试segregated storage的程序 现在看起来没有写对啊
void test_segregated_storage() {
  // 那首先我们遍历整个链表数组
  // 给定所有的size 看一下映射是不是对的
  for (size_t i = 16; i <= 4096; i += 8) {
    size_t index = size_class(i);
    debug_printf("size: %d, size class: %d\n", i, index);
  }
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  // 原来如此 这个程序会被调用多次 计算平均值 原来如此
  word_t *brk = mem_sbrk(SMALL_SIZE);
  heap_size = SMALL_SIZE;

  heap_ptr = brk;
  segregated_lists = (list_node_t *)heap_ptr;
  for (size_t i = 0; i < SEGREGATED_LIST_SIZE; ++i) {
    list_init_head(&segregated_lists[i]);
  }

  brk += SEGREGATED_LIST_SIZE * 2;
  // begin block
  *brk = 0 | 1;

  // 在init的时候，我们就把这一大块给切开，而且在coaleasing代码里面对这一块进行保护
  // 那么我们就不会coalease了
  // 这块地址就永远用于小内存
  // 在最开始的时候 切成小内存块最大的内存块就可以了 然后插入到对应的链表中

  // 中间是一个大的payload 需要放置在合适的list里面
  // 那我们首先计算其大小
  size_t size =
      SMALL_SIZE - WORD - WORD - SEGREGATED_LIST_SIZE * sizeof(list_node_t);
  brk += 2;

  byte_t *block_ptr = (byte_t *)brk;
  while (size >= MAX_LINEAR_SIZE) {
    set_header((byte_t *)block_ptr, MAX_LINEAR_SIZE);
    set_footer((byte_t *)block_ptr, MAX_LINEAR_SIZE);
    segregated_lists_insert((list_node_t *)block_ptr, MAX_LINEAR_SIZE);
    size -= MAX_LINEAR_SIZE;
    block_ptr = next((byte_t *)block_ptr);
  }

  // 然后在加上最后一块
  // 不过最后一块可能会小于 MINIMUN_SIZE 啊
  // 不过应该不会 我们可以assert一下
  assert(size >= MIN_BLOCK_SIZE);
  set_header(block_ptr, size);
  set_footer(block_ptr, size);
  segregated_lists_insert((list_node_t *)block_ptr, size);

  brk = footer_ptr((byte_t *)block_ptr);
  // 插入到合适的位置 给定一个指针block 插入到合适的位置 可以写成一个函数

  // end block
  brk++;
  *brk = 0 | 1;

  // 我们在这里检查一下
  size_t small_size = 0;
  size_t small_free_block = 0;
  check_implicit_list(small_begin(), small_end(), &small_size,
                      &small_free_block);

  // 我们在这里设置一道屏障，也就是再申请一个chunk_size的内存，然后作为最后一个list的内容
  // 同时也提供begin和end块
  // 那么最好使用一个全局变量记录 emm 也不用 其实就是 heap_ptr + SMALL_SIZE
  brk = mem_sbrk(CHUNCK_SIZE);
  heap_size += CHUNCK_SIZE;
  // begin block
  *brk = 0 | 1;
  brk += 2;
  set_header((byte_t *)brk, CHUNCK_SIZE - DWORD);
  set_footer((byte_t *)brk, CHUNCK_SIZE - DWORD);
  segregated_lists_insert((list_node_t *)brk, CHUNCK_SIZE - DWORD);
  brk = footer_ptr((byte_t *)brk);

  brk++;
  // end block
  *brk = 0 | 1;

  mm_check();
  return 0;
}

// 因为我们是32位的，所以刚好可以放在一个word里面

byte_t *first_fit(list_node_t *head, size_t size) {
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

void coalease_heap(byte_t *begin, byte_t *end) {
  // 拿到begin block 和 end block的地址
  byte_t *block_ptr = begin + DWORD;
  word_t header = 0;
  size_t block_size = 0;
  size_t size = 0;
  byte_t *coalease_ptr = NULL;

  // 如果在小内存区域里面 我们就不进行coaleas
  block_ptr = (byte_t *)heap_ptr + CHUNCK_SIZE;

  while (block_ptr != end) {
    size = 0;
    // 首先找到第一个free的block
    for (; block_ptr != end; block_ptr = next(block_ptr)) {
      header = get_header(block_ptr);
      if (!is_allocated(header)) {
        break;
      }
    }

    coalease_ptr = block_ptr;

    // 接下来我们就要找到第一个allocated的block
    for (; block_ptr != end; block_ptr = next(block_ptr)) {
      header = get_header(block_ptr);
      if (is_allocated(header)) {
        break;
      }
      block_size = get_size(header);
      size += block_size;
      list_erase((list_node_t *)block_ptr);
    }

    if (size != 0) {
      set_header(coalease_ptr, size);
      set_footer(coalease_ptr, size);
      segregated_lists_insert((list_node_t *)coalease_ptr, size);
    }
  }

  // 我们在这里进行一个check
  if (CHECK_FLAG) {
    size = CHUNCK_SIZE;
    // 从begin block 遍历到 end block
    // 1. 不能又连续free
    // 2. 所有block大小必须一样
    bool prev_allocated = true;
    for (block_ptr = (byte_t *)heap_ptr + CHUNCK_SIZE; block_ptr != end;
         block_ptr = next(block_ptr)) {
      header = get_header(block_ptr);
      size += get_size(header);

      assert(!(!prev_allocated && !is_allocated(header)));
      prev_allocated = is_allocated(header);
    }
    assert(size == heap_size);
    mm_check();
  }
}

byte_t *find_block(size_t size) {
  byte_t *block_ptr = NULL;
  for (size_t i = size_class(size); i < SEGREGATED_LIST_SIZE; ++i) {
    block_ptr = first_fit(&segregated_lists[i], size);
    if (block_ptr) {
      return block_ptr;
    }
  }
  // 如果还是找不到 只能extend heap
  return extend_heap(umax(size, CHUNCK_SIZE));
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
  segregated_lists_insert((list_node_t *)block_ptr, curr_size - size);
  // list_insert_after(get_list_head(), (list_node_t *)block_ptr);
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
  debug_printf("malloc %zu: \n", size);

  // 第三步，需要遍历整个链表，使用firstfir方法，找到一个大小能够容纳size的地址
  // size需要加上header和footer的大小
  //
  byte_t *block_ptr = find_block(size);
  // 第四步如果没有找到，分配一个
  // if (!block_ptr) {
  //   block_ptr = extend_heap(umax(size, CHUNCK_SIZE));
  // }

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

  mm_check();

  return block_ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
  if (ptr) {
    // 现在free需要把节点添加到explicit free list链表头
    size_t block_size = get_size(get_header((byte_t *)ptr));
    set_header(ptr, block_size);
    set_footer(ptr, block_size);
    // list_insert_after(get_list_head(), (list_node_t *)ptr);
    segregated_lists_insert((list_node_t *)ptr, block_size);

    // 我们根据大小进行coalease 只有超过一定大小的才会在free的时候执行coalease
    if (is_large(ptr)) {
      coalease(ptr);
    }
    // 如果太小 直接标记成free就可以了
  }

  debug_printf("free %p: \n", ptr);
  mm_check();
}

bool is_last_block(byte_t *block_ptr) { return next(block_ptr) == large_end(); }

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {

  static int case1 = 0;
  static int case2 = 0;
  static int case3 = 0;
  static int case4 = 0;
  static int case5 = 0;
  static int case6 = 0;

  debug_printf("realloc %p: \n", ptr);
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
    case1++;
    split(ptr, size);
    newptr = ptr;
  } else if (size <= block_size) {
    case2++;
  } else {
    // size > block_size
    // 我们需要判断我们是不是在 small_region 并且最终的大小超过了限制
    // 那么只能调用malloc了
    if (is_small(ptr) && size > MAX_LINEAR_SIZE) {
      // 在这种情况下，我们直接调用malloc 即可
      newptr = mm_malloc(initial_size);
      if (!newptr) {
        return NULL;
      }

      copySize = block_size - DWORD;
      memcpy(newptr, oldptr, copySize);
      mm_free(oldptr);
      return newptr;
    }

    // 还有一种情况 就是前面的一个block也是free的 这样我们就可以利用前面的block
    // 从而不用再申请内存了

    // todo
    // 其实还有一种情况不需要复制，就是我们的下一个块是free的，如果加上这个块刚好够的话
    // 就不用动了 就直接合并成一个 然后做一个split就可以了
    byte_t *next_block = next(ptr);
    word_t next_header = get_header(next_block);
    byte_t *prev_block = prev(ptr);
    word_t prev_footer = *(word_t *)(ptr - DWORD);

    if (!is_allocated(next_header) &&
        block_size + get_size(next_header) >= size) {
      case3++;
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
    }

    else if (is_last_block(ptr)) {
      case4++;
      // 我想统计一下一共会触发多少次
      static int last_block_trigger_count = 0;
      debug_printf("last block: %d\n", ++last_block_trigger_count);

      // 那么我们同样也是不需要拷贝内存的 只需要在后面续上一块内存就行了
      // 目前的大小 block_size
      // 需要的大小 size
      // 那么我们需要扩展一下heap 每次扩展至少一个page
      assert(size > block_size);
      mem_sbrk(size - block_size);
      heap_size += size - block_size;
      // 这里不用删除 因为我们是allocated的 没有在free list里面
      // list_erase(ptr);
      set_header(ptr, size | 1);
      set_footer(ptr, size | 1);
      // 同时也不需要加 因为 我们没有被free
      // segregated_lists_insert(ptr, size);
      // set end block
      set_header(next(ptr), 1);
    }

    else if (!is_allocated(prev_footer) &&
             block_size + get_size(prev_footer) >= size) {
      case5++;
      // 在这种情况下 我们也需要合并

      list_erase((list_node_t *)prev_block);
      newSize = (block_size + get_size(prev_footer));
      set_header(prev_block, newSize | 1);
      set_footer(prev_block, newSize | 1);
      // memcpy(prev_block, ptr, block_size - DWORD);
      for (size_t i = 0; i < block_size - DWORD; ++i) {
        prev_block[i] = *((byte_t *)(ptr) + i);
      }

      newptr = prev_block;
      // 然后需要执行一次split
      if (newSize - size >= MIN_BLOCK_SIZE) {
        split(prev_block, size);
        // 只有split之后 才需要执行一个coalease
        // 我们的下一个可能是free的 所以可能需要做一次coaleas
        coalease(next(newptr));
      }
    }

    else {
      case6++;
      // todo
      // 我又想到了一种情况，可以减少内存的分配
      // 就是尽可能的检测上方和下方的free block 如果
      // 如果都是free 并且大小都是够的 那么我们也可以不用申请新的内存
      // 如果我们不是last block 但是我们是倒数第二个 同时last block是free的
      // 那么我们也可以只申请一小部分内存

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

  mm_check();

  // printf("case1: %d, case2: %d, case3： %d, case4: %d, case5: %d, case6:
  // %d\n",
  //        case1, case2, case3, case4, case5, case6);

  return newptr;
}
