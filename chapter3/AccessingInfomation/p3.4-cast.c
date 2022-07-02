// 2022/7/2
// zhangzhong
// Practice Problem 3.4

// sp: source pointer, dp: destination pointer
void long2long(long *sp, long *dp) { *dp = (long)*sp; }

void char2int(char *sp, int *dp) { *dp = (int)*sp; }

void char2unsigned(char *sp, unsigned *dp) { *dp = (unsigned)*sp; }

// why this generation movl otherwise movq?
void uchar2long(unsigned char *sp, unsigned *dp) { *dp = (unsigned)*sp; }

void int2char(int *sp, char *dp) { *dp = (char)*sp; }

void unsigned2uchar(unsigned *sp, unsigned char *dp) {
  *dp = (unsigned char)*sp;
}

void char2short(char *sp, short *dp) { *dp = (short)*sp; }
