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
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

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

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

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
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
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
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  return ~(~x&~y); // DeMorgans law to get OR equivalent
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~x&~y) & ~(x&y); // ~(x&y) translates to ~x | ~y. When comparing that and bitOr, it gives bitXor
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1; // This is how you negate in two's complement
}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  int x = 1; // create 0...01
  x = x << 31; // creates 10....0
  return ~x; // negating gives a 0 followed by all ones
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
  x = ~x;
  x = x >> 31; // returns all ones (-1) or all 0s
  return x + 1; // returns 0 or 1 
}
/* 
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int oddBits(void) {
  //170 used due to it representing 10101010
  int x = 170 << 24; // 101010100...0 
  int y = 170 << 16; // 00000000101010100..0
  int z = 170 << 8; // 0...0101010100..0
  return x + y + z + 170; //adding all produces 1010...10
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  return (!x ^ (((x>>31) & (1)) ^ 1)); // all of the importance is in the sign bit
}
/*
 * ezThreeEights - multiplies by 3/8 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/8),
 *   including overflow behavior.
 *   Examples: ezThreeEights(11) = 4 
 *             ezThreeEights(-9) = -3 
 *             ezThreeEights(1073741824) = -134217728 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeEights(int x) {
  int bias;
  x = (x << 1) + x; // *3
  bias = (x>>31) &7; //bias gathered due to division. &7 included to round towards 0
  x = (x+bias)>>3; // /8
  return x;
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  int z = (((x+y)>>31) & 1); //get sign bit of x + y
  x = (x >> 31) & 1; // gets sign bit of x
  y = (y >> 31) & 1; // gets sign bit of y
  return (x^y) | !(z^x); // checks is x and y the same or if there is a difference in the sum sign bit
}
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  
  return ((~x>>31)&x) | ((x>>31)&(~x+1)); //first part returns all zeros (if original x negative) or original bit (if original x positive) and second part returns the opposite.
}
/* 
 * isUppercaseLetter - return 1 if 0x41 <= x <= 0x5A (ASCII codes for
 * characters 'A' to 'Z')
 *   Example: isUppercaseLetter(0x41) = 1.
 *            isUppercaseLetter(0x3a) = 0.
 *            isUppercaseLetter(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isUppercaseLetter(int x) {
  // between 65 and 90 should return 1
  int y = ((x + ~91+1) >> 31) & 1; // x - 91 (checks upper bound) and returns 0 or 1
  int z = ((64 + ~x+1) >> 31) & 1; // 64 - x (checks lower bound) and returns 0 or 1
  return y&z; // checks if x within 65-90 range based off bound returns
}
/*
 * bitParity - returns xor of all bits of x 
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
 // Compares first half to second half and stores the result
  x = x ^ (x>>16); // compares 1st 16 bits to last 16 bits
  x = x ^ (x>>8);  // compares  1st 8 bits to last 8 bits
  x = x ^ (x>>4); // compares first 4 bits to last 4 bits
  x = x ^ (x>>2); // compares first 2 bits to last 2 bits
  x = x ^ (x>>1); // compares first bit to last bit
  return x&1; // returns 1 or zeros based off last bit
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  unsigned exp = (uf >> 23) & 255; //gets last 9 bits and &255 removes sign bit
  if (exp == 255 && (uf << 9 != 0)){ //Return NaN if uf is NaN
    return uf;
  }

 return (uf ^ (1 << 31)); // changes sign bit to a 0
}
/* 
 * float_times_four - Return bit-level equivalent of expression 4*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_times_four(unsigned uf) {
  unsigned sign = uf & (1 << 31); // Sign Bit
  unsigned exp = (uf >> 23) & 255; // Exponent Value
  unsigned frac = uf & 8388607; // Mantissa
  unsigned fracTwoSig = frac >> 21; // Leading 2 frac bits
  unsigned infin = (255 << 23) | sign; // 0111111110..0 (infinity representation)

  if(exp == 255 || (uf << 1) == 0){ // Return NaN +- infinity and +- 0 as themselves
    return uf;
  }
  if((exp + 2) >= 255){ // Check if overflow after *4 (+2 in exp is *4)
    return infin;
  }
  if(exp == 0){ // Denormalized
    if(fracTwoSig >= 2){ // check most significant fraction bit
      return ((uf << 1) + 0x00800000) | sign; // If most signif is 1
    } else{
        return (uf << 2) | sign; // If not 1
    }
  }
  if((uf << 1) < 0xFE000000){ // Normalized
    return (uf + 0x01000000); // add 2 to exp if no overflow
  }
  return infin; // Overflow results in Infinity
}
