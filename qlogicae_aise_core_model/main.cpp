#include "pch.hpp"

#include "main.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include <rapidcsv.h>

struct Position {
    int row;
    int col;
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

std::vector<Position> shortest_knight_path(Position start, Position end) {
    if (start == end) return { start };
    std::queue<std::pair<Position, std::vector<Position>>> q;
    bool visited[8][8] = { false };
    q.push({ start, { start } });
    visited[start.row][start.col] = true;

    std::vector<std::pair<int, int>> moves = {
        {2,1},{2,-1},{-2,1},{-2,-1},
        {1,2},{1,-2},{-1,2},{-1,-2}
    };

    while (!q.empty()) {
        auto [pos, path] = q.front(); q.pop();
        for (auto [dr, dc] : moves) {
            Position next{ pos.row + dr, pos.col + dc };
            if (next.row >= 0 && next.row < 8 && next.col >= 0 && next.col < 8 && !visited[next.row][next.col]) {
                visited[next.row][next.col] = true;
                auto new_path = path; new_path.push_back(next);
                if (next == end) return new_path;
                q.push({ next, new_path });
            }
        }
    }
    return {};
}

std::string path_to_string(const std::vector<Position>& path) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < path.size(); ++i) {
        oss << "(" << path[i].row << "," << path[i].col << ")";
        if (i + 1 < path.size()) oss << ",";
    }
    oss << "]";
    return oss.str();
}

int main(int, char**) {
    rapidcsv::Document doc("", rapidcsv::LabelParams(0, -1));
    doc.SetColumnName(0, "start_row");
    doc.SetColumnName(1, "start_col");
    doc.SetColumnName(2, "target_row");
    doc.SetColumnName(3, "target_col");
    doc.SetColumnName(4, "path");

    int rowIndex = 0;
    for (int sr = 0; sr < 8; sr++) {
        for (int sc = 0; sc < 8; sc++) {
            for (int tr = 0; tr < 8; tr++) {
                for (int tc = 0; tc < 8; tc++) {
                    Position start{ sr, sc };
                    Position target{ tr, tc };
                    auto path = shortest_knight_path(start, target);
                    std::string path_str = path_to_string(path);
                    std::vector<std::string> row = {
                        std::to_string(sr),
                        std::to_string(sc),
                        std::to_string(tr),
                        std::to_string(tc),
                        path_str
                    };
                    doc.SetRow<std::string>(rowIndex++, row);
                }
            }
        }
    }

    doc.Save("training_dataset.csv");
    std::cout << "Generated training_dataset.csv\n";
    return 0;
}

namespace QLogicaeKnightVisionAiModelTrainer
{

}
