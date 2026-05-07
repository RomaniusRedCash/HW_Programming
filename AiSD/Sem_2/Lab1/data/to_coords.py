#!/bin/python
import sys
import re

def parse_to_coords(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Разбиваем по блокам файлов
    blocks = re.split(r'Processing:\s+', content)

    for block in blocks:
        if not block.strip(): continue

        file_name = re.search(r'^([^\s\-]+)', block).group(1)
        print(f"\n=== Coordinates for {file_name} ===")

        # Словари для накопления данных (чтобы потом отсортировать по X)
        lzss_coords = []
        lzw_coords = []

        lines = block.strip().split('\n')
        for line in lines:
            # Парсим LZSS (формат lzss.1024)
            lzss_match = re.search(r'lzss\.(\d+)\s+\|\s+\d+\s+\|\s+([\d\.]+)', line)
            if lzss_match:
                x = int(lzss_match.group(1))
                y = float(lzss_match.group(2))
                lzss_coords.append((x, y))

            # Парсим LZW (формат lzw.d-1024)
            lzw_match = re.search(r'lzw\.d\-(\d+)\s+\|\s+\d+\s+\|\s+([\d\.]+)', line)
            if lzw_match:
                x = int(lzw_match.group(1))
                y = float(lzw_match.group(2))
                lzw_coords.append((x, y))

        # Сортируем по X и выводим
        if lzss_coords:
            lzss_coords.sort()
            print("LZSS (Buffer size, Ratio %):")
            print(" ".join([f"({x}, {y})" for x, y in lzss_coords]))

        if lzw_coords:
            lzw_coords.sort()
            print("LZW (Dict size, Ratio %):")
            print(" ".join([f"({x}, {y})" for x, y in lzw_coords]))

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python to_coords.py <log_file>")
    else:
        parse_to_coords(sys.argv[1])
