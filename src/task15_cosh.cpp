#include "integration.hpp"
#include <cmath>
#include <iostream>
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
    double a = 0.0, b = 1.0;
    std::vector<int> ns = {10, 1000, 100000};
    std::string out_csv = "results_task15.csv";

    for (int i = 1; i < argc; ++i) {
        std::string key = argv[i];
        if ((key == "--a" || key == "-a") && i + 1 < argc) a = std::stod(argv[++i]);
        else if ((key == "--b" || key == "-b") && i + 1 < argc) b = std::stod(argv[++i]);
        else if ((key == "--n" || key == "-n") && i + 1 < argc) ns = parse_nlist(argv[++i]);
        else if (key == "--csv" && i + 1 < argc) out_csv = argv[++i];
        else if (key == "--help" || key == "-h") {
            std::cout << "Usage: " << argv[0]
                      << " [--a A] [--b B] [--n n1,n2,...] [--csv file]\n";
            return 0;
        }
    }

    const auto f = [](double x) { return std::cosh(x); };
    const double exact = std::sinh(b) - std::sinh(a);

    const auto rows = run_all(a, b, ns, f, true, exact);
    save_csv(out_csv, rows);

    std::cout << "Variant 15: f(x)=cosh(x)\n"
              << "Interval: [" << a << ", " << b << "]\n"
              << "Exact (sinh(b)-sinh(a)) = " << exact << "\n"
              << "CSV saved to: " << out_csv << "\n";
    return 0;
}
