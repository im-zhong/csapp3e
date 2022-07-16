#include "cachelab.h"
#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// 大体的逻辑都理顺了，今天晚上应该就能写完了
// set args -v -s 4 -E 1 -b 4 -t traces/yi.trace

void help() {
  const char *msg =
      "Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
      "Options:\n"
      "  -h         Print this help message.\n"
      "  -v         Optional verbose flag.\n"
      "  -s <num>   Number of set index bits.\n"
      "  -E <num>   Number of lines per set.\n"
      "  -b <num>   Number of block offset bits.\n"
      "  -t <file>  Trace file.\n"
      "\n"
      "Examples:\n"
      "linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
      "linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n";
  printf("%s", msg);
}

typedef unsigned long line_t;

#define VIRTUAL_ADDRESS_BITS (48)

// 根据csapp书上 描述一个cache需要四个参量
typedef struct {
  int s;
  int E;
  int b;
  int m;
  // 我还必须有一个模拟cache的数组
  line_t *lines;
} cache_t;

typedef unsigned long ptr_t;

// va: virtual address
unsigned long get_tag(cache_t *cache, ptr_t va) {
  // tag直接移位就可以了
  return va >> (cache->b + cache->s);
}
unsigned long get_index(cache_t *cache, ptr_t va) {
  // 先移位 然后只要低 s 位
  // 如果s = 0 结果就是零 也是正确的
  return (va >> cache->b) & ((1 << cache->s) - 1);
}
unsigned long get_offset(cache_t *cache, ptr_t va) {
  // 只要低b位
  return va & ((1 << cache->b) - 1);
}

bool is_valid(line_t line) {
  // 最高位是1 valid
  return (long)line < 0;
}

unsigned long get_line_tag(line_t line) {
  // 只需要把高位清零即可
  return LONG_MAX & line;
}

// LRU
// 给定一个set为首地址，n个cache line的数组，line表示新的读取数据，需要放到cache
// set首地址处 p表示这个line之前在哪里，如果是新加入的，p应该等于n-1
// 也就是放到最后一个位置 这样最后一个元素就可以被覆盖掉了 可以
// 所以这个算法的速度就O(n)的 好像还真是不如链表啊 不过这样比较实现简单
void replace(line_t *set, size_t n, line_t line, size_t p) {
  // 这样我们就只需要从后向前遍历，把数据后移即可，
  // 然后把line放到首地址处
  assert(p < n);
  for (; p > 0; --p) {
    set[p] = set[p - 1];
  }
  set[0] = line;
}

// 不对 还需要统计eviction
// cache hit:
// cache miss:
// cache eviction: if cache miss, it needs to retrieve the requested block from
// the next level in the memory hierarchy. if the set is full of valid cache
// lines, then one of the existing lines must be evicted
// 就用低三位分别表示是否hit miss 和 eviction 就好了
// 最低位表示是否hit
// 然后是是否miss
// 然后是是否eviction
int load(cache_t *cache, ptr_t va) {
  if (!cache) {
    exit(EXIT_FAILURE);
  }

  // cache line: [valid bit|tag]
  // ptr_t tag = (va << 1) >> 1;
  // int valid = va >> 63;

  unsigned long index = get_index(cache, va);

  // 我们需要通过index找到对应的set
  // 然后在set内部对比E个cache line的valid bit和tag 如果有匹配的 就hit
  // 如果没有匹配的就miss；如果miss，需要使用LRU算法的replacement
  // policy来替换cache line中的内容

  unsigned long tag = get_tag(cache, va);

  // 最关键的是LRU策略如何实现
  // 最普通的办法应该是使用双向链表实现 但是我们有很多的cache
  // 都用双向链表实现太慢了
  // 如果用数组实现，每次读取和写入都需要移动数组元素

  // 我怎么表示一个cache set呢
  // 首先找到这个cache set的首地址
  // size_t set = index * cache->E;
  // 我这里直接把set的首地址给拿出来不香吗
  line_t *set_ptr = cache->lines + index * cache->E;

  // 遍历整个cache set
  // 在这个过程中我就可以直到究竟是 miss 还是 hit
  // 如果miss 究竟需不需要eviction
  bool hit = false;
  size_t i = 0;
  for (; i < cache->E; i++) {
    // 遍历检查是否命中，
    // 如果出现invalid的部分直接退出即可
    bool valid = false;
    valid = is_valid(set_ptr[i]);
    if (!valid) {
      break;
    }
    // check if tag is the same
    if (tag == get_line_tag(set_ptr[i])) {
      hit = true;
      break;
    }
  }

  bool evict = false;
  line_t line = 0;
  // 如果hit我需要直到hit的位置, i就表示hit的位置
  if (hit) {
    // 如果hit我们需要把hit的元素提到数组首部，其余元素后撤
    // 这些个逻辑写起来真的复杂 真不如用链表
    line = set_ptr[i];
  } else {
    // 如果没有hit我们需要创建一个新的cache line 放到首部,
    // i就是有数据的部分的下一个位置，但是这个位置可能越界 所以需要判断一下
    if (i == cache->E) {
      evict = true;
      i = cache->E - 1;
    }
    // valid_bit & tag
    line = LONG_MIN | tag;
  }

  // 我需要实现一个replacement函数，用来实现LRU策略，这样读写都可以使用
  replace(set_ptr, cache->E, line, i);

  int r = 0;
  if (hit) {
    r |= 1;
  } else {
    r |= 2;
  }
  if (evict) {
    r |= 4;
  }
  return r;
}

