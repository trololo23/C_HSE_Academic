#include "word2vec.h"
#include <vector>

int64_t Distance(const std::vector<int>* vec, const auto& vectors) {
    int64_t dist = 0;
    for (size_t i = 0; i < vec->size(); ++i) {
        dist += vec->at(i) * vectors[0][i];
    }
    return dist;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {

    std::vector<std::pair<const std::vector<int>*, const std::string*>> pointers_vector_word;
    std::vector<std::string> answer_list;

    for (size_t i = 0; i < vectors.size(); ++i) {
        pointers_vector_word.push_back({&vectors[i], &words[i]});
    }

    if (pointers_vector_word.size() < 2) {
        return {};
    }

    int64_t max_dist = Distance(pointers_vector_word[1].first, vectors);
    for (size_t i = 1; i < pointers_vector_word.size(); ++i) {
        int64_t local_dist = Distance(pointers_vector_word[i].first, vectors);
        if (local_dist > max_dist) {
            max_dist = local_dist;
        }
    }

    for (size_t i = 1; i < pointers_vector_word.size(); ++i) {
        if (Distance(pointers_vector_word[i].first, vectors) == max_dist) {
            answer_list.push_back(*pointers_vector_word[i].second);
        }
    }

    return answer_list;
}
