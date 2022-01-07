#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <tuple>

std::map<int, uint64_t> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Nope\n";
        exit(1);
    }

    std::map<int, uint64_t> crabs;

    int  value;
    char separator;

    file >> value;
    ++crabs[value];
    while (file >> separator >> value)
        ++crabs[value];

    file.close();

    return crabs;
}

std::tuple<int, unsigned> brute_force(std::map<int, uint64_t>& crabs, std::function<int(int)> cost_fn)
{
    auto min_index = crabs.begin()->first;
    auto max_index = crabs.rbegin()->first;

    int best_idx   = std::numeric_limits<int>::max();
    int lower_cost = std::numeric_limits<int>::max();

    for (auto idx = min_index; idx <= max_index; ++idx)
    {
        auto cost = 0;
        for (auto loop = min_index; loop <= max_index; ++loop)
            cost += crabs[loop] * cost_fn(loop - idx);
        if (cost < lower_cost)
        {
            lower_cost = cost;
            best_idx   = idx;
        }
    }
    return {best_idx, lower_cost};
}

int cost_fn_part1(int in)
{
    return std::abs(in);
}

int cost_fn_part2(int in)
{
    return std::abs(in) * (std::abs(in) + 1) / 2;
}

int main()
{
    auto crabs = read_file("day_07.txt");

    auto [part1_idx, part1_cost] = brute_force(crabs, cost_fn_part1);
    std::cout << "Part1: " << part1_idx << " " << part1_cost << std::endl;

    auto [part2_idx, part2_cost] = brute_force(crabs, cost_fn_part2);
    std::cout << "Part2: " << part2_idx << " " << part2_cost << std::endl;

    return 0;
}
