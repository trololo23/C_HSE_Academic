#include <catch.hpp>

#include "scorer.h"

TEST_CASE("Scorer") {
    Scorer scorer;

    scorer.OnCheckSuccess("Popov", "old");
    scorer.Reset();

    scorer.OnCheckSuccess("Ivanov", "utf8");
    scorer.OnCheckSuccess("Ivanov", "scorer");
    scorer.OnCheckSuccess("Petrov", "utf8");
    scorer.OnCheckFailed("Petrov", "scorer");
    scorer.OnCheckFailed("Sidorov", "scorer");

    const ScoreTable expected_result = {
        {"Ivanov", {"utf8", "scorer"}},
        {"Petrov", {"utf8"}},
    };

    REQUIRE(expected_result == scorer.GetScoreTable());
}

TEST_CASE("MyScorer") {
    Scorer scorer;

    scorer.OnCheckSuccess("Popov", "old");
    scorer.Reset();

    scorer.OnCheckSuccess("Ivanov", "utf8");
    scorer.OnCheckSuccess("Ivanov", "scorer");
    scorer.OnCheckSuccess("Petrov", "utf8");
    scorer.OnCheckFailed("Petrov", "scorer");
    scorer.OnCheckFailed("Sidorov", "scorer");
    scorer.OnCheckSuccess("Sidorov", "scorer");
    scorer.OnMergeRequestOpen("Sidorov", "scorer");
    scorer.OnMergeRequestClosed("Sidorov", "scorer");
    scorer.OnCheckSuccess("Sidorov", "bebra");

    const ScoreTable expected_result = {
        {"Ivanov", {"utf8", "scorer"}}, {"Petrov", {"utf8"}}, {"Sidorov", {"scorer", "bebra"}}};

    REQUIRE(expected_result == scorer.GetScoreTable());
}

TEST_CASE("MyScore") {
    Scorer scorer;

    scorer.OnCheckSuccess("Popov", "old");
    scorer.Reset();

    scorer.OnCheckSuccess("Ivanov", "utf8");
    scorer.OnCheckSuccess("Ivanov", "utf8");
    scorer.OnCheckSuccess("Ivanov", "utf8");

    const ScoreTable expected_result = {{"Ivanov", {"utf8"}}};

    REQUIRE(expected_result == scorer.GetScoreTable());
}
