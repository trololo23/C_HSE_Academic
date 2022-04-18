#include "tests_checking.h"
#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<const StudentAction*> tests_heap;
    std::vector<std::string> last_names_list;
    for (const auto& action : student_actions) {
        if (action.side == Side::Top) {
            tests_heap.push_front(&action);
        } else {
            tests_heap.push_back(&action);
        }
    }
    for (const auto& pos : queries) {
        last_names_list.push_back(tests_heap[pos - 1]->name);
    }
    return last_names_list;
}
