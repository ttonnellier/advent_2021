#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

std::vector<std::vector<uint16_t>> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Nope\n";
        exit(1);
    }

    std::vector<std::vector<uint16_t>> grid;

    std::string line;
    while (std::getline(file, line))
    {
        grid.push_back(std::vector<uint16_t>());
        for (const auto& c : line)
            grid.back().push_back(c - '0');
    }

    file.close();

    return grid;
}

std::vector<std::pair<size_t, size_t>> find_low_points(const std::vector<std::vector<uint16_t>>& grid, const std::vector<std::pair<int, int>>& valid_moves)
{
    std::vector<std::pair<size_t, size_t>> coordinates;

    for (auto row = 0; row < static_cast<int>(grid.size()); ++row)
    {
        for (auto col = 0; col < static_cast<int>(grid[row].size()); ++col)
        {
            auto moves = 0;
            auto lt    = 0;
            for (const auto& v : valid_moves)
            {
                auto x = row + v.first;
                auto y = col + v.second;
                if (x >= static_cast<int>(grid.size()) || x < 0 || y >= static_cast<int>(grid[row].size()) || y < 0)
                    continue;
                ++moves;
                lt += (grid[row][col] < grid[x][y]);
            }
            if (lt == moves)
                coordinates.emplace_back(row, col);
        }
    }
    return coordinates;
}

int main()
{
    auto                                   grid = read_file("day_09.txt");
    const std::vector<std::pair<int, int>> valid_moves{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    const auto low_points = find_low_points(grid, valid_moves);

    auto sum = 0;
    for (const auto& point : low_points)
    {
        sum += (grid[point.first][point.second] + 1);
    }
    std::cout << "Part1: " << sum << std::endl;

    std::vector<int> bassin_sizes;
    for (const auto& point : low_points)
    {
        std::stack<std::tuple<int, int, uint16_t>> points_to_consider;
        points_to_consider.emplace(point.first, point.second, grid[point.first][point.second]);
        auto count = 1;
        //std::cout << "Init point: " << std::get<0>(point) << " " << std::get<1>(point) << std::endl;
        while (!points_to_consider.empty())
        {
            const auto cur_point = points_to_consider.top();
            const auto row       = std::get<0>(cur_point);
            const auto col       = std::get<1>(cur_point);
            const auto val       = std::get<2>(cur_point);
            //std::cout << "Cur point: " << row << " " << col << " " << val << " " << std::endl;
            points_to_consider.pop();
            for (const auto& move : valid_moves)
            {
                auto x = row + move.first;
                auto y = col + move.second;
                if (x >= static_cast<int>(grid.size()) || x < 0 || y >= static_cast<int>(grid[row].size()) || y < 0)
                    continue;
                if ((grid[x][y] > val) && (grid[x][y] != 9))
                {
                    //std::cout << "Point added: " << x << " " << y << " " << grid[x][y] << std::endl;
                    points_to_consider.emplace(x, y, grid[x][y]);
                    grid[x][y] = 9;
                    ++count;
                }
            }
        }
        bassin_sizes.push_back(count);
    }
    std::partial_sort(bassin_sizes.begin(), bassin_sizes.begin() + 3, bassin_sizes.end(), std::greater<>());
    std::cout << "Part2: " << bassin_sizes[0] * bassin_sizes[1] * bassin_sizes[2] << std::endl;

    return 0;
}
