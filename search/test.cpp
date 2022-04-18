#include <catch.hpp>

#include "search.h"

TEST_CASE("Search") {
    std::string_view text =
        "Lorem Ipsum is, typesetting\n"
        "of the printing and typesetting industry.\n"
        "Lorem Ipsum has been the industry's standard\n"
        "dummy text ever since the 1500s, when an unknown\n"
        "printer took a galley of type and scrambled it\n"
        "to make a type specimen book. It has survived\n"
        "not only five centuries, but also the leap into\n"
        "electronic typesetting, remaining essentially\n"
        "unchanged.typesetting It was\n"
        "the release of Letraset sheets containing Lorem\n"
        "psum passages, and more recently with desktop\n"
        "publishing software like Aldus PageMaker\n"
        "including versions     Ipsum,,,, .. .. typesetting";

    std::string_view query = "typesetting typesetting typesetting";
    std::vector<std::string_view> expected = {"Lorem Ipsum is, typesetting", "electronic typesetting, remaining essentially", "unchanged.typesetting It was"};

    REQUIRE(expected == Search(text, query, 3));
}

TEST_CASE("Help") {
    std::string_view text =
            "beb askjfas\n"
            "beb fmjdksa\n"
            "";

    std::string_view query = "beb some bruh";
    std::vector<std::string_view> expected = {"beb askjfas", "beb fmjdksa"};

    REQUIRE(expected == Search(text, query, 5));
}