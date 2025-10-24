#!/usr/bin/env python3
# Построение графиков ЛБ на русском языке.
# Правила: matplotlib (без seaborn), один график на фигуру, без явных цветов/стилей.
import sys, os
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams['axes.unicode_minus'] = False

METHOD_RU = {
    'LeftRect': 'Левые прямоугольники',
    'MidRect': 'Средние прямоугольники',
    'Trapezoид': 'Трапеции',
    'Simpson': 'Симпсон',
}

def main(csv_path):
    df = pd.read_csv(csv_path, sep=';')
    df['method'] = df['method'].map(lambda m: METHOD_RU.get(m, m))

    pv_val = df.pivot_table(index='n', columns='method', values='value', aggfunc='first').sort_index()
    pv_err = df.pivot_table(index='n', columns='method', values='abs_err', aggfunc='first').sort_index()

    os.makedirs('plots', exist_ok=True)

    # 1) Значение интеграла vs n
    plt.figure()
    for col in pv_val.columns:
        plt.plot(pv_val.index, pv_val[col], marker='o', label=col)
    plt.xlabel('n (число разбиений)')
    plt.ylabel('Значение интеграла')
    plt.title('Значение интеграла в зависимости от n')
    plt.grid(True, which='both')
    plt.legend()
    plt.savefig('plots/value_vs_n.png', dpi=200)
    plt.close()

    # 2) Абсолютная погрешность vs n (лог-лог)
    plt.figure()
    for col in pv_err.columns:
        series = pv_err[col].dropna()
        if series.empty:
            continue
        plt.loglog(series.index, series.values, marker='o', label=col)
    plt.xlabel('n (число разбиений)')
    plt.ylabel('Абсолютная погрешность')
    plt.title('Абсолютная погрешность в зависимости от n (лог-лог)')
    plt.grid(True, which='both')
    plt.legend()
    plt.savefig('plots/error_vs_n.png', dpi=200)
    plt.close()

    print('Сохранено: plots/value_vs_n.png и plots/error_vs_n.png')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Использование: plot_errors.py <results.csv>')
        sys.exit(1)
    main(sys.argv[1])
