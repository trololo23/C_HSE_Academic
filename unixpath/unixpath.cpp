#include "unixpath.h"

#include "deque"

auto Split(const std::string_view& string, std::deque<std::string_view>& split_commands) {
    auto i = std::find(string.begin(), string.end(), '/');
    if (i == string.end()) {
        split_commands.push_back(string);
    } else {
        int it_1 = -1;
        for (size_t j = 0; j < string.size(); ++j) {
            if (string[j] == '/') {
                split_commands.push_back(string.substr(it_1 + 1, j - it_1 - 1));
                it_1 = j;
            }
        };
        if (string.back() != '/') {
            split_commands.push_back(string.substr(it_1 + 1, string.size() - it_1 - 1));
        }
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::deque<std::string_view> absolute_path_split;
    std::deque<std::string_view> path_split;
    std::string answer;

    if (path[0] == '/') {
        Split(path, path_split);
        for (auto sbstr : path_split) {
            if (sbstr != "." && !sbstr.empty()) {
                answer += '/';
                answer += sbstr;
            }
        }
    } else {
        Split(current_working_dir, absolute_path_split);
        Split(path, path_split);

        for (auto sbstr : path_split) {
            if (sbstr == "..") {
                if (!absolute_path_split.empty()) {
                    absolute_path_split.pop_back();
                }
            } else if (!sbstr.empty() && sbstr != ".") {
                absolute_path_split.push_back(sbstr);
            }
        }

        for (auto sbstr : absolute_path_split) {
            if (sbstr != "." && !sbstr.empty()) {
                answer += '/';
                answer += sbstr;
            }
        }
    }

    if (answer.empty()) {
        answer = "/";
    }

    return answer;
}
