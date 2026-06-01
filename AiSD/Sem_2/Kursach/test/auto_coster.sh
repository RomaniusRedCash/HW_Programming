#!/bin/bash
EXE=./RRT
TMP=tmp.text
MAP=$1
# for MAP in $(seq 0 3); do
    for RRT in "fastrrt" "rrt*"; do
        cat test_map/ai_map${MAP}.rrt > $TMP
        for ((i = 5; i < 6; i+=45)); do
            echo "$RRT step $i go 1000000"
        done >> $TMP
        # SAFE_RRT=${RRT/\*/z}
        echo $RRT step map $MAP
        cat $TMP | $EXE # | awk '/Optimal Cost:/ { cost = $4 } /Spent time:/   { count++; print (count * step) " " cost " " $3 }' > "data/${SAFE_RRT}_cost_ai${MAP}.dat"
    done
# done
