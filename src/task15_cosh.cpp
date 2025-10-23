#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

/*
  Индивидуальное задание №15: ∫_0^1 cosh(x) dx
  Аналитический ответ: sinh(1) - sinh(0) = sinh(1)

  Реализованы 4 метода: левые/средние прямоугольники, трапеции, Симпсон.
  Вывод — таблица (CSV-подобная) для наборов n.

  Этот файл — отдельный исполняемый модуль, не трогает остальной код лаборатории.
*/

// --- функция интегрируемая на [a,b] ---
static inline double f(double x) {
    return std::cosh(x);
}

// --- численные методы ---
double left_rect(double a, double b, int n) {
    double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; ++i)
        s += f(a + i * h);
    return s * h;
}

double mid_rect(double a, double b, int n) {
    double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; ++i)
        s += f(a + (i + 0.5) * h);
    return s * h;
}

double trapezoid(double a, double b, int n) {
    double h = (b - a) / n;
    double s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i)
        s += f(a + i * h);
    return s * h;
}

double simpson(double a, double b, int n) {
    if (n % 2) ++n; // для Симпсона n должно быть чётным
    double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; i += 2) s += 4.0 * f(a + i * h);
    for (int i = 2; i < n; i += 2) s += 2.0 * f(a + i * h);
    return s * h / 3.0;
}

int main() {
    const double a = 0.0, b = 1.0;
    const double exact = std::sinh(1.0); // аналитический ответ
    std::vector<int> ns = {10, 1000, 100000, 10000000};

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "n;LeftRect;MidRect;Trapezoid;Simpson;Exact;sigma(Left);sigma(Mid);sigma(Trap);sigma(Simpson)\n";

    for (int n : ns) {
        double L = left_rect(a, b, n);
        double M = mid_rect(a, b, n);
        double T = trapezoid(a, b, n);
        double S = simpson(a, b, n);

        std::cout << n << ';'
                  << L << ';' << M << ';' << T << ';' << S << ';' << exact << ';'
                  << std::abs(L - exact) << ';'
                  << std::abs(M - exact) << ';'
                  << std::abs(T - exact) << ';'
                  << std::abs(S - exact) << '\n';
    }

    return 0;
}