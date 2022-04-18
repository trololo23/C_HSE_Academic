#include "minesweeper.h"
#include "iostream"


Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count)
    :mines_numb_(mines_count), height_(height), width_(width) {
    game_table_ = Vector<Vector<MyCell>>(height);
    for (size_t i = 0; i < height; ++i) {
        game_table_[i].Reserve(width);
    }
    closed_cells_count_ = height_ * width_ - mines_count;
    TableGenerator();
}

void Minesweeper::TableGenerator() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, width_ * height_);

    Vector<std::pair<size_t, size_t>> positions;
    positions.Reserve(width_ * height_);

    for (size_t line = 0; line < height_; ++line) {
        for (size_t column = 0; column < width_; ++column) {
            positions.PushBack({line, column});
        }
    }

//    std::shuffle(positions.begin(), positions.end(), gen);
    Vector<std::pair<size_t, size_t>> result_list;
    int list_size = mines_numb_;
    for (size_t i = 0; i < mines_numb_; ++i) {
        size_t ind = gen() % list_size;
        result_list.PushBack(positions[ind]);
        std::swap(positions[ind], positions[list_size - 1]);
        --list_size;
    }

    for (size_t i = 0; i < mines_numb_; ++i) {
        game_table_[result_list[i].first][result_list[i].second].mine_ = true;
        AddToNeighbours(result_list[i].first, result_list[i].second);
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, const Vector<Cell>& cells_with_mines)
    : height_(height), width_(width) {
    game_table_ = Vector<Vector<MyCell>>(height);
    for (size_t i = 0; i < height; ++i) {
        game_table_[i] = Vector<MyCell>(width);
    }
    mines_numb_ = cells_with_mines.Size();
    closed_cells_count_ = height_ * width_ - mines_numb_;
    for (const auto& [x, y] : cells_with_mines) {
        game_table_[y][x].mine_ = true;
        AddToNeighbours(y, x);
    }
}

bool Minesweeper::BordCheck(size_t x, size_t y) const {
    return x < height_ && y < width_;
}

void Minesweeper::VictoryCheck() {
    if (closed_cells_count_ == 0) {
        status_ = GameStatus::VICTORY;
        end_ = time(nullptr);
    }
}

void Minesweeper::AddToNeighbours(size_t x, size_t y) {
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (BordCheck(x + i, y + j)) {
                ++game_table_[x + i][y + j].near_mines_;
            }
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    status_ = GameStatus::NOT_STARTED;
    game_table_.Clear();
    game_table_ = Vector<Vector<MyCell>>(height);
    for (size_t i = 0; i < height; ++i) {
        game_table_[i].Reserve(width);
    }
    height_ = height;
    width_ = width;
    mines_numb_ = mines_count;
    closed_cells_count_ = height_ * width_ - mines_count;
    TableGenerator();
}

void Minesweeper::NewGame(size_t width, size_t height, const Vector<Cell>& cells_with_mines) {
    status_ = GameStatus::NOT_STARTED;
    game_table_.Clear();
    game_table_ = Vector<Vector<MyCell>>(height);
    for (size_t i = 0; i < height; ++i) {
        game_table_[i].Reserve(width);
    }
    height_ = height;
    width_ = width;
    mines_numb_ = cells_with_mines.Size();
    closed_cells_count_ = height_ * width_ - mines_numb_;
    for (const auto& [x, y] : cells_with_mines) {
        game_table_[y][x].mine_ = true;
        AddToNeighbours(y, x);
    }
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (status_ != GameStatus::DEFEAT && status_ != GameStatus::VICTORY) {
        if (status_ == GameStatus::NOT_STARTED) {
            start_ = time(nullptr);
            status_ = GameStatus::IN_PROGRESS;
        }

        auto [y, x] = cell;

        if (!game_table_[x][y].flag_) {
            if (game_table_[x][y].mine_) {
                status_ = GameStatus::DEFEAT;
                end_ = time(nullptr);
            } else if (!game_table_[x][y].open_) {
                std::queue<std::pair<size_t, size_t>> cells;
                std::set<std::pair<size_t, size_t>> used_cells;
                cells.push({x, y});

                while (!cells.empty()) {
                    auto [l, c] = cells.front();
                    used_cells.insert({l, c});
                    cells.pop();
                    game_table_[l][c].open_ = true;
                    --closed_cells_count_;
                    if (game_table_[l][c].near_mines_ == 0) {
                        for (int i = -1; i < 2; ++i) {
                            for (int j = -1; j < 2; ++j) {
                                if (BordCheck(l + i, c + j) && !used_cells.contains({l + i, c + j}) &&
                                    !game_table_[l + i][c + j].mine_ && !game_table_[l + i][c + j].flag_ &&
                                    !game_table_[l + i][c + j].open_) {
                                    cells.push({l + i, c + j});
                                    used_cells.insert({l + i, c + j});
                                }
                            }
                        }
                    }
                }
                VictoryCheck();
            }
        }
    }
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (status_ != GameStatus::DEFEAT && status_ != GameStatus::VICTORY) {
        auto [y, x] = cell;
        if (!game_table_[x][y].open_) {
            if (game_table_[x][y].flag_) {
                game_table_[x][y].flag_ = false;
            } else {
                game_table_[x][y].flag_ = true;
            }
        }
    }
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return status_;
}

time_t Minesweeper::GetGameTime() const {
    if (status_ == GameStatus::NOT_STARTED) {
        return 0;
    } else if (status_ == GameStatus::IN_PROGRESS) {
        return time(nullptr) - start_;
    } else {
        return end_ - start_;
    }
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    Vector<std::string> rendered_field;
    if (status_ == GameStatus::DEFEAT) {
        for (const auto& row : game_table_) {
            std::string curr_row;
            for (const auto& cell : row) {
                if (cell.mine_) {
                    curr_row += '*';
                } else if (cell.near_mines_ == 0) {
                    curr_row += '.';
                } else {
                    curr_row += std::to_string(cell.near_mines_);
                }
            }
            rendered_field.PushBack(curr_row);
        }
    } else {
        for (const auto& row : game_table_) {
            std::string curr_row;
            for (const auto& cell : row) {
                if (cell.flag_) {
                    curr_row += '?';
                } else if (cell.open_) {
                    if (cell.near_mines_ == 0) {
                        curr_row += '.';
                    } else {
                        curr_row += std::to_string(cell.near_mines_);
                    }
                } else {
                    curr_row += '-';
                }
            }
            rendered_field.PushBack(curr_row);
        }
    }
    return rendered_field;
}
