//
// f2i: convert IEEE single-precision floating-point to
// 32-bit two's complement integer.
//
// Remember when you implement the function you may not use the
// float or double types. Everything must be done using only integer
// types.

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "f2i.h"

// f2i
//   Parameter: 32-bit value that should be interpreted as an IEEE
//     single-precision floating-point value.
//   Return value: 32-bit two's complement integer value produced by
//     converting the parameter to integer.
//   Note: This routine should duplicate what the Intel 64 hardware
//     does when converting floats to ints. Be sure to consider NaNs,
//     denormalized numbers, infinities, overflow, etc.
int32_t f2i(uint32_t in)
{
  if (check_val(in) == 0x80000000)
    return 0x80000000;
  else if (check_val(in) == 0)
    return 0x00000000;
  else if (check_neg(in))
    return -1 * shift_sig(in);
  else 
    return shift_sig(in);
    
}

// returns the stored exponent
static uint32_t get_exp_s(uint32_t input) {return (input & 0x7f800000) >> 23;}

// gets the actual exponent
static int32_t get_exp_a(uint32_t input)
{
  int32_t exp_s = (int32_t) get_exp_s(input);
  return exp_s -127;
}

// returns the significand 
static uint32_t get_sig(uint32_t input)
{
  uint32_t sig = (input & 0x007fffff);
  sig += 8388608;
  return sig;
}

// shifts the significand to remove decimal
static int32_t shift_sig(uint32_t input)
{
  int32_t exp = get_exp_a(input);
  uint32_t sig = get_sig(input);
  if (exp < 23)
    sig >>= 23 - exp;
  else if (exp > 23)
    sig <<= exp - 23;
  return (int32_t) sig;
}

// checks if the input is a valid number
static int32_t check_val(uint32_t input)
{
  // checks if val is NaN
  if ((get_exp_s(input) == 0xff && get_sig(input) != 0))
    return 0x80000000;
  // checks if val is infinity
  else if (get_exp_s(input) == 0xff && get_sig(input) == 0)
    return 0x80000000;
  // checks if val is denormalized
  else if (get_exp_a(input) < 0)
    return 0;
  else if (get_exp_a(input) >= 31)
    return 0x80000000;
  else 
    return input; 
}

// determines if the value is negative or positive
static bool check_neg(uint32_t input)
{
  if ((input & 0x80000000) == 0x80000000)
    return true;
  else 
    return false;
}