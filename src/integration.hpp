#pragma once
#include <functional>
#include <string>
#include <vector>

using Func = std::function<double(double)>;

struct MethodResult {
    std::string name;
    int n;
    double value;
    double abs_err;
};

double left_rect(double a, double b, int n, const Func& f);
double mid_rect(double a, double b, int n, const Func& f);
double trapezoid(double a, double b, int n, const Func& f);
double simpson(double a, double b, int n, const Func& f);

std::vector<MethodResult> run_all(double a, double b, const std::vector<int>& ns,
                                  const Func& f,
                                  bool has_exact,
                                  double exact_value);

void save_csv(const std::string& path, const std::vector<MethodResult>& rows);
