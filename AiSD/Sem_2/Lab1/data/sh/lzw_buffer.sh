#!/bin/bash
export EXE=../../exe

run_performance_test() {
    local FILE=$1
    local ORIG_SIZE=$(stat -c%s "$FILE")

    # Буферы для теста
    local BUFFERS=(1024 4096 16384 65536 262144 524288 1048576 2097152)

    echo "=== Performance Analysis for $FILE ==="

    for BUF in "${BUFFERS[@]}"; do
        # Путь для временного сжатого файла
        local OUT="cmpr/perf_test_lzw.compr"

        # Замеряем время выполнения (только сжатие)
        # -f "%e" выводит только секунды (например, 0.45)
        local TIME_ELAPSED=$( /usr/bin/time -f "%e" $EXE -i "$FILE" -o "$OUT" --lzw=d,$BUF 2>&1 >/dev/null )

        local COMPR_SIZE=$(stat -c%s "$OUT")
        local RATIO=$(echo "scale=2; $COMPR_SIZE * 100 / $ORIG_SIZE" | bc)

        echo "Buffer: $BUF | Time: ${TIME_ELAPSED}s | Ratio: ${RATIO}%"

        # Сохраняем в файл для графиков (Buffer Time Ratio)
        echo "$BUF $TIME_ELAPSED $RATIO" >> "perf_${FILE}.txt"
        rm -f "$OUT"
    done
}

mkdir -p cmpr
run_performance_test "enwik7"
run_performance_test "xrEngine.exe"
