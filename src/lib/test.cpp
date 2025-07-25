#include "test.hpp"

void printComplexVector(std::vector<std::complex<double>> v) {
  std::cout << "{\n";
  for (size_t i = 0; i < v.size(); ++i) {
    double re = std::abs(v[i].real()) < 1e-10 ? 0 : v[i].real();
    double im = std::abs(v[i].imag()) < 1e-10 ? 0 : v[i].imag();
    std::cout << "\t" << re << " + " << im << "i,\n";
  }
  std::cout << "}\n";
}
