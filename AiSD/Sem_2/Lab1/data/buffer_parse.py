#!/bin/python
import re
import argparse
import sys

def parse_performance_data(file_path):
    # Регулярное выражение для поиска значений Buffer, Time и Ratio
    # Ищем числа (включая точки для float) после соответствующих ключевых слов
    pattern = re.compile(r"Buffer:\s*(?P<buffer>\d+)\s*\|\s*Time:\s*(?P<time>[\d.]+)s\s*\|\s*Ratio:\s*(?P<ratio>[\d.]+)%")

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

            # Разделяем контент на блоки по заголовкам анализа
            sections = re.split(r"=== Performance Analysis for .+ ===", content)

            # Первый элемент сплита обычно пустой, если файл начинается с заголовка
            for section in sections:
                if not section.strip():
                    continue

                matches = pattern.finditer(section)

                results_time = []
                results_ratio = []

                for match in matches:
                    b = match.group('buffer')
                    t = match.group('time')
                    r = match.group('ratio')

                    results_time.append(f"({b}, {t})")
                    results_ratio.append(f"({b}, {r})")

                if results_time:
                    print(" ".join(results_time))
                    print(" ".join(results_ratio))
                    print("-" * 20) # Разделитель между блоками файлов

    except FileNotFoundError:
        print(f"Ошибка: Файл '{file_path}' не найден.")
    except Exception as e:
        print(f"Произошла ошибка: {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Парсинг логов производительности.")
    parser.add_argument("input_file", help="Путь к текстовому файлу с данными")

    args = parser.parse_args()
    parse_performance_data(args.input_file)
