#!/bin/python
import re
import sys
import os

def generate_latex_tables(file_path):
    if not os.path.exists(file_path):
        print(f"Ошибка: Файл {file_path} не найден.")
        return

    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Ищем блоки данных, начинающиеся с "Processing:"
    blocks = re.split(r'Processing:\s+', content)

    latex_output = []

    for block in blocks:
        if not block.strip():
            continue

        # Извлекаем имя файла и Byte size
        file_name_match = re.search(r'^([^\s\(]+)', block)
        byte_size_match = re.search(r'Byte size:\s+(\d+)', block)

        if not file_name_match:
            continue

        file_name = file_name_match.group(1).replace('_', r'\_')
        byte_size = byte_size_match.group(1) if byte_size_match else "1"

        # Заголовок таблицы
        latex_output.append(r"\begin{table}[H]")
        latex_output.append(r"\centering")
        latex_output.append(r"\caption{Результаты сжатия файла \texttt{" + file_name + r"}}")
        latex_output.append(r"\begingroup\small") # Открываем группу и ставим размер
        latex_output.append(r"\begin{tabular}{|l|r|r|}%")
        latex_output.append(r"\hline%")
        latex_output.append(r"\textbf{Алгоритм} & \textbf{Размер (байт)} & \textbf{Коэф. (\%)} \\ \hline%")

        # Ищем строки с результатами (формат: Method | Size | Ratio)
        lines = block.strip().split('\n')
        for line in lines:
            if '|' in line and 'Method' not in line:
                parts = [p.strip() for p in line.split('|')]
                if len(parts) >= 3:
                    method = parts[0].replace('_', r'\_')
                    size = parts[1]
                    # Убираем лишние пробелы в процентах и экранируем символ %
                    ratio = parts[2].replace(' ', '').replace('%', r'\%')

                    latex_output.append(f"{method} & {size} & {ratio} \\\\ \\hline%")

        latex_output.append(r"\end{tabular}")
        latex_output.append(r"\endgroup")
        latex_output.append(r"\end{table}")
        latex_output.append("\n" + r"% " + "="*50 + "\n")

    print("\n".join(latex_output))

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование: python to_latex.py <путь_к_файлу_логов>")
    else:
        generate_latex_tables(sys.argv[1])
