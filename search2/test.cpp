#include <catch.hpp>

#include "search.h"

TEST_CASE("Search") {


    const std::string_view text =
        "Lorem Ipsum is simply dummy text\n"
        "of the printing and typesetting industry.\n"
        "Lorem Ipsum has been the industry's standard\n"
        "dummy text ever since the 1500s, when an unknown\n"
        "printer took a galley of type and scrambled it\n"
        "to make a type specimen book. It has survived\n"
        "not only five centuries, but also the leap into\n"
        "electronic typesetting, remaining essentially\n"
        "unchanged. It was popularised in the 1960s with\n"
        "the release of Letraset sheets containing Lorem\n"
        "psum passages, and more recently with desktop\n"
        "publishing software like Aldus PageMaker\n"
        "including versions of Lorem Ipsum.\n";

    const std::string_view text_1 =
            "............Lorem Ipsum is simply dummy text\n"
            "of the printing and typesetting industry.\n"
            "Lorem Ipsum has been the industry's standard\n"
            "dummy text ever since the 1500s, when an unknown\n"
            "printer took a galley of type and scrambled it\n"
            "to make a type specimen book. It has survived\n"
            "not only five centuries, but also the leap into\n"
            "electronic typesetting, remaining essentially\n"
            "unchanged. It was popularised in the 1960s with...............\n"
            "\n"
            "psum passages, and more recently with desktop....................................\n"
            "\n"
            "\n";

    SearchEngine search_engine;
    search_engine.BuildIndex(text);
    const std::string_view query = "typesetting release";
    const std::vector<std::string_view> expected_1 = {"electronic typesetting, remaining essentially"};
    REQUIRE(expected_1 == search_engine.Search(query, 1));
    search_engine.BuildIndex(text_1);
    REQUIRE(expected_1 == search_engine.Search(query, 1));
}

TEST_CASE("Search1") {

    SearchEngine search_engine;
    const std::string_view query = "typesetting release";
    const std::vector<std::string_view> expected = {};
    REQUIRE(expected == search_engine.Search(query, 1));
}

TEST_CASE("Vzlom_ochka") {
    const std::string_view text =
        "............Lorem Ipsum is simply dummy text\n"
        "of the printing and typesetting industry.\n"
        "Lorem Ipsum has been the industry's standard\n"
        "dummy text ever since the 1500s, when an unknown\n"
        "printer took a galley of type and scrambled it\n"
        "to make a type specimen book. It has survived\n"
        "not only five centuries, but also the leap into\n"
        "electronic typesetting, remaining essentially\n"
        "unchanged. It was popularised in the 1960s with...............\n"
        "\n"
        "psum passages, and more recently with desktop....................................\n"
        "\n"
        "\n";

    const std::string_view query = "typesetting release";
    const std::vector<std::string_view> expected = {};
    SearchEngine search_engine;
    REQUIRE(expected == search_engine.Search(query, 1));
    REQUIRE(expected == search_engine.Search(query, 100));
    REQUIRE(expected == search_engine.Search(query, 0));
    search_engine.BuildIndex(text);
    const std::vector<std::string_view> expected_1 = {"electronic typesetting, remaining essentially"};
    REQUIRE(expected_1 == search_engine.Search(query, 1));
    search_engine.BuildIndex(text);
    REQUIRE(expected == search_engine.Search(query, 0));
}

TEST_CASE("anal_research") {
    const std::string_view text = "\n";
    SearchEngine search_engine;
    search_engine.BuildIndex(text);
    const std::string_view query = "typesetting release";
    const std::vector<std::string_view> expected = {};
    REQUIRE(expected == search_engine.Search(query, 1));
}