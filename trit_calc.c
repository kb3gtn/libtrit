//////////////////////////////////////////////////
// simple Ternary value calculator 
//
// Peter Fetterer <kb3gtn@gmail.com>
//
///////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "libtrit.h"

/////////////////////
// Local Prototypes
void print_help(char *argv[]);
int verify_trit_string(char *tstr);

//////////////////////
///    MAIN 
//////////////////////
int main(int argc, char *argv[]) {
  if (argc < 3) {
    print_help(argv);
    return -1;
  }
  if ( strcmp(argv[1], "enc") == 0) {
    int64_t v = 0;
    if (strcmp(argv[2], "0") == 0 ) {
      v = 0;
    } else {
      // check for number value
      v = strtoll(argv[2], NULL, 10);
      // strtoll returns 0 on parse fail
      if ( v==0 ) {
        printf("number parse failed.\n");
        return -1;
      }
    }
    // do encoding process
    printf("\n");
    TRIT_ARRAY v1 = to_trits(v);
    print_TA(&v1);
    printf("\n");
    return 0;
  }

  if ( strcmp(argv[1], "dec") == 0) {
    int tl = verify_trit_string(argv[2]);
    if ( tl < 0 ) {
      printf("Failed to parse provided trit string\n");
      return -1;
    }
    TRIT_ARRAY v2 = ta_construct();
    RESULT r = set_TA(&v2, argv[2]);
    if ( r == Err ) {
      printf("Failed to initialize trit array using provided value.\n");
      return -1;
    }
    printf("\n");
    print_TA(&v2);
    printf("\n");
    return 0;
  }

  // got here, unknown command 
  printf("unknown command %s %s\n", argv[1], argv[2]);
  print_help(argv);
  return 0;
}

void print_help(char *argv[]) {
  printf("\nTrit Calculator Program\n");
  printf("Usage:\n");
  printf(" %s <cmd> <value>\n", argv[0] );
  printf(" cmd:\n");
  printf("   enc  -- Encode decimal into balanced ternary representation\n");
  printf("           value should be a decimal number\n");
  printf("   dec  -- Decode balanced ternary representation to a decimal value\n");
  printf("           value should be a string containing +,0,- chars\n");
  printf(" Examples:\n\n");
  printf("   %s enc 4096\n", argv[0]);
  printf("   %s dec +00--0++\n", argv[0]);
  printf("\n");
}

int verify_trit_string(char *tstr) {
  int tl = safe_strlen(tstr, 24);
  char *p;
  p = tstr;
  for (int idx=0; idx < tl; ++idx ) {
    if ( (*p != '+') && (*p != '0') && (*p != '-') ) {
      return -1;
    }
  }
  return tl;
}

