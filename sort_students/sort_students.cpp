#include "sort_students.h"
#include <tuple>

auto TieMembersDate(const Student& student) {
    return std::tie(student.birth_date.year, student.birth_date.month,
                    student.birth_date.day, student.last_name, student.name);
}

auto TieMembersName(const Student& student) {
    return std::tie(student.last_name, student.name, student.birth_date.year,
                    student.birth_date.month, student.birth_date.day);
}

bool SortByDate(const Student& lhs, const Student& rhs) {
    return TieMembersDate(lhs) < TieMembersDate(rhs);
}

bool SortByName(const Student& lhs, const Student& rhs) {
    return TieMembersName(lhs) < TieMembersName(rhs);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Name) {
        std::sort(students.begin(), students.end(), SortByName);
    } else {
        std::sort(students.begin(), students.end(), SortByDate);
    }
}
