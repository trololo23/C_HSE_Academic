#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    for (size_t index_1 = 0; index_1 < table.size(); ++index_1) {
        for (size_t index_2 = 0; index_2 < table.size(); ++index_2) {
            for (size_t index_3 = 0; index_3 < table.size(); ++index_3) {
                size_t lhs = table[table[index_1][index_2]][index_3];
                size_t rhs = table[index_1][table[index_2][index_3]];
                if (lhs != rhs) {
                    return false;
                }
            }
        }
    }

    return true;
}
