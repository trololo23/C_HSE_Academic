#pragma once

#include <string>
#include <string_view>
#include <deque>

class UnixPath {
public:
    UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;

private:
    std::string_view initial_dir_;
    std::deque<std::string_view> init_dir_deq_;

    std::string_view path_;
    std::deque<std::string_view> path_deq_;

    std::string abs_path_;
    std::deque<std::string_view> abs_path_deq_;

    std::string rel_path_;
    std::deque<std::string_view> rel_path_deq_;

    static void Split(const std::string_view& string, std::deque<std::string_view>& split_commands);

    static void NormalizeAbsPath(std::deque<std::string_view>& path);

    void RelPathToStr();

    void AbsPathToStr();

    void DeqToStrAbs();

    void DeqToStrRel();

    void RelPathFromInit();
};
