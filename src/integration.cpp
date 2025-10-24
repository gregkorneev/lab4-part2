#include "integration.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <stdexcept>

double left_rect(double a, double b, int n, const Func& f) {
    const double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; ++i) s += f(a + i * h);
    return s * h;
}

double mid_rect(double a, double b, int n, const Func& f) {
    const double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; ++i) s += f(a + (i + 0.5) * h);
    return s * h;
}

double trapezoid(double a, double b, int n, const Func& f) {
    const double h = (b - a) / n;
    double s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) s += f(a + i * h);
    return s * h;
}

double simpson(double a, double b, int n, const Func& f) {
    if (n % 2 != 0) ++n;
    const double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; i += 2) s += 4.0 * f(a + i * h);
    for (int i = 2; i < n; i += 2) s += 2.0 * f(a + i * h);
    return s * h / 3.0;
}

static void push_row(std::vector<MethodResult>& v, const std::string& name,
                     int n, double val, double exact, bool has_exact) {
    MethodResult r{ name, n, val, has_exact ? std::abs(val - exact) : std::numeric_limits<double>::quiet_NaN() };
    v.push_back(r);
}

std::vector<MethodResult> run_all(double a, double b, const std::vector<int>& ns,
                                  const Func& f,
                                  bool has_exact,
                                  double exact_value) {
    std::vector<MethodResult> out;
    out.reserve(ns.size() * 4);
    for (int n : ns) {
        if (n <= 0) continue;
        push_row(out, "LeftRect",  n, left_rect(a,b,n,f),  exact_value, has_exact);
        push_row(out, "MidRect",   n, mid_rect(a,b,n,f),   exact_value, has_exact);
        push_row(out, "Trapezoid", n, trapezoid(a,b,n,f),  exact_value, has_exact);
        push_row(out, "Simpson",   n, simpson(a,b,n,f),    exact_value, has_exact);
    }
    return out;
}

void save_csv(const std::string& path, const std::vector<MethodResult>& rows) {
    std::ofstream ofs(path);
    if (!ofs) throw std::runtime_error("Cannot open CSV file for writing: " + path);
    ofs << "n;method;value;abs_err\n";
    ofs << std::setprecision(17);
    for (const auto& r : rows) {
        ofs << r.n << ';' << r.name << ';' << r.value << ';';
        if (std::isnan(r.abs_err)) ofs << ""; else ofs << r.abs_err;
        ofs << "\n";
    }
}
