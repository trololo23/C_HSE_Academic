#include "admission.h"
#include <vector>
#include <tuple>
#include "unordered_map"

auto TieStudentDate(const Student* student) {
    return std::tie(student->birth_date.year, student->birth_date.month, student->birth_date.day);
}

bool CompareStudents(const Student* student_1, const Student* student_2) {
    if (student_1->name != student_2->name) {
        return student_1->name < student_2->name;
    } else {
        return TieStudentDate(student_1) < TieStudentDate(student_2);
    }
}

bool CompareApplicants(const Applicant* applicant_1, const Applicant* applicant_2) {
    if (applicant_1->points != applicant_2->points) {
        return applicant_1->points >= applicant_2->points;
    } else if (TieStudentDate(&applicant_1->student) != TieStudentDate(&applicant_2->student)) {
        return TieStudentDate(&applicant_1->student) < TieStudentDate(&applicant_2->student);
    } else {
        return applicant_1->student.name < applicant_2->student.name;
    }
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> student_pointers;
    AdmissionTable result_table;
    std::unordered_map<std::string, size_t> students_counter;
    std::unordered_map<std::string, size_t> students_max_size;

    for (const University& university : universities) {
        students_counter[university.name] = 0;
        students_max_size[university.name] = university.max_students;
    }

    for (const auto& student : applicants) {
        student_pointers.push_back(&student);
    }

    std::sort(student_pointers.begin(), student_pointers.end(), CompareApplicants);

    for (const Applicant* it : student_pointers) {
        for (const auto& university : it->wish_list) {
            if (students_counter[university] < students_max_size[university]) {
                result_table[university].push_back(&it->student);
                ++students_counter[university];
                break;
            }
        }
    }

    for (auto& pair : result_table) {
        std::sort(pair.second.begin(), pair.second.end(), CompareStudents);
    }

    return result_table;
}
