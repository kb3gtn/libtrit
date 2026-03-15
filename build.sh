#!/bin/bash

if [[ "$1" == "clean" ]]; then 
  echo "Cleaning up builds file.."
  rm -f trit_calc 
  rm -f ./libtrit/libtrit.so
  exit 0
fi

cd ./libtrit
./build.sh
cd ..
gcc -I./libtrit trit_calc.c -o trit_calc -L./libtrit -ltrit
