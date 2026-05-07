#!/bin/python
import sys
def parse_logs_line_by_line(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            if "Results for" in line:
                name = line.split("Results for")[-1].strip()
                print(f"\n% --- {name} ---")
                continue
            if "Byte size:" in line and "% ---" in line: # доп. инфо в заголовок
                size = line.split("Byte size:")[-1].replace(')', '').strip()
                print(f"% Byte size: {size}")
                continue
            if '|' in line and 'Method' not in line and '---' not in line:
                parts = [p.strip() for p in line.split('|')]
                if len(parts) >= 3:
                    method = parts[0].replace('_', r'\_')
                    size = parts[1]
                    print(f"                ({method},{size})%")
if __name__ == "__main__":
    # Проверяем, передан ли аргумент
    if len(sys.argv) > 1:
        path = sys.argv[1]
        parse_logs_line_by_line(path)
    else:
        print("Ошибка: Укажите путь к файлу. Пример: python script.py file.log")
