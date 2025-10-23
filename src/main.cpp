#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>

// Функция для интегрирования (вариант 6): f(x) = x^2 * sin(x)
double f(double x) {
    return x * x * std::sin(x);
}

// Аналитическое решение интеграла x^2 * sin(x) dx
double analytical_solution(double a, double b) {
    auto F = [](double x) {
        return (2 - x*x) * std::cos(x) + 2*x * std::sin(x);
    };
    return F(b) - F(a);
}

// Метод левых прямоугольников
double left_rectangle(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Метод правых прямоугольников
double right_rectangle(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Метод средних прямоугольников
double mid_rectangle(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    return sum * h;
}

// Метод трапеций
double trapezoidal(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Метод Симпсона
double simpson(double a, double b, int n) {
    if (n % 2 != 0) n++;
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    
    for (int i = 1; i < n; i += 2) {
        double x = a + i * h;
        sum += 4 * f(x);
    }
    
    for (int i = 2; i < n; i += 2) {
        double x = a + i * h;
        sum += 2 * f(x);
    }
    
    return sum * h / 3.0;
}

// Функция для измерения времени выполнения
template<typename Func>
std::pair<double, long long> measure_time(Func method, double a, double b, int n) {
    auto start = std::chrono::high_resolution_clock::now();
    double result = method(a, b, n);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    return std::make_pair(result, duration.count());
}

// Функция для создания графика точности от количества разбиений
void print_precision_graph(const std::vector<int>& n_values,
                          const std::vector<std::vector<double>>& errors,
                          const std::vector<std::string>& method_names) {
    const int graph_width = 70;
    const int graph_height = 20;
    
    // Находим диапазон ошибок для масштабирования
    double min_error = 1e20;
    double max_error = 1e-20;
    
    for (const auto& method_errors : errors) {
        for (double error : method_errors) {
            if (error > 0) {
                if (error < min_error) min_error = error;
                if (error > max_error) max_error = error;
            }
        }
    }
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "ГРАФИК: Зависимость точности от количества разбиений n\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << "Ось Y: -log10(ошибка) (чем выше, тем точнее)\n";
    std::cout << "Ось X: количество разбиений n\n\n";
    
    // Символы для разных методов
    std::vector<char> symbols = {'L', 'R', 'M', 'T', 'S'};
    std::vector<std::string> colors = {"[1;31m", "[1;32m", "[1;33m", "[1;34m", "[1;35m"};
    
    // Создаем координатную сетку
    for (int row = graph_height; row >= 0; row--) {
        // Левая шкала (точность = -log10(ошибка))
        double precision_value = (static_cast<double>(row) / graph_height) * 15.0; // от 0 до 15 знаков точности
        std::cout << std::setw(6) << std::fixed << std::setprecision(1) 
                  << precision_value << " |";
        
        // Отображаем точки для каждого метода
        for (int col = 0; col < graph_width; col++) {
            // Логарифмическая шкала для n
            double n_log = std::log10(n_values[0]) + 
                          (static_cast<double>(col) / graph_width) * 
                          (std::log10(n_values.back()) - std::log10(n_values[0]));
            double n_value = std::pow(10, n_log);
            
            bool point_drawn = false;
            for (size_t method_idx = 0; method_idx < method_names.size(); method_idx++) {
                // Находим ближайшее значение n
                for (size_t n_idx = 0; n_idx < n_values.size(); n_idx++) {
                    if (std::abs(std::log10(n_values[n_idx]) - n_log) < 0.1) {
                        double error = errors[method_idx][n_idx];
                        if (error > 0) {
                            double precision = -std::log10(error); // чем выше, тем точнее
                            int precision_row = static_cast<int>((precision / 15.0) * graph_height);
                            
                            if (precision_row == row) {
                                std::cout << "\033" << colors[method_idx] << symbols[method_idx] << "\033[0m";
                                point_drawn = true;
                                break;
                            }
                        }
                    }
                }
                if (point_drawn) break;
            }
            
            if (!point_drawn) {
                if (row == 0) {
                    std::cout << "-"; // Ось X
                } else {
                    std::cout << " ";
                }
            }
        }
        std::cout << "\n";
    }
    
    // Ось X (логарифмическая шкала для n)
    std::cout << "       +";
    for (int col = 0; col < graph_width; col++) {
        std::cout << "-";
    }
    std::cout << "> n\n        ";
    
    for (int col = 0; col <= graph_width; col += graph_width / 6) {
        double n_log = std::log10(n_values[0]) + 
                      (static_cast<double>(col) / graph_width) * 
                      (std::log10(n_values.back()) - std::log10(n_values[0]));
        int n_value = static_cast<int>(std::pow(10, n_log));
        std::cout << std::setw(8) << n_value;
    }
    std::cout << "\n";
    
    // Легенда
    std::cout << "\nЛегенда:\n";
    for (size_t i = 0; i < method_names.size(); i++) {
        std::cout << "\033" << colors[i] << symbols[i] << "\033[0m - " << method_names[i] << "\n";
    }
    std::cout << "\nПримечание: Высота точки показывает количество верных знаков после запятой\n";
}

// Функция для создания графика времени от точности
void print_time_precision_graph(const std::vector<int>& n_values,
                               const std::vector<std::vector<long long>>& times,
                               const std::vector<std::vector<double>>& errors,
                               const std::vector<std::string>& method_names) {
    const int graph_width = 70;
    const int graph_height = 20;
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "ГРАФИК: Зависимость времени вычисления от достигнутой точности\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << "Ось Y: время (наносекунды, логарифмическая шкала)\n";
    std::cout << "Ось X: -log10(ошибка) (количество верных знаков)\n\n";
    
    std::vector<char> symbols = {'L', 'R', 'M', 'T', 'S'};
    std::vector<std::string> colors = {"[1;31m", "[1;32m", "[1;33m", "[1;34m", "[1;35m"};
    
    // Находим диапазоны для масштабирования
    long long max_time = 0;
    double max_precision = 0;
    
    for (size_t method_idx = 0; method_idx < method_names.size(); method_idx++) {
        for (size_t n_idx = 0; n_idx < n_values.size(); n_idx++) {
            if (times[method_idx][n_idx] > max_time) max_time = times[method_idx][n_idx];
            double error = errors[method_idx][n_idx];
            if (error > 0) {
                double precision = -std::log10(error);
                if (precision > max_precision) max_precision = precision;
            }
        }
    }
    
    for (int row = graph_height; row >= 0; row--) {
        // Левая шкала (время)
        double time_value = std::pow(10, 
            std::log10(1000.0) + (static_cast<double>(row) / graph_height) * 
            (std::log10(max_time) - std::log10(1000.0)));
        
        if (row % 4 == 0) {
            if (time_value < 1e6) {
                std::cout << std::setw(5) << static_cast<int>(time_value/1000) << "K |";
            } else {
                std::cout << std::setw(5) << static_cast<int>(time_value/1e6) << "M |";
            }
        } else {
            std::cout << "      |";
        }
        
        // Отображаем точки для каждого метода
        for (int col = 0; col < graph_width; col++) {
            double precision_value = (static_cast<double>(col) / graph_width) * max_precision;
            
            bool point_drawn = false;
            for (size_t method_idx = 0; method_idx < method_names.size(); method_idx++) {
                for (size_t n_idx = 0; n_idx < n_values.size(); n_idx++) {
                    double error = errors[method_idx][n_idx];
                    if (error > 0) {
                        double precision = -std::log10(error);
                        if (std::abs(precision - precision_value) < 0.3) {
                            double time_log = std::log10(times[method_idx][n_idx]);
                            double min_time_log = std::log10(1000.0);
                            double max_time_log = std::log10(max_time);
                            int time_row = static_cast<int>(
                                (max_time_log - time_log) / (max_time_log - min_time_log) * graph_height);
                            
                            if (time_row == row) {
                                std::cout << "\033" << colors[method_idx] << symbols[method_idx] << "\033[0m";
                                point_drawn = true;
                                break;
                            }
                        }
                    }
                }
                if (point_drawn) break;
            }
            
            if (!point_drawn) {
                if (row == graph_height) {
                    std::cout << "-";
                } else {
                    std::cout << " ";
                }
            }
        }
        std::cout << "\n";
    }
    
    // Ось X (точность)
    std::cout << "      +";
    for (int col = 0; col < graph_width; col++) {
        std::cout << "-";
    }
    std::cout << "> точность\n       ";
    
    for (int col = 0; col <= graph_width; col += graph_width / 6) {
        double precision_value = (static_cast<double>(col) / graph_width) * max_precision;
        std::cout << std::setw(8) << std::fixed << std::setprecision(1) << precision_value;
    }
    std::cout << "\n";
    
    std::cout << "\nЛегенда:\n";
    for (size_t i = 0; i < method_names.size(); i++) {
        std::cout << "\033" << colors[i] << symbols[i] << "\033[0m - " << method_names[i] << "\n";
    }
}

// Функция для создания таблицы точности
void print_precision_table(const std::vector<int>& n_values,
                          const std::vector<std::vector<double>>& errors,
                          const std::vector<std::string>& method_names) {
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "ТАБЛИЦА: Количество верных знаков после запятой\n";
    std::cout << std::string(100, '=') << "\n";
    
    std::cout << std::setw(8) << "n";
    for (const auto& name : method_names) {
        std::cout << std::setw(15) << name;
    }
    std::cout << "\n" << std::string(100, '-') << "\n";
    
    for (size_t n_idx = 0; n_idx < n_values.size(); n_idx++) {
        std::cout << std::setw(8) << n_values[n_idx];
        for (size_t method_idx = 0; method_idx < method_names.size(); method_idx++) {
            double error = errors[method_idx][n_idx];
            if (error > 0) {
                double correct_digits = -std::log10(error);
                std::cout << std::setw(15) << std::fixed << std::setprecision(1) << correct_digits;
            } else {
                std::cout << std::setw(15) << "∞";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    // Параметры для варианта 6
    double a = 0.0;
    double b = 3.14159; // π
    
    // Значения n для тестирования (больше значений для лучшего графика)
    std::vector<int> n_values = {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    std::vector<std::string> method_names = {
        "Left Rect", "Right Rect", "Mid Rect", "Trapezoid", "Simpson"
    };
    
    // Аналитическое решение
    double analytical = analytical_solution(a, b);
    std::cout << "АНАЛИТИЧЕСКОЕ РЕШЕНИЕ: ∫x²sin(x)dx от " << a << " до " << b << " = " 
              << std::setprecision(10) << analytical << "\n";
    
    // Векторы для хранения результатов
    std::vector<std::vector<double>> results(method_names.size());
    std::vector<std::vector<long long>> times(method_names.size());
    std::vector<std::vector<double>> errors(method_names.size());
    
    // Заполняем векторы методов
    std::vector<double(*)(double, double, int)> methods = {
        left_rectangle, right_rectangle, mid_rectangle, trapezoidal, simpson
    };
    
    std::cout << "\nВыполняются вычисления для построения графиков точности...\n";
    std::cout << "Это может занять несколько секунд...\n";
    
    // Выполняем вычисления
    for (size_t method_idx = 0; method_idx < methods.size(); method_idx++) {
        std::cout << "Метод: " << method_names[method_idx] << "... ";
        for (int n : n_values) {
            auto [result, time] = measure_time(methods[method_idx], a, b, n);
            results[method_idx].push_back(result);
            times[method_idx].push_back(time);
            double error = std::abs(result - analytical);
            // Избегаем нулевой ошибки для логарифма
            errors[method_idx].push_back(error > 0 ? error : 1e-20);
        }
        std::cout << "готово\n";
    }
    
    // Выводим графики и таблицы
    print_precision_graph(n_values, errors, method_names);
    print_time_precision_graph(n_values, times, errors, method_names);
    print_precision_table(n_values, errors, method_names);
    
    // Создаем CSV файл с результатами
    std::ofstream csv_file("precision_analysis.csv");
    csv_file << "n,Left_Rectangle_Precision,Right_Rectangle_Precision,Mid_Rectangle_Precision,"
             << "Trapezoidal_Precision,Simpson_Precision,"
             << "Left_Rectangle_Time,Right_Rectangle_Time,Mid_Rectangle_Time,"
             << "Trapezoidal_Time,Simpson_Time\n";
    
    for (size_t n_idx = 0; n_idx < n_values.size(); n_idx++) {
        csv_file << n_values[n_idx];
        
        // Точность (количество верных знаков)
        for (size_t method_idx = 0; method_idx < methods.size(); method_idx++) {
            double precision = -std::log10(errors[method_idx][n_idx]);
            csv_file << "," << precision;
        }
        
        // Время
        for (size_t method_idx = 0; method_idx < methods.size(); method_idx++) {
            csv_file << "," << times[method_idx][n_idx];
        }
        
        csv_file << "\n";
    }
    csv_file.close();
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "РЕЗУЛЬТАТЫ СОХРАНЕНЫ В ФАЙЛ: precision_analysis.csv\n";
    std::cout << std::string(50, '=') << "\n";
    
    return 0;
}