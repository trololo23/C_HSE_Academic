#include "password.h"

bool ValidatePassword(const std::string& password) {
    size_t password_lower_bound = 8;
    size_t password_upper_bound = 14;
    size_t code_lower_bound = 33;
    size_t code_upper_bound = 126;
    bool lower_cast = false;
    bool upper_cast = false;
    bool digits = false;
    bool another = false;
    if ((password.size() < password_lower_bound) || (password.size() > password_upper_bound)) {
        return false;
    }
    for (auto letter : password) {
        int code = static_cast<int>(letter);
        if ((code < code_lower_bound) || (code > code_upper_bound)) {
            return false;
        }
        if (std::islower(letter)) {
            lower_cast = true;
        } else if (std::isupper(letter)) {
            upper_cast = true;
        } else if (std::isdigit(letter)) {
            digits = true;
        } else {
            another = true;
        }
    }
    return lower_cast + upper_cast + digits + another >= 3;
}
