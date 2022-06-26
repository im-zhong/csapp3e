/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif
// 1
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // x ^ y = (x & ~y) | (~x & y)
  // x | y = ~(~x & ~y)
  return ~(~(x & ~y) & ~(~x & y));
}
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // TMin = 100...0
  return 1 << 31;
}
// 2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // TMax = 011...1
  // only TMax + 1 = TMin
  // only 0 = -0, TMin = -TMin
  // x ^ y equals to x != y
  // -x = ~x + 1
  x = x + 1;
  return !((!x) | (x ^ (~x + 1)));
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  x &= x >> 16;
  x &= x >> 8;
  x &= 0xAA;
  return !(x ^ 0xAA);
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) { return ~x + 1; }
// 3
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0'
 * to '9') Example: isAsciiDigit(0x35) = 1. isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  return (!(x & ~0xff) & !((x & 0xf8) ^ 0x30)) | !(x ^ 0x38) | !(x ^ 0x39);
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int minus_one = ~1 + 1;
  return ((!x + minus_one) & y) | ((!!x + minus_one) & z);
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // 所以这道题目需要判断溢出
  // 下面这个式子只能在没有溢出的时候使用
  // 或者不不用判断，只需要判断xy异号，直接返回结果就行
  // 同号相减不会导致溢出
  int sign_x = x >> 31;
  int sign_y = y >> 31;
  // case 1: x < 0, y >= 0
  int case_1 = sign_x & !sign_y;
  // case 2: x >= 0, y < 0
  int case_2 = !(sign_y & !sign_x);
  // case 3: x >= 0 && y >= 0, x <= && y < 0
  int case_3 = !((y + ~x + 1) >> 31);
  return case_2 & (case_1 | case_3);
}
// 4
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  x |= x >> 16;
  x |= x >> 8;
  x |= x >> 4;
  x |= x >> 2;
  x |= x >> 1;
  return (x & 1) ^ 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  // suppose x = 0x00ffffff
  // binary search
  // 第一次search完成之后，我们可以获得高16位是否存在1的信息
  // 并且低十六位应该保存搜索结果
  // 这样下次我们就可以在低十六位里面 按八位搜索 并且将结果保存哎低八位

  // int initial_x = x;
  int minus_one = ~1 + 1;
  // int min = 0x80 << 24;

  int b16 = 0;
  int b8 = 0;
  int b4 = 0;
  int b2 = 0;
  int b1 = 0;
  int b0 = 0;
  int high_bits = 0;
  // 下面的正数的搜索逻辑应该是对的了，但是如果是负数，我们就求他的补，刚好1的位置是对的
  // 毕竟她们需要的位数大小是一样的
  // if x < 0, x = ~x
  int sign_x = x >> 31;
  // x = ((~x + 1) & sign_x) | (x & ~sign_x);
  // 这不tama是异或吗?????
  // x = (~x & sign_x) | (x & ~sign_x);
  x = sign_x ^ x;

  // high_bits 还需要 & mask
  high_bits = (x >> 16) & (0xff | 0xff << 8);
  // if high_bits == 0, b16 = 0; x = x >> b16;
  // if high_bits != 0, b16 = 15; x = x >> 16;
  high_bits = !high_bits + minus_one;
  b16 = high_bits & 16;
  x >>= b16;

  high_bits = (x >> 8) & (0xff);
  high_bits = !high_bits + minus_one;
  b8 = high_bits & 8;
  x >>= b8;

  high_bits = (x >> 4) & 0xf;
  high_bits = !high_bits + minus_one;
  b4 = high_bits & 4;
  x >>= b4;

  high_bits = (x >> 2) & 0x3;
  high_bits = !high_bits + minus_one;
  b2 = high_bits & 2;
  x >>= b2;

  high_bits = (x >> 1) & 1;
  high_bits = !high_bits + minus_one;
  b1 = high_bits & 1;
  x >>= b1;

  b0 = x;

  return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
// float
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned f = uf;
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xff;
  unsigned frac = f & 0x7fffff;

  if (exp == 0) {
    // non normalized
    // left shift the whole number, except the sign
    // becase exp must be 00000000
    // so after the shift, the sign must be 0
    // so we use '|' to add the original sign bit
    // think a very interesting case:
    // when f = 0|00000000|1000...0 = 2^-1 * 2^-126
    // 2.0 * f will cause denormaolized to normolized, but how can we deal with
    // it we just right shift the whole frac into the exponent part, which is
    // just right! 2.0 * f = 2^-126 = 0|00000001|0000...0
    return (sign << 31) | (f << 1);
  }

  if (exp == 0xff) {
    // the f should be infinite or nan, whatever situation
    // we just return the f is correct
    return f;
  }

  // normalized
  exp += 1;
  // 这个操作可能溢出哦
  // 如果在计算完成之后，exp == 0xff, frac should be zero
  if (exp == 0xff) {
    frac = 0;
  }
  return (sign << 31) | (exp << 23) | frac;
}
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {

  unsigned f = uf;
  unsigned E = 0;
  unsigned k = 0;
  int i = 0;

  unsigned sign = f >> 31;
  unsigned exp = (f >> 23) & 0xff;
  unsigned frac = f & 0x7fffff;
  // printf("frac = ");
  // show_bytes(frac);

  // f < 1.0f, i should be zero
  unsigned bias = 127;
  if (exp < 127u) {
    return 0;
  } else {
    // exp = 127, frac = 0; then f = 1.0f
    // normalized, M = 1.frac
    frac = 0x800000 | frac;
    // printf("frac = ");
    // show_bytes(frac);
    // exp >= 127
    E = exp - bias;
    if (E <= 23) {
      k = 23 - E;
      frac = frac >> k;
    } else if (E < (23 + 8)) {
      k = E - 23;
      frac = frac << k;
    } else {
      return 0x80000000;
    }

    // frac
    i = frac;
    if (sign == 1) {
      // -i
      i = ~i + 1;
    }
    return i;
  }
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {

  unsigned exp = 0u;
  unsigned frac = 0u;
  unsigned u = 0u;

  // the smallest positive number
  int bias = 127; // 2^7 - 1
  int float_frac = 23;
  // nonnormalized E is 1 - bias = -126
  // and the smallest frac is 2^(-23)
  // so the min power is -(23 + 126) = -149
  if (x < -149) {
    // too small, return 0
    exp = 0u;
    frac = 0u;
  }
  // the smallest normalized number is 1.0 * 2^(1 - bias)
  // so the min power is 1 - 127 = -126
  else if (x < -126) {
    // [-149, -127] is nonnormalized number
    exp = 0u;
    frac = 1 << (float_frac - (1 - bias - x));
  }
  // the biggest normallized number is 1.0 * 2^bias
  // so the max poser is bias
  else if (x < 127 + 1) {
    // [-126, 127]
    // normalized number
    exp = x + bias;
    frac = 0;
  } else {
    // too big, just positive infinite
    // exp is 11111111, becase float has 8-bit exp
    exp = 0xff;
    frac = 0u;
  }
  u = exp << 23 | frac;
  return u;
}