// store和load一样啊
int store(cache_t *cache, ptr_t va) { return load(cache, va); }
// modify = load + store
int modify(cache_t *cache, ptr_t va) {
  int l = load(cache, va);
  store(cache, va);

  // modify两次都可能是hit
  // 需要特殊的处理
  // 因为modify的第二次store一定hit
  // 所以我们可以让结果只保存第一次是否hit
  // 然后在输出的时候添加一块特殊的逻辑，如果是modify就再补上一次hit就可以了
  return l;
}

void init_cache(cache_t *cache, int s, int E, int b, int m) {
  if (cache) {
    cache->s = s;
    cache->E = E;
    cache->b = b;
    cache->m = m;
    size_t size = (1 << s) * E * sizeof(line_t);
    cache->lines = malloc(size);
    if (!cache->lines) {
      exit(EXIT_FAILURE);
    }
    // 我还要把所有的数值都初始化位零
    memset(cache->lines, 0, size);
  }
}

void free_cache(cache_t *cache) {
  if (cache && cache->lines) {
    free(cache->lines);
  }
}

// 需要返回三个参数，整个模拟过程的hit数量，miss数量和eviction数量
// 并且需要根据verbose参数来确定是否输出详细的过程
typedef struct {
  int hits;
  int misses;
  int evictions;
} simulate_result_t;

// 得到的就是动作和地址

void simulate(cache_t *cache, const char *trace_file, bool verbose,
              simulate_result_t *result) {
  // 首先要打开文件
  FILE *fin = fopen(trace_file, "r");
  char line[64] = {'\0'};
  // size_t size = 0;
  // size_t nread = 0;

  result->evictions = 0;
  result->misses = 0;
  result->hits = 0;

  // getline C99 用不了
  // 只能用 fgets
  // while ((nread = getline(&line, &size, fin)) != -1) {
  while (fgets(line, sizeof(line), fin)) {
    // [0:nread-1]
    // parse the line
    // 分解出动作和 地址
    if (line[0] == 'I') {
      continue;
    }
    char op = '\0';
    unsigned long va = 0;
    int len = 0;
    sscanf(line, " %c %lx,%d\n", &op, &va, &len);

    int r = 0;
    switch (op) {
    case 'L':
      r = load(cache, va);
      break;
    case 'S':
      r = store(cache, va);
      break;
    case 'M':
      r = modify(cache, va);
      break;
    default:
      // 到这里就说明有bug
      exit(EXIT_FAILURE);
    }

    // 我们就可以分析r
    int hit = (r & 0x1);
    int miss = (r & 2) >> 1;
    int eviction = (r & 4) >> 2;
    result->hits += hit;
    result->misses += miss;
    result->evictions += eviction;

    if (op == 'M') {
      result->hits += 1;
    }

    if (verbose) {
      // 我们需要一些额外的输出
      // 就是之前的line 在加上miss 或者hit 等等动作
      // 需要修改一个line的最后一个位置
      // 不要输出回车
      // 而且不出书开头的空格
      // 然后是动作 miss hit eviction
      line[strlen(line) - 1] = '\0';
      printf("%s%s%s%s%s\n", line + 1, miss ? " miss" : "",
             eviction ? " eviction" : "", hit ? " hit" : "",
             op == 'M' ? " hit" : "");
    }
  }

  fclose(fin);
}

int main(int argc, char *argv[]) {

  // // 输入一些参数
  int s = 0;
  int E = 0;
  int b = 0;
  bool verbose = false;
  const char *trace_file = NULL;

  int opt = 0;
  while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
    switch (opt) {
    case 'h':
      help();
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      verbose = true;
      break;
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      trace_file = optarg;
      break;
    default:
      fprintf(stderr, "Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  cache_t cache;
  init_cache(&cache, s, E, b, VIRTUAL_ADDRESS_BITS);
  // 接下来我就需要模拟cache过程了
  simulate_result_t result;
  simulate(&cache, trace_file, verbose, &result);
  free_cache(&cache);
  printSummary(result.hits, result.misses, result.evictions);
  return 0;
}
