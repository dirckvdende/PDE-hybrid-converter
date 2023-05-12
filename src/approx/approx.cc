
#include "approx.h"

SpatialApprox::SpatialApprox(const std::vector<std::string> &dims) : dims(dims)
{
    for (size_t i = 0; i < dims.size(); i++)
        dimMap.emplace(dims[i], i);
}

ExprNode SpatialApprox::getApprox(std::string func, const std::vector<size_t>
&pos, const std::vector<std::string> &deriv) {
    std::vector<size_t> amounts(dims.size(), 0);
    for (const std::string &d : deriv)
        amounts[dimMap[d]]++;
    // TODO: implement
}

std::vector<double> SpatialApprox::getCoeffs(size_t n, double scale) {
    if (n % 2 == 0) {
        std::vector<double> coeffs(n + 1);
        // c_i = (-1)^(n-i) (n choose i) / scale^n
        double c = 1.0;
        if (n % 2 != 0)
            c = -1.0;
        for (size_t i = 0; i < n; i++)
            c /= scale;
        for (size_t i = 0; i <= n; i++) {
            coeffs[i] = c;
            c = c / (i + 1) * (n - i);
        }
        return coeffs;
    }
    std::vector<double> prev = getCoeffs(n - 1, scale);
    std::vector<double> coeffs(n + 2, 0.0);
    for (size_t i = 0; i <= n + 1; i++) {
        if (i <= n - 1)
            coeffs[i] -= prev[i];
        if (i >= 2)
            coeffs[i] += prev[i - 2];
        coeffs[i] /= scale;
    }
    return coeffs;
}