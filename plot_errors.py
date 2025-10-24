#!/usr/bin/env python3
# Plot results from CSV produced by the C++ programs.
# Rules: use matplotlib (no seaborn), one chart per figure, no explicit colors/styles.
import sys, os
import pandas as pd
import matplotlib.pyplot as plt

def main(csv_path):
    df = pd.read_csv(csv_path, sep=';')
    pv_val = df.pivot_table(index='n', columns='method', values='value', aggfunc='first')
    pv_err = df.pivot_table(index='n', columns='method', values='abs_err', aggfunc='first')

    os.makedirs('plots', exist_ok=True)

    # Values vs n
    plt.figure()
    for col in pv_val.columns:
        plt.plot(pv_val.index, pv_val[col], marker='o', label=col)
    plt.xlabel('n')
    plt.ylabel('Integral value')
    plt.title('Integral values vs n')
    plt.grid(True, which='both')
    plt.legend()
    plt.savefig('plots/value_vs_n.png', dpi=200)
    plt.close()

    # Abs error vs n (log-log)
    plt.figure()
    for col in pv_err.columns:
        series = pv_err[col].dropna()
        if series.empty:
            continue
        plt.loglog(series.index, series.values, marker='o', label=col)
    plt.xlabel('n')
    plt.ylabel('abs error')
    plt.title('Absolute error vs n (log-log)')
    plt.grid(True, which='both')
    plt.legend()
    plt.savefig('plots/error_vs_n.png', dpi=200)
    plt.close()

    print('Saved plots to plots/value_vs_n.png and plots/error_vs_n.png')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: plot_errors.py <results.csv>')
        sys.exit(1)
    main(sys.argv[1])
