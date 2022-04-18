#include "unixpath.h"

UnixPath::UnixPath(std::string_view initial_dir) : initial_dir_(initial_dir), abs_path_(initial_dir) {
    Split(initial_dir_, init_dir_deq_);
    NormalizeAbsPath(init_dir_deq_);
    abs_path_deq_ = init_dir_deq_;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    path_ = path;
    path_deq_.clear();
    Split(path_, path_deq_);
    if (path_[0] == '/') {
        AbsPathToStr();
    } else {
        RelPathToStr();
    }
}

void UnixPath::Split(const std::string_view& string, std::deque<std::string_view>& split_commands) {
    auto delimeter_position = std::find(string.begin(), string.end(), '/');
    if (delimeter_position == string.end()) {
        if (string != "." && !string.empty()) {
            split_commands.push_back(string);
        }
    } else {
        int start_pos = -1;
        for (size_t j = 0; j <= string.size(); ++j) {
            if (j == string.size() || string[j] == '/') {
                std::string_view dir = string.substr(start_pos + 1, j - start_pos - 1);
                if (dir == ".." && !split_commands.empty() && split_commands.back() != "..") {
                    split_commands.pop_back();
                } else if (dir != "." && !dir.empty()) {
                    split_commands.push_back(dir);
                }
                start_pos = j;
            }
        }
    }
}

void UnixPath::NormalizeAbsPath(std::deque<std::string_view>& path) {
    while (path.front() == "..") {
        path.pop_front();
    }
}

void UnixPath::RelPathFromInit() {
    rel_path_deq_.clear();
    size_t in_size = init_dir_deq_.size();
    for (size_t i = 0; i < init_dir_deq_.size(); ++i) {
        if (i == abs_path_deq_.size() || init_dir_deq_[i] != abs_path_deq_[i]) {
            in_size = i;
            break;
        }
    }

    for (size_t i = in_size; i < init_dir_deq_.size(); ++i) {
        rel_path_deq_.push_back("..");
    }

    for (size_t i = in_size; i < abs_path_deq_.size(); ++i) {
        rel_path_deq_.push_back(abs_path_deq_[i]);
    }
}

void UnixPath::RelPathToStr() {
    for (const auto& dir : path_deq_) {
        if (dir == "..") {
            if (!abs_path_deq_.empty()) {
                abs_path_deq_.pop_back();
            }
        } else {
            abs_path_deq_.push_back(dir);
        }
    }
    RelPathFromInit();
    DeqToStrRel();
    DeqToStrAbs();
}

void UnixPath::AbsPathToStr() {
    NormalizeAbsPath(path_deq_);
    abs_path_deq_ = path_deq_;
    RelPathFromInit();
    DeqToStrRel();
    DeqToStrAbs();
}

void UnixPath::DeqToStrAbs() {
    abs_path_.clear();
    for (const auto& dir : abs_path_deq_) {
        abs_path_ += '/';
        abs_path_ += dir;
    }
    if (abs_path_deq_.empty()) {
        abs_path_ += '/';
    }
}

void UnixPath::DeqToStrRel() {
    rel_path_.clear();
    if (rel_path_deq_.empty()) {
        rel_path_ = ".";
    } else {
        if (rel_path_deq_[0] != "..") {
            rel_path_ += "./";
        }
        for (const auto& dir : rel_path_deq_) {
            rel_path_ += dir;
            rel_path_ += '/';
        }
        rel_path_.pop_back();
    }
}

std::string UnixPath::GetAbsolutePath() const {
    return abs_path_;
}

std::string UnixPath::GetRelativePath() const {
    return rel_path_;
}
