#!/bin/bash
NAME=$(basename "$0")
PARAM="--bwt --mtf --rle --ha"
echo $PARAM
./exe $PARAM -i $1 -o $1.${NAME%.*}.cmpr
