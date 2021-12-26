#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <utility>
#include <stack>
#include <algorithm>
#include <set>

std::vector<std::vector<uint16_t>> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if ( !file )
    {
        std::cerr << "Nope\n";
        exit(1);
    }

    std::vector<std::vector<uint16_t>> grid;

    std::string line;
    while ( std::getline(file, line) )
    {
        grid.push_back(std::vector<uint16_t>());
        for (const auto& c: line)
            grid.back().push_back(c - '0');
    }

    file.close();

    return grid;
}

void flash (      std::vector<std::vector<uint16_t>> &grid,
                  std::stack< std::pair<int, int>>   &to_flash,
            const std::vector<std::pair<int, int>>   &valid_moves,
            const int                                 i,
            const int                                 j)
{
    for (const auto& move: valid_moves)
    {
        auto ii = i + move.first;
        auto jj = j + move.second;
        if ( ii >= static_cast<int>(grid.size()) || ii < 0 || jj >= static_cast<int>(grid[ii].size()) || jj < 0 )
            continue;

        ++grid[ii][jj];
        if (grid[ii][jj] > 9)
            to_flash.emplace(ii, jj);
    }
}

int step (std::vector<std::vector<uint16_t>>& grid, const std::vector<std::pair<int, int>>& valid_moves)
{
    std::set<std::pair<int, int>> flashed_coordinates;
    std::stack<std::pair<int, int>> to_flash;
    int flashes = 0;

    for (auto i = 0; i < static_cast<int>(grid.size()); ++i)
    {
        for (auto j = 0 ; j < static_cast<int>(grid[i].size()); ++j)
        {
            ++grid[i][j];
            if (grid[i][j] > 9)
            {
                flashed_coordinates.emplace(i, j);
                ++flashes;
                flash(grid, to_flash, valid_moves, i, j);
            }
        }
    }
    while ( !to_flash.empty() )
    {
        auto coordinates = to_flash.top();
        to_flash.pop();

        if ( flashed_coordinates.find(coordinates) != flashed_coordinates.end() )
            continue;

        flashed_coordinates.emplace(coordinates);
        ++flashes;
        flash(grid, to_flash, valid_moves, coordinates.first, coordinates.second);
    }

    for (auto& line: grid)
        std::replace_if(line.begin(), line.end(), [](auto i){ return i > 9; }, 0);

    return flashes;
}

int main()
{
    auto grid = read_file("day_11.txt");
    const std::vector<std::pair<int,int>> valid_moves { {-1,0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    uint32_t total_flashes = 0;
    for (auto i = 1; i <= 100; ++i)
        total_flashes += step(grid, valid_moves);
    std::cout << "Part1: " << total_flashes << std::endl;

    uint32_t steps = 0;
    while ( std::accumulate(grid.cbegin(), grid.cend(), 0,
                            [](auto lhs, const auto& rhs) { return std::accumulate(rhs.cbegin(), rhs.cend(), lhs); }
                           )
                        != 0 )
    {
        ++steps;
        step(grid, valid_moves);
    }
    std::cout << "Part2: " << steps+100 << std::endl;
    return 0;
}
