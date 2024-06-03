#!/bin/bash

CC="gcc"
CFLAGS="-Wall"
SRC_DIR="src"
INC_DIR=$SRC_DIR"/include"
OBJ_DIR="obj"

mkdir -p $OBJ_DIR

for C_FILE in $SRC_DIR/*.c; do
	$CC -c $C_FILE -I $INC_DIR -o "$OBJ_DIR/$(basename $C_FILE .c).o" $CFLAGS
done

$CC $OBJ_DIR/*.o -o main

rm -r $OBJ_DIR