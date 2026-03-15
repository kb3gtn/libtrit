#include "libtrit.h"

// Pre-Computed table for sum of powers of 3 up to an exponent.
// table of sum of power of three..
const int64_t POW3_SUM_TABLE[24] = {
  1,
  4,
  13,
  40,
  121,
  364,
  1093,
  3280,
  9841,
  29524,
  88573,
  265720,
  797161,
  2391484,
  7174453,
  21523360,
  64570081,
  193710244,
  581130733,
  1743392200,
  5230176601,
  15690529804,
  47071589413,
  141214768240,
};

// Pre-Computed table for power of 3 up to an exponent 
// table of power of three..
const int64_t POW3_TABLE[24] = {
  1,
  3,
  9,
  27,
  81,
  243,
  729,
  2187,
  6561,
  19683,
  59049,
  177147,
  531441,
  1594323,
  4782969,
  14348907,
  43046721,
  129140163,
  387420489,
  1162261467,
  3486784401,
  10460353203,
  31381059609,
  94143178827,
};

// Pre-Computed table of floor(POW3/2) values 
// used as sum/sub decision threshold
const int64_t POW3_DIV2_TABLE[24] = {
  0,
  1,
  4,
  13,
  40,
  121,
  364,
  1093,
  3280,
  9841,
  29524,
  88573,
  265720,
  797161,
  2391484,
  7174453,
  21523360,
  64570081,
  193710244,
  581130733,
  1743392200,
  5230176601,
  15690529804,
  47071589413,
};



////////////////////////////
// construct an empty trit array object
// register is initialized to all '0'
TRIT_ARRAY ta_construct() {
  TRIT_ARRAY ta;
  // initialize array to all zeros
  for (int idx=0; idx < 24; ++idx) {
    ta.reg[idx] = '0';
  }
  return ta;
}

////////////////////////////
// Trits_len_for ... 
// return the number of trits needed to represent the 
// provided value.. 
int trits_len_for(int64_t value) {
  if (value < 0) {
    // make value positive
    value = value*-1; // could be a invert and add 1
  }
  int idx=0;
  for (idx; idx < 24; ++idx) {
    if (POW3_SUM_TABLE[idx] >= value) {
      return idx;
    }
  }
  // not representable in 24 trits
  return -1;
}

//////////////////////////////
// Find Pow3 Sum that fits 
// the value provided (same as trits_len_for)
int find_pow3_sum_that_fits(int64_t v) {
  return trits_len_for(v);
}

/////////////////////////////
// Simple Abosolute power 
// foce to positive value..
int64_t simple_abs(int64_t v) {
  if ( v < 0 ) {
    return v*-1;
  } else {
    return v;
  }
}

//////////////////////////////
// Value to Trits
TRIT_ARRAY to_trits(int64_t value) {
  // most significant trit
  int mst = trits_len_for(value);
  //printf("trits needed for %lld is %d\n", value, mst);
  TRIT_ARRAY ta = ta_construct();
  int is_negitive=0;
  if ( value < 0 ) {
    is_negitive = 1;
    // make positive, invert anwser in BTL to get negitave.
    value = value * -1;
  }

  int64_t acc = value;
  int idx = mst;
  while( idx > 0 ) {
    int u = find_pow3_sum_that_fits(simple_abs(acc))-1;
    uint64_t p3m1 = POW3_TABLE[idx-1];
    if (simple_abs(acc) > POW3_DIV2_TABLE[idx]) {
      if (acc > 0) {
        acc = acc - POW3_TABLE[idx];
        ta.reg[idx] = TTRUE;
      } else {
        acc = acc + POW3_TABLE[idx];
        ta.reg[idx] = TFALSE;
      }
    } else {
      ta.reg[idx] = TNONE;
    }
    --idx;
  }


  if ( acc == -1 ) {
    ta.reg[0] = TFALSE;
  } else {
    if ( acc == 1 ) {
      ta.reg[0] = TTRUE;
    } else {
      // not -1 or +1 left over.. 
      ta.reg[0] = TNONE;
    }
  }
  
  if (is_negitive) {
    // invert balanced ternary will invert sign of value stored
    trit_logical_neg(&ta);
  }
  return ta;
}


/////////////////////////////////
// Print Trit Register in TA 
void print_TA(TRIT_ARRAY *ta) {
  printf(VT100_COLOR_RESET);
  printf("    ");
  for (int i=23; i >= 0; --i) {
    printf(" %2d ", i);
    if ((i%6) == 0) {
      printf("  ");
    }
  }
  printf("\n    ");
  for (int i=0; i < 24; ++i) {
    printf("+---");
    if ( ((i%6) == 5) && (i < 23)) {
      printf("+ ");
    }
  }
  printf("+\n    ");
  for (int i=23; i >= 0; --i) {
    printf("| ");
    if (ta->reg[i] == TTRUE) {
      printf(VT100_COLOR_TTRUE "+" VT100_COLOR_RESET);
    } else {
      if ( ta->reg[i] == TFALSE ) {
        printf(VT100_COLOR_TFALSE "-" VT100_COLOR_RESET);
      } else {
        printf(VT100_COLOR_TNONE "0" VT100_COLOR_RESET);
      }
    }
    printf(" ");
    if ( ((i%6) == 0) && (i > 0) ) {
      printf("| ");
    }
  }
  int64_t v = trits_to_int(ta);
  printf("| => %lldd\n    ", v);
  printf(VT100_COLOR_RESET);
  for (int i=0; i < 24; ++i) {
    printf("+---");
    if ( ((i%6) == 5) && (i < 23)) {
      printf("+ ");
    }
  }
  printf("+\n");
}

///////////////////////////////
// Convert trit array back into 
// a int64_t value. 
int64_t trits_to_int(TRIT_ARRAY *ta) {
  int64_t sum=0;
  for (int i=0; i < 24; ++i) {
    if (ta->reg[i] == TTRUE) {
      sum = sum + POW3_TABLE[i];
    } else {
      if ( ta->reg[i] == TFALSE ) {
        sum = sum - POW3_TABLE[i];
      } else {
        // no change (should get optimized out)
        sum = sum;
      }
    }
  }
  return sum;
}

////////////////////////////////
// Trit Logical Negation (NEG)
// True => False, False => True, None => None
void trit_logical_neg(TRIT_ARRAY *ta) {
  for (int i=0; i < 24; ++i) {
    if (ta->reg[i] == TTRUE ) {
      ta->reg[i] = TFALSE;
    } else {
      if (ta->reg[i] == TFALSE ) {
        ta->reg[i] = TTRUE;
      }
      // None, do nothing..
    }
  }
}

size_t safe_strlen(const char *str, size_t max_len)
{
  size_t l=0;
  for (int l=0; l < max_len; ++l) {
    if (str[l] == 0) {
      return l-1;
    }
  }
  // if got here, did not find a null.
  return 0;
}

///////////////////////////////////////
// Set a Trit Array to a trit value 
// trit string is a string of +,-,0 starting with 
// the Most Significant Trit first
RESULT set_TA(TRIT_ARRAY *ta, const char* trit_string_mstf) {
  int sl = safe_strlen(trit_string_mstf, 24);
  if (sl < 1) {
    return Err; 
  }
  int si = sl;
  for (int i=0; i <= sl; ++i) {
    char t = trit_string_mstf[si];
    if (!((t == '+') || (t == '-') || (t == '0'))) {
      return Err;
    }
    --si;
  }
  
  si = sl;
  for (int i=0; i <= sl; ++i) {
    ta->reg[i] = trit_string_mstf[si];
    --si;
  }
  return Ok;
}

