#include "poly.h"
#include "iostream"

Poly::Poly(const std::vector<int64_t> &vec_of_coeffs) {
    coeffs_.clear();
    for (size_t i = 0; i < vec_of_coeffs.size(); ++i) {
        if (vec_of_coeffs[i] != 0) {
            coeffs_.emplace_back(i, vec_of_coeffs[i]);
        }
    }
}

Poly::Poly(const std::vector<std::pair<int64_t, int64_t>> &coeffs_to_degs) {
    coeffs_.clear();
    for (const auto &deg_coeff : coeffs_to_degs) {
        coeffs_.push_back(deg_coeff);
    }
}

Poly::Poly() {
}

int64_t Poly::operator()(int64_t arg) const {
    int64_t result = 0;
    for (const auto &[deg, coeff] : coeffs_) {
        result += coeff * powl(arg, deg);
    }
    return result;
}

bool Poly::operator==(const Poly &other) const {
    return coeffs_ == other.coeffs_;
}

bool Poly::operator!=(const Poly &other) const {
    return !(*this == other);
}

Poly &Poly::operator+=(const Poly &other) {
    auto first_ptr = coeffs_.begin();
    auto sec_ptr = other.coeffs_.begin();
    while (first_ptr != coeffs_.end() && sec_ptr != other.coeffs_.end()) {
        if (sec_ptr->first < first_ptr->first) {
            if (sec_ptr->second != 0) {
                coeffs_.insert(first_ptr, *sec_ptr);
            }
            ++sec_ptr;
        } else if (sec_ptr->first == first_ptr->first) {
            first_ptr->second += sec_ptr->second;
            if (first_ptr->second == 0) {
                first_ptr = coeffs_.erase(first_ptr);
            }
            ++sec_ptr;
        } else {
            ++first_ptr;
            if (first_ptr == coeffs_.end()) {
                for (auto it = sec_ptr; it != other.coeffs_.end(); ++it) {
                    if (it->second != 0) {
                        coeffs_.insert(first_ptr, *it);
                    }
                }
            }
        }
    }
    if (this->coeffs_.empty()) {
        this->coeffs_.push_back({0, 0});
    }
    return *this;
}

Poly Poly::operator-() const {
    Poly polynom = *this;
    for (auto &coeff : polynom.coeffs_) {
        coeff.second *= -1;
    }
    return polynom;
}

Poly &Poly::operator-=(const Poly &other) {
    *this += -other;
    return *this;
}

Poly Poly::operator+(const Poly &other) const {
    Poly polynom = *this;
    polynom += other;
    return polynom;
}

Poly Poly::operator-(const Poly &other) const {
    Poly polynom = *this;
    polynom -= other;
    return polynom;
}

Poly Poly::operator*(const Poly &other) const {
    Poly polynom;
    for (auto i = coeffs_.begin(); i != coeffs_.end(); ++i) {
        for (auto j = other.coeffs_.begin(); j != other.coeffs_.end(); ++j) {
            polynom += Poly({std::make_pair(i->first + j->first, i->second * j->second)});
        }
    }
    return polynom;
}

std::ostream &operator<<(std::ostream &stream, const Poly &poly) {
    stream << "y =";

    for (auto i = poly.coeffs_.rbegin(); i != poly.coeffs_.rend(); ++i) {
        stream << " " << i->second;
        if (i->first > 1) {
            stream << "x^" << i->first;
        } else if (i->first == 1) {
            stream << "x";
        }
        if (i != --poly.coeffs_.rend()) {
            stream << " +";
        }
    }
    return stream;
}
