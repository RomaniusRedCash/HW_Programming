#!/bin/python
import math
import sys

def calculate_entropy(data):
    if not data: return 0
    file_size = len(data)
    counts = {}
    for byte in data:
        counts[byte] = counts.get(byte, 0) + 1

    entropy = 0
    for count in counts.values():
        p = count / file_size
        entropy -= p * math.log2(p)
    return entropy

if __name__ == "__main__":
    # Читаем байты из stdin
    input_data = sys.stdin.buffer.read()
    print(f"{calculate_entropy(input_data):.6f}")
