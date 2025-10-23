#include <cmath>      // exp, log
#include <iostream>   // cout
#include <iomanip>    // setprecision, fixed
#include <vector>     // vector

/*
  Персональное задание №22.
  Требуется численно вычислить интеграл на отрезке [0, 1]:

      I = ∫_0^1  e^x / (1 + e^x) dx

  1) Аналитика:
     ∫ e^x/(1+e^x) dx = ln(1 + e^x) + C.
     Поэтому точное значение на [0,1]:
         I_exact = ln(1 + e^1) - ln(1 + e^0) = ln(1 + e) - ln(2).

  2) Численные методы (минимальный набор):
     - Метод трапеций:  O(h^2)
     - Формула Симпсона: O(h^4), требует чётное n.

  3) Вывод:
     Печатаем компактную таблицу:
         n ; Trapezoid ; Simpson ; Exact ; |Err(Trap)| ; |Err(Simpson)|

  Код максимально понятен и автономен: без внешних модулей, классов и усложнений.
*/

// Интегрируемая функция f(x) = e^x / (1 + e^x).
// Эквивалентная форма 1/(1+e^{-x}) возможна, но оставим исходную для наглядности.
static inline double f(double x) {
    return std::exp(x) / (1.0 + std::exp(x));
}

// Метод трапеций на равномерной сетке.
// a, b — границы интегрирования; n — число отрезков (n >= 1).
static double trapezoid(double a, double b, int n) {
    const double h = (b - a) / n;     // шаг сетки
    double s = 0.5 * (f(a) + f(b));   // полусумма крайних значений
    for (int i = 1; i < n; ++i) {
        s += f(a + i * h);            // внутренние узлы с коэффициентом 1
    }
    return s * h;
}

// Формула Симпсона (равномерная сетка, чётное n).
// Если n нечётно — увеличиваем на 1 (минимально инвазивная правка для корректности).
static double simpson(double a, double b, int n) {
    if (n % 2) ++n;                  // делаем n чётным
    const double h = (b - a) / n;
    double s = f(a) + f(b);          // коэффициенты 1 на концах

    // нечётные индексы — коэффициент 4
    for (int i = 1; i < n; i += 2) {
        s += 4.0 * f(a + i * h);
    }
    // чётные индексы (внутренние) — коэффициент 2
    for (int i = 2; i < n; i += 2) {
        s += 2.0 * f(a + i * h);
    }
    return s * h / 3.0;
}

int main() {
    const double a = 0.0;
    const double b = 1.0;

    // Точное значение: ln(1 + e) - ln(2)
    const double exact = std::log(1.0 + std::exp(1.0)) - std::log(2.0);

    // Наборы n для демонстрации сходимости/точности.
    // Можно свободно менять на свои.
    const std::vector<int> ns = {10, 1000, 100000, 1000000};

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "n;Trapezoid;Simpson;Exact;absErr(Trap);absErr(Simpson)\n";

    for (int n : ns) {
        const double T = trapezoid(a, b, n);
        const double S = simpson(a, b, n);

        std::cout << n << ';'
                  << T << ';'
                  << S << ';'
                  << exact << ';'
                  << std::abs(T - exact) << ';'
                  << std::abs(S - exact) << '\n';
    }

    return 0;
}