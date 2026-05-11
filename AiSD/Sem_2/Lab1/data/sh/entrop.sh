#!/bin/bash

# Очистка при прерывании
cleanup() {
    echo -e "\n[!] Interrupted. Killing processes..."
    trap - SIGINT
    kill -9 0
}
trap cleanup SIGINT SIGTERM

export EXE=../../exe
# Вспомогательный python-скрипт для расчета энтропии из stdin
export PY_ENTROPY="import math, sys; data = sys.stdin.buffer.read();
if not data: print(0); sys.exit();
counts = {}; [counts.__setitem__(b, counts.get(b, 0) + 1) for b in data];
entropy = -sum((c/len(data)) * math.log2(c/len(data)) for c in counts.values());
print(f'{entropy:.6f}')"

run_entropy_test() {
    local FILE=$1
    if [ ! -f "$FILE" ]; then echo "File $FILE not found!"; return 1; fi

    local ORIG_SIZE=$(stat -c%s "$FILE")
    # Считаем исходную энтропию
    local BASE_ENTROPY=$(python3 -c "$PY_ENTROPY" < "$FILE")

    echo "Processing Entropy Research: $FILE"
    echo "Original Entropy: $BASE_ENTROPY bits/byte"
    echo "--------------------------------------------------------------------------"
    printf "%-15s | %-15s\n" "BWT Block Size" "Entropy (bits/byte)"
    echo "--------------------------------------------------------------------------"

    # Размеры блоков для исследования
    local BLOCKS=(
    1024 4096 16384 65536 262144 524288 921600
    1048576
    4194304
    )

    for BUF in "${BLOCKS[@]}"; do
        # Мы НЕ сохраняем файл в -o, а выводим в stdout (если прога умеет)
        # Или сжимаем во временный файл и читаем его.
        # Т.к. нам нужна энтропия ПОСЛЕ BWT+MTF, но ДО Huffman (HA),
        # используем флаги --bwt=$BUF --mtf

        # Если твоя прога не умеет в stdout, используем временный файл:
        local TMP_OUT="cmpr/tmp_entropy.bin"

        $EXE -i "$FILE" -o "$TMP_OUT" --bwt=$BUF --mtf > /dev/null 2>&1

        local CURRENT_ENTROPY=$(python3 -c "$PY_ENTROPY" < "$TMP_OUT")

        printf "%-15s | %-15s\n" "$BUF" "$CURRENT_ENTROPY"
        rm -f "$TMP_OUT"
    done
    echo -e "--------------------------------------------------------------------------\n"
}

# Создаем папку если нет
mkdir -p cmpr

# Запуск тестов
run_entropy_test "enwik7"
# run_entropy_test "xrEngine.exe"
# run_entropy_test "color.raw"
