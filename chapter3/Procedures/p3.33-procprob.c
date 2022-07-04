// 2022/7/4
// zhangzhong
// Practice Problem

// Actually there are lots of posibilities
unsigned procprob(int a, short b, long *u, char *v) {
  *u += a;
  *v += b;
  return sizeof(a) + sizeof(b);
}

unsigned procprob1(int a, unsigned short b, unsigned long *u,
                   unsigned char *v) {
  *u += a;
  *v += b;
  return sizeof(a) + sizeof(b);
}
