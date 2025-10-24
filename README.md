# ЛБ4 ч.2 — вариант 15 (cosh)

## Build
```
rm -rf build && сmake -B build && cmake --build build && cd ./build/bin && ./Lab4 && ./task15_cosh
```

## Run
```
./bin/task15_cosh --a 0 --b 1 --n 10,1000,100000 --csv results_task15.csv
./bin/Lab4 --func cosh --a 0 --b 1 --n 10,100,1000 --csv results.csv
```

CSV format:
```
n;method;value;abs_err
10;LeftRect;...;...
```
