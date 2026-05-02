#!/bin/bash
NAME=$(basename "$0")
PARAM="--lzw --ha"
echo $PARAM
./exe $PARAM -i $1 -o $1.${NAME%.*}.cmpr
