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
  // method 1
  // make all 1 into low 8 bits
  // if all odd bits are 1, the x should be 0xAA
  x &= x >> 16;
  x &= x >> 8;
  x &= 0xAA;
  return !(x ^ 0xAA);

  // method 2
  // just make a 32-bits 0xAAAAAAAA mask and check
  // int mask = 0xAA;
  // mask |= mask << 8;
  // mask |= mask << 16;
  // x &= mask;
  // return !(x ^ mask);
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
  // 0x30 - 0x37: 0x00110xxx, so the check the b7-b2
  // 0x38, 0x39 just check directly
  // and do not forget check the high 24 bits is zero or not
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
  // return expr = (expr1(x) & y) | (expr2(x) & z)
  // we want to control the final result of the whole expr to y or z
  // so we need a total two result:
  // if x != 0, expr = (-1 & y) | (0 & z) = y
  // if x == 0, expr = (0 & y) | (-1 & z) = z
  // which means
  // if x != 0, expr1(x) = -1, expr2(x) = 0
  // if x == 0, expr1(x) = 0, expr2(x) = -1
  // then the whole expression (expr1(x) & y) | (expr2(x) & z) will work
  // and finally, I found two expression which does these works
  // expr1(x) = !x - 1
  // expr2(x) = !!x - 1
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
  // be careful of overflow
  int sign_x = x >> 31;
  int sign_y = y >> 31;
  // case 1: x < 0, y >= 0
  int case_1 = sign_x & !sign_y;
  // case 2: x >= 0, y < 0
  int case_2 = (sign_y & !sign_x);
  // case 3: x >= 0 && y >= 0, x < 0 && y < 0
  int case_3 = !((y + ~x + 1) >> 31);
  // if (case_1 || case_3), return true
  // if case_2, return false
  return (!case_2) & (case_1 | case_3);
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
  // if x == 0, return 1
  // if x != 0, return 0
  // if any bit is 1, after these calculation, the lsb will 1
  // otherwise the lsb will be 0
  x |= x >> 16;
  x |= x >> 8;
  x |= x >> 4;
  x |= x >> 2;
  x |= x >> 1;
  // so the just take the lsb and check if it is equal to 1
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
  // binary search the highest 1
  // 第一次二分搜索
  // 对32位的整数二分搜索，high_bits获得其高16位
  // 情况一：若high_bits为零，则继续在低16位搜索，偏移量b16为0
  // 情况二：若high_bits非零，则需要在高16位搜索，我们将高16位右移至低16位，
  // 这样就可以与情况一保持一致，但是记录偏移量b16为16
  // 第二次二分搜索
  // 对低16位进行类似上述的处理过程，但是在高8位和低8位上进行搜索
  // 最终将下一次需要搜索的区间放在低4位上，并记录偏移量b8
  // ...
  // 直到结束，最终x的值为0或1，表示最后1位是否为1
  // 最终还要加上1位，用于表示符号位

  int minus_one = ~1 + 1;
  int high_bits = 0;
  int b16 = 0;
  int b8 = 0;
  int b4 = 0;
  int b2 = 0;
  int b1 = 0;
  int b0 = 0;

  // if x < 0, x = ~x; otherwise x remain the same
  // becase when x is negative, ~x need the same bit as positive, not -x
  x = (x >> 31) ^ x;

  // get half high bits
  high_bits = (x >> 16) & (0xff | 0xff << 8);
  // if high_bits == 0, b16 = 0, x = x >> 0;
  // if high_bits != 0, b16 = 16, x = x >> 16;
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
  // we also need 1 bit to be sign bit
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
  unsigned sign = uf >> 31;
  unsigned exp = uf >> 23 & 0xff;
  unsigned frac = uf & 0x7fffff;

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
    return (sign << 31) | (uf << 1);
  }

  if (exp == 0xff) {
    // the f should be infinite or nan, whatever situation
    // we just return the f is correct
    return uf;
  }

  // normalized
  exp += 1;
  // if exp == 0xff, frac should be zero, because we overflow to infinite
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
  unsigned sign = uf >> 31;
  unsigned exp = (uf >> 23) & 0xff;
  unsigned frac = uf & 0x7fffff;
  int bias = 127; // 2^7 - 1
  unsigned E = 0;

  // f < 1.0f, i should be zero
  if (exp < 127u) {
    return 0;
  } else {
    // exp >= 127
    // f is normalized, M = 1.frac
    frac = 0x800000 | frac;
    E = exp - bias;
    // |uf| = 1.frac * 2^E
    // what we should do now is left or right shift frac
    // to make the '.' on the right side of lsb
    if (E <= 23) {
      // case 1: E <= 23, which means frac should right shift (23 - E) bits
      frac = frac >> (23 - E);
    } else if (E < 31) {
      // case 2: E < 31(23 + 7), which means frac should left shift (E - 23)
      // bits
      frac = frac << (E - 23);
    } else {
      // case 3: f is too large to fit in integer
      // return TMin to indicate infinite
      return 0x80000000;
    }
    // if uf < 0, frac = -frac
    if (sign == 1) {
      frac = -frac;
    }
    return frac;
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
  int bias = 127; // 2^7 - 1

  // the smallest positive number
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
    // [-149, -127] is nonnormalized number: 0.frac * 2^(1-bias)
    exp = 0u;
    // 1 << k = 2^(k - 23)
    // (k - 23) + (1 - bias) = x
    frac = 1 << (x + 149);
  }
  // the biggest normallized number is 1.0 * 2^bias
  // so the max poser is bias
  else if (x < 128) {
    // [-126, 127]
    // normalized number: 1.0 * 2^(e-bias) = 2^x
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
