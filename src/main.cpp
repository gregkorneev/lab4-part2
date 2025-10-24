#include "integration.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

static std::vector<int> parse_nlist(const std::string& s) {
    std::vector<int> ns;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        if (!tok.empty()) ns.push_back(std::stoi(tok));
    }
    if (ns.empty()) ns = {10, 1000, 100000};
    return ns;
}

int main(int argc, char** argv) {
    std::string func_key = "cosh";
    double a = 0.0, b = 1.0;
    std::vector<int> ns = {10, 1000, 100000};
    std::string out_csv = "results.csv";

    for (int i = 1; i < argc; ++i) {
        std::string key = argv[i];
        if ((key == "--func" || key == "-f") && i + 1 < argc) func_key = argv[++i];
        else if ((key == "--a" || key == "-a") && i + 1 < argc) a = std::stod(argv[++i]);
        else if ((key == "--b" || key == "-b") && i + 1 < argc) b = std::stod(argv[++i]);
        else if ((key == "--n" || key == "-n") && i + 1 < argc) ns = parse_nlist(argv[++i]);
        else if (key == "--csv" && i + 1 < argc) out_csv = argv[++i];
        else if (key == "--help" || key == "-h") {
            std::cout << "Usage: " << argv[0]
                      << " [--func cosh|quad|exp] [--a A] [--b B] [--n n1,n2,...] [--csv file]\n";
            return 0;
        }
    }

    Func f;
    bool has_exact = true;
    double exact = std::numeric_limits<double>::quiet_NaN();

    if (func_key == "cosh") {
        f = [](double x){ return std::cosh(x); };
        exact = std::sinh(b) - std::sinh(a);
    } else if (func_key == "quad") {
        f = [](double x){ return x*x; };
        exact = (b*b*b - a*a*a) / 3.0;
    } else if (func_key == "exp") {
        f = [](double x){ return std::exp(x); };
        exact = std::exp(b) - std::exp(a);
    } else {
        f = [](double x){ return x; };
        has_exact = false;
    }

    auto rows = run_all(a, b, ns, f, has_exact, exact);
    save_csv(out_csv, rows);

    std::cout << "func=" << func_key << ", interval=[" << a << "," << b
              << "], exact=" << (has_exact ? exact : std::numeric_limits<double>::quiet_NaN()) << "\n";
    std::cout << "CSV saved to: " << out_csv << "\n";
    return 0;
}
