
//////////////////////////////////////////////////
// simple Ternary value library
// Peter Fetterer <kb3gtn@gmail.com>
///////////////////////////////////////////////////

#ifndef __LIBTRIT_H__
#define __LIBTRIT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

// states in balanced ternary logic
#define TTRUE '+'
#define TNONE '0'
#define TFALSE '-'

#define VT100_COLOR_TTRUE "[1;32;40m"
#define VT100_COLOR_TFALSE "[1;31;40m"
#define VT100_COLOR_TNONE "[2;37;40m"
#define VT100_COLOR_RESET "[0m"

typedef char TRIT; 

extern const int64_t POW3_SUM_TABLE[24];
extern const int64_t POW3_TABLE[24];
extern const int64_t POW3_DIV2_TABLE[24];

typedef enum {
  Ok = 0,
  Err = -1
} RESULT;

// array of trits with max length of 24 trits. (8 heptavintimal digits)
// covers range +/- 141,214,768,240
typedef struct {
  TRIT reg[24];
} TRIT_ARRAY;

//////////////////////////
/// Prototypes
TRIT_ARRAY ta_construct();
int trits_len_for(int64_t value);
TRIT_ARRAY to_trits(int64_t value);
void print_TA(TRIT_ARRAY *ta);
RESULT set_TA(TRIT_ARRAY *ta, const char* trit_string_mstf);
int64_t trits_to_int(TRIT_ARRAY *ta);
void trit_logical_neg(TRIT_ARRAY *ta);
size_t safe_strlen(const char *str, size_t max_len);
int find_pow3_sum_that_fits(int64_t v);
int64_t simple_abs(int64_t v);


#endif // __LIBTRIT_H__
