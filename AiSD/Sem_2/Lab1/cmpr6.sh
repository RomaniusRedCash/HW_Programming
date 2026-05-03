#!/bin/bash
NAME=$(basename "$0")
PARAM="--lzss=250"
echo $PARAM
./exe $PARAM -i $1 -o $1.${NAME%.*}.cmpr
