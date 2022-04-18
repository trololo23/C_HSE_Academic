#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

struct TaskStatus {
    bool merge_closed = true;
    bool check_success = false;
};

class Scorer {
public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();
    void AddToTable(const StudentName& student_name, const TaskName& task_name);
    void RemoveFromTable(const StudentName& student_name, const TaskName& task_name);

    ScoreTable GetScoreTable() const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, TaskStatus>> students_tasks_status_;
    ScoreTable score_table_;
};
