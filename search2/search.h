#pragma once

#include <string_view>
#include <vector>
#include "set"
#include "unordered_map"

struct Row {
    double relevance;
    size_t begin_index;
    size_t length;

    bool operator<(const Row& row_2) const {
        return relevance > row_2.relevance;
    }
};

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;

private:
    std::string_view text_;
    std::vector<std::vector<std::pair<size_t, size_t>>> words_indexes_;
    std::vector<std::pair<size_t, size_t>> rows_ind_len_;
    bool build_called_ = false;
};
