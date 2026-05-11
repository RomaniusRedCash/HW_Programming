#!/bin/bash

cleanup() {
    echo -e "\n[!] CTRL+C detected. Killing child processes..."
    # kill 0 убивает все процессы в текущей группе процессов (весь скрипт и его дети)
    trap - SIGINT # Отключаем ловушку для самой себя, чтобы не зациклиться
    kill -9 0
}

# Вешаем функцию на сигнал SIGINT (Ctrl+C) и SIGTERM
trap cleanup SIGINT SIGTERM

mkdir -p cmpr
export EXE=../../exe

# Определение функции
run_test() {
    local FILE=$1      # Первый аргумент: имя файла
    local BYTE_SIZE=$2 # Второй аргумент: --byte (1 или 2)

    if [ ! -f "$FILE" ]; then
        echo "File $FILE not found!"
        return 1
    fi

    local ORIG_SIZE=$(stat -c%s "$FILE")
    echo "Processing: $FILE (Bytes: $BYTE_SIZE, Size: $ORIG_SIZE)"

    # Локальные массивы настроек
    local BWT_BUFFERS=(921600)
    local LZSS_BUFFERS=(65536)
    local LZW_BUFFERS=(524288)
    local METODS=(
        # "--ha" "--rle" "--bwt --rle" "--bwt --mtf --ha"
        # "--bwt --rle --mtf --ha" "--lzss" "--lzss --ha"
        # "--lzss" "--lzss --ha"
        "--lzw" "--lzw --ha"
    )
    echo $FILE

    # Вложенная функция для параллельного запуска
    process_method() {
        local MET=$1
        local BUF=$2
        local F=$3
        local B=$4

        local CMD_FLAGS=$MET
        if [ "$BUF" != "0" ]; then
            CMD_FLAGS=${CMD_FLAGS//--bwt/--bwt=$BUF}
            CMD_FLAGS=${CMD_FLAGS//--lzss/--lzss=$BUF}
            CMD_FLAGS=${CMD_FLAGS//--lzw/--lzw=f,$BUF}
        fi

        local TAG=${CMD_FLAGS//--/}
        TAG=${TAG// /_}; TAG=${TAG//=/.}; TAG=${TAG//,/-}
        local OUT="cmpr/${F}_b${B}_${TAG}.compr"

        # echo $EXE --byte=$B -i "$F" -o "$OUT" $CMD_FLAGS 2>/dev/null
        $EXE --byte=$B -i "$F" -o "$OUT" $CMD_FLAGS
    }

    # Запуск
    for METOD in "${METODS[@]}"; do
        local CURRENT_BUFFERS=(0)
        [[ $METOD == *"--bwt"* ]] && CURRENT_BUFFERS=("${BWT_BUFFERS[@]}")
        [[ $METOD == *"--lzss"* ]] && CURRENT_BUFFERS=("${LZSS_BUFFERS[@]}")
        [[ $METOD == *"--lzw"* ]] && CURRENT_BUFFERS=("${LZW_BUFFERS[@]}")

        for BUF in "${CURRENT_BUFFERS[@]}"; do
            process_method "$METOD" "$BUF" "$FILE" "$BYTE_SIZE" &

            if [[ $(jobs -r | wc -l) -ge $(nproc) ]]; then
                wait -n
            fi
        done
    done

    wait # Ждем окончания сжатия для этого файла

    # Вывод результатов для конкретного файла
    echo "--------------------------------------------------------------------------"
    printf "Results for %-15s (Byte size: %s)\n" "$FILE" "$BYTE_SIZE"
    echo "--------------------------------------------------------------------------"
    printf "%-30s | %-12s | %-10s\n" "Method" "Size (bytes)" "Ratio (%)"
    echo "--------------------------------------------------------------------------"

    # Маска поиска теперь включает b$BYTE_SIZE, чтобы не смешивать тесты
    for OUT_FILE in cmpr/${FILE}_b${BYTE_SIZE}_*.compr; do
        [ -f "$OUT_FILE" ] || continue
        local COMPR_SIZE=$(stat -c%s "$OUT_FILE")
        local RATIO=$(echo "scale=2; $COMPR_SIZE * 100 / $ORIG_SIZE" | bc)
        local TAG=$(basename "$OUT_FILE")
        TAG=${TAG#${FILE}_b${BYTE_SIZE}_}
        TAG=${TAG%.compr}
        printf "%-30s | %-12s | %-10s%%\n" "$TAG" "$COMPR_SIZE" "$RATIO"
    done
    echo -e "--------------------------------------------------------------------------\n"
}

# --- ИСПОЛЬЗОВАНИЕ ---

# run_test "rus.txt" 1
run_test "rus.txt" 2
# run_test "enwik7" 1
# run_test "xrEngine.exe" 1
# run_test "color.raw" 1
# run_test "chb.raw" 1
# run_test "gray.raw" 1
# run_test "color.raw" 3
run_test "chb.raw" 3
# run_test "gray.raw" 3
