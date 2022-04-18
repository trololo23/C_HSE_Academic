#include "search.h"
#include "math.h"
#include <cctype>
#include <string>

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    if (!build_called_) {
        return {};
    }

    std::set<std::string> unique_words;
    std::string buffer;
    size_t rows_number = words_indexes_.size();
    std::unordered_map<std::string, size_t> word_rows_total_counter;
    std::unordered_map<std::string, size_t> row_words_local_counter;
    std::vector<Row> rows_relevance;
    std::vector<std::string_view> found_rows;

    for (auto letter : query) {
        if (std::isalpha(letter)) {
            buffer += std::tolower(letter);
        } else {
            unique_words.insert(buffer);
            buffer.clear();
        }
    }
    unique_words.insert(buffer);
    buffer.clear();

    for (const auto& row : words_indexes_) {
        for (const auto& [start, end] : row) {
            std::string word = std::string(text_.substr(start + 1, end - start - 1));
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (unique_words.find(word) != unique_words.end()) {
                ++row_words_local_counter[word];
            }
        }
        for (const auto& unique_word : unique_words) {
            if (row_words_local_counter[unique_word] > 0) {
                ++word_rows_total_counter[unique_word];
            }
        }
        row_words_local_counter.clear();
    }

    size_t row_it = 0;
    for (const auto& row : words_indexes_) {
        size_t words_count = 0;
        for (const auto& [i_1, i_2] : row) {
            ++words_count;
            std::string word = std::string(text_.substr(i_1 + 1, i_2 - i_1 - 1));
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (unique_words.find(word) != unique_words.end()) {
                ++row_words_local_counter[word];
            }
        }

        double sum_of_tf_idf = 0;
        for (const auto& word : unique_words) {
            if (row_words_local_counter[word] > 0 && word_rows_total_counter[word] > 0) {
                double tf = row_words_local_counter[word] / static_cast<double>(words_count);
                double idf = std::log(rows_number / static_cast<double>(word_rows_total_counter[word]));
                sum_of_tf_idf += tf * idf;
                row_words_local_counter[word] = 0;
            }
        }
        rows_relevance.push_back({sum_of_tf_idf, rows_ind_len_[row_it].first, rows_ind_len_[row_it].second});
        row_words_local_counter.clear();
        ++row_it;
    }

    std::stable_sort(rows_relevance.begin(), rows_relevance.end());
    for (size_t i = 0; i < std::min(rows_relevance.size(), results_count); ++i) {
        if (rows_relevance[i].relevance == 0) {
            break;
        }
        found_rows.push_back(text_.substr(rows_relevance[i].begin_index, rows_relevance[i].length));
    }

    return found_rows;
}

void SearchEngine::BuildIndex(std::string_view text) {
    build_called_ = true;
    text_ = text;
    words_indexes_.clear();
    rows_ind_len_.clear();
    std::vector<std::pair<size_t, size_t>> current_string;

    size_t word_id = -1;
    size_t row_id = -1;
    for (size_t i = 0; i <= text_.size(); ++i) {
        if (i == text_.size() || !std::isalpha(text_[i])) {
            if (!text_.substr(word_id + 1, i - word_id - 1).empty()) {
                current_string.emplace_back(word_id, i);
            }
            word_id = i;
            if (i == text_.size() || text_[i] == '\n') {
                if (!current_string.empty()) {
                    rows_ind_len_.push_back({row_id + 1, i - row_id - 1});
                    words_indexes_.push_back(current_string);
                }
                current_string.clear();
                row_id = i;
            }
        }
    }
}
