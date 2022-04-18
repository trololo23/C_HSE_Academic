#pragma once
#include "vector"
#include "list"
#include "ostream"
#include "cmath"

class Poly {
public:
    Poly();

    Poly(const std::vector<int64_t>& vec_of_coeffs);

    Poly(const std::vector<std::pair<int64_t, int64_t>>& coeffs_to_degs);

    int64_t operator()(int64_t arg) const;

    Poly(const Poly& other) = default;

    bool operator==(const Poly& other) const;

    bool operator!=(const Poly& other) const;

    Poly& operator+=(const Poly& other);

    Poly& operator-=(const Poly& other);

    Poly operator*(const Poly& other) const;

    Poly operator+(const Poly& other) const;

    Poly operator-(const Poly& other) const;

    Poly operator-() const;

    friend std::ostream& operator<<(std::ostream& stream, const Poly& poly);

private:
    std::list<std::pair<int64_t, int64_t>> coeffs_ = {{0, 0}};
};
