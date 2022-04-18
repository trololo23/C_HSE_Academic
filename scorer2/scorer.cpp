#include "scorer.h"
#include "iostream"

void Scorer::AddToTable(const StudentName &student_name, const TaskName &task_name) {
    if (students_tasks_status_[student_name][task_name].check_success == true &&
        students_tasks_status_[student_name][task_name].merge_closed == true) {
        score_table_[student_name].insert(task_name);
    }
}

void Scorer::RemoveFromTable(const StudentName &student_name, const TaskName &task_name) {
    if (students_tasks_status_[student_name][task_name].check_success != true ||
        students_tasks_status_[student_name][task_name].merge_closed != true) {
        score_table_[student_name].erase(task_name);
        if (score_table_[student_name].empty()) {
            score_table_.erase(student_name);
        }
    }
}

void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    students_tasks_status_[student_name][task_name].check_success = false;
    RemoveFromTable(student_name, task_name);
}

void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    students_tasks_status_[student_name][task_name].check_success = true;
    AddToTable(student_name, task_name);
}

void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    students_tasks_status_[student_name][task_name].merge_closed = true;
    AddToTable(student_name, task_name);
}

void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    students_tasks_status_[student_name][task_name].merge_closed = false;
    RemoveFromTable(student_name, task_name);
}

void Scorer::Reset() {
    students_tasks_status_.clear();
    score_table_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    return score_table_;
}
