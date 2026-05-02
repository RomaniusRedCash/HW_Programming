#!/bin/bash
NAME=$(basename "$0")
PARAM="--bwt --rle"
echo $PARAM
./exe $PARAM -i $1 -o $1.${NAME%.*}.cmpr
