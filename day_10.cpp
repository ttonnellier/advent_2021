#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

std::vector<std::string> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Nope\n";
        exit(1);
    }

    std::vector<std::string> nav_ss;

    std::string line;
    while (std::getline(file, line))
    {
        nav_ss.push_back(line);
    }

    file.close();

    return nav_ss;
}

bool is_opening(const char& c)
{
    return (c == '{' || c == '(' || c == '<' || c == '[');
}

int main()
{
    auto nav_ss = read_file("day_10.txt");

    const std::map<char, char> matching_char    = {{'{', '}'}, {'[', ']'}, {'<', '>'}, {'(', ')'}};
    const std::map<char, int>  score_corrupted  = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    const std::map<char, int>  score_incomplete = {{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};

    auto                  part1 = 0;
    std::vector<uint64_t> part2;

    for (const auto& line : nav_ss)
    {
        std::stack<char> open;
        bool             corrupted = false;
        for (auto c : line)
        {
            if (is_opening(c))
                open.push(c);
            else
            {
                auto expected = matching_char.at(open.top());
                open.pop();
                if (expected != c)
                {
                    part1 += score_corrupted.at(c);
                    corrupted = true;
                    break;
                }
            }
        }
        if (!corrupted)
        {
            uint64_t count = 0;
            while (!open.empty())
            {
                count *= 5;
                count += score_incomplete.at(matching_char.at(open.top()));
                open.pop();
            }
            part2.push_back(count);
        }
    }
    std::cout << "Part1: " << part1 << std::endl;

    std::sort(part2.begin(), part2.end());
    std::cout << "Part2: " << part2[part2.size() / 2] << std::endl;

    return 0;
}
