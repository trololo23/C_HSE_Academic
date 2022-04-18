#pragma once

#include <string>
#include <random>
#include <queue>
#include <set>
#include "tuple"

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    struct MyCell {
        bool mine_ = false;
        bool flag_ = false;
        bool open_ = false;
        int near_mines_ = 0;

        bool operator==(const MyCell& other) const {
            return std::tie(mine_, flag_, open_, near_mines_) == std::tie(other.mine_, other.flag_, other.open_, other.near_mines_);
        }

        bool operator!=(const MyCell& other) const {
            return !(*this == other);
        }
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = Vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const Vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const Vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    GameStatus status_ = GameStatus::NOT_STARTED;
    Vector<Vector<MyCell>> game_table_;
    size_t mines_numb_ = 0;
    size_t height_ = 0;
    size_t width_ = 0;
    size_t closed_cells_count_ = 0;
    time_t start_ = time(nullptr);
    time_t end_ = time(nullptr);

    void TableGenerator();

    void AddToNeighbours(size_t x, size_t y);

    void VictoryCheck();

    bool BordCheck(size_t x, size_t y) const;
};
