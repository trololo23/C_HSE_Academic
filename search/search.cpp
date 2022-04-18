#include "search.h"
#include "unordered_map"
#include "set"
#include "tuple"
#include "iostream"
#include "math.h"
#include "algorithm"

struct Row {
    double relevance;
    size_t begin_index;
    size_t length;

    bool operator<(const Row& row_2) const {
        return relevance > row_2.relevance;
    }
};

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::string buffer;
    std::unordered_map<std::string, size_t> word_rows_total_counter;
    std::unordered_map<std::string, size_t> row_words_local_counter;
    std::set<std::string> unique_words;
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

    size_t rows_number = 1;
    for (size_t i = 0; i < text.size(); ++i) {
        if (std::isalpha(text[i])) {
            buffer += std::tolower(text[i]);
        } else {
            if (unique_words.find(buffer) != unique_words.end()) {
                ++row_words_local_counter[buffer];
            }
            buffer.clear();
            if (text[i] == '\n') {
                ++rows_number;
                for (const auto& word : unique_words) {
                    if (row_words_local_counter[word] > 0) {
                        ++word_rows_total_counter[word];
                        row_words_local_counter[word] = 0;
                    }
                }
            }
        }
    }

    if (unique_words.find(buffer) != unique_words.end()) {
        ++row_words_local_counter[buffer];
    }
    buffer.clear();
    for (const auto& word : unique_words) {
        if (row_words_local_counter[word] > 0) {
            ++word_rows_total_counter[word];
            row_words_local_counter[word] = 0;
        }
    }

    std::vector<Row> rows_relevance;
    size_t begin_index = 0;
    size_t words_in_row = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        if (std::isalpha(text[i])) {
            buffer += tolower(text[i]);
        } else {
            if (unique_words.find(buffer) != unique_words.end()) {
                row_words_local_counter[buffer] += 1;
            }
            if (!buffer.empty()) {
                words_in_row += 1;
            }
            buffer.clear();
            if (text[i] == '\n') {
                double sum_of_tf_idf = 0;
                for (const auto& word : unique_words) {
                    if (row_words_local_counter[word] > 0 && word_rows_total_counter[word] > 0) {
                        double tf = row_words_local_counter[word] / static_cast<double>(words_in_row);
                        double idf = std::log(rows_number / static_cast<double>(word_rows_total_counter[word]));
                        sum_of_tf_idf += tf * idf;
                        row_words_local_counter[word] = 0;
                    }
                }
                rows_relevance.push_back({sum_of_tf_idf, begin_index, i - begin_index});
                begin_index = i + 1;
                words_in_row = 0;
            }
        }
    }

    double sum_of_tf_idf = 0;
    if (unique_words.find(buffer) != unique_words.end()) {
        row_words_local_counter[buffer] += 1;
    }
    if (!buffer.empty()) {
        words_in_row += 1;
    }
    for (const auto& word : unique_words) {
        if (row_words_local_counter[word] > 0 && word_rows_total_counter[word] > 0) {
            double tf = row_words_local_counter[word] / static_cast<double>(words_in_row);
            double idf = std::log(rows_number / static_cast<double>(word_rows_total_counter[word]));
            sum_of_tf_idf += tf * idf;
        }
    }
    rows_relevance.push_back({sum_of_tf_idf, begin_index, text.size() - begin_index});

    std::stable_sort(rows_relevance.begin(), rows_relevance.end());
    for (size_t i = 0; i < std::min(rows_relevance.size(), results_count); ++i) {
        if (rows_relevance[i].relevance == 0) {
            break;
        }
        found_rows.push_back(text.substr(rows_relevance[i].begin_index, rows_relevance[i].length));
    }

    return found_rows;
}
