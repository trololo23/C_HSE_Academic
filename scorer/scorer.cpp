#include "scorer.h"
#include <unordered_map>
#include "algorithm"
#include "iostream"
bool CompareTime(const Event* event_1, const Event* event_2) {
    return event_1->time < event_2->time;
}

struct TaskStatus {
    bool merge_closed = true;
    bool check_success = false;
};

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::unordered_map<std::string, std::unordered_map<std::string, TaskStatus>> students_tasks_status;
    std::vector<const Event*> events_pointers;
    ScoreTable score_table;

    for (const auto& event : events) {
        events_pointers.push_back(&event);
    }

    std::sort(events_pointers.begin(), events_pointers.end(), CompareTime);

    for (auto it : events_pointers) {
        if (it->time > score_time) {
            break;
        }
        if (it->event_type == EventType::CheckFailed) {
            students_tasks_status[it->student_name][it->task_name].check_success = false;
        } else if (it->event_type == EventType::CheckSuccess) {
            students_tasks_status[it->student_name][it->task_name].check_success = true;
        } else if (it->event_type == EventType::MergeRequestClosed) {
            students_tasks_status[it->student_name][it->task_name].merge_closed = true;
        } else if (it->event_type == EventType::MergeRequestOpen) {
            students_tasks_status[it->student_name][it->task_name].merge_closed = false;
        }
    }

    for (const auto& student : students_tasks_status) {
        for (const auto& task : student.second) {
            if (task.second.merge_closed == true && task.second.check_success == true) {
                score_table[student.first].insert(task.first);
            }
        }
    }

    return score_table;
}
