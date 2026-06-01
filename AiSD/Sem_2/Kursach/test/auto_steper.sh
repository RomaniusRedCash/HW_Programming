#!/bin/bash
EXE=./RRT
TMP=tmp.text
for MAP in $(seq 0 3); do
    for RRT in "rrt" "fastrrt"; do
        cat test_map/ai_map${MAP}.rrt > $TMP
        for ((i = 5; i < 50; i++)); do
            echo "$RRT step $i go 100000"
        done >> $TMP
        SAFE_RRT=${RRT/\*/z}
        cat $TMP | $EXE | awk -v step=1 '/Spent time:/ {count++; print (count * step) " " $3}' > "data/${SAFE_RRT}_ai${MAP}.dat"
    done
done
