#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

// we assume that a check is represented by -1;

bool bingo(const std::vector<std::vector<int>>& card)
{
    for (auto it = card.begin(); it != card.end(); ++it)
    {
        unsigned count = std::count_if(it->begin(), it->end(), [](const int& i)
                                       { return i == -1; });
        if (count == it->size())
            return true;
    }

    std::vector<std::vector<int>> transposed(card[0].size(), std::vector<int>(card.size()));
    for (auto i = 0u; i < card.size(); ++i)
        for (auto j = 0u; j < card[i].size(); ++j)
            transposed[j][i] = card[i][j];
    for (auto it = transposed.begin(); it != transposed.end(); ++it)
    {
        unsigned count = std::count_if(it->begin(), it->end(), [](const int& i)
                                       { return i == -1; });
        if (count == it->size())
            return true;
    }

    return false;
}

void cross(std::vector<std::vector<int>>& card, int number)
{
    for (auto it = card.begin(); it != card.end(); ++it)
        std::replace(it->begin(), it->end(), number, -1);
}

int score(std::vector<std::vector<int>>& card)
{
    auto count = 0;
    for (auto it = card.begin(); it != card.end(); ++it)
    {
        std::replace(it->begin(), it->end(), -1, 0);
        count += std::accumulate(it->begin(), it->end(), 0);
    }
    return count;
}

int main()
{
    constexpr int ROWS = 5;
    constexpr int COLS = 5;

    std::vector<std::vector<std::vector<int>>> cards;
    std::vector<std::vector<int>>              card(ROWS, std::vector<int>(COLS, 0));
    std::vector<int>                           numbers;

    std::ifstream file("day_04.txt");
    if (!file)
    {
        std::cerr << "Nope\n";
        return 1;
    }

    int         number;
    char        separator;
    std::string first_line;
    std::getline(file, first_line);
    std::stringstream ss(first_line);
    ss >> number;
    numbers.push_back(number);
    while (ss >> separator >> number)
        numbers.push_back(number);

    while (!file.eof())
    {
        for (auto& outer : card)
            for (auto& inner : outer)
                file >> inner;
        cards.push_back(card);
    }
    file.close();

    std::vector<std::vector<std::vector<int>>> cards_cpy = cards;
    for (auto num : numbers)
    {
        for (auto& card : cards)
        {
            cross(card, num);
            if (bingo(card))
            {
                std::cout << "Part1: " << num * score(card) << std::endl;
                goto part2;
            }
        }
    }

part2:
    int  i   = 0;
    auto num = 0;
    while (cards_cpy.size() > 1)
    {
        num = numbers[i++];
        for (auto& card : cards_cpy)
            cross(card, num);
        cards_cpy.erase(std::remove_if(cards_cpy.begin(), cards_cpy.end(), bingo), cards_cpy.end());
    }
    while (!bingo(cards_cpy[0]))
    {
        num = numbers[i++];
        cross(cards_cpy[0], num);
    }
    std::cout << "Part2: " << num * score(cards_cpy[0]) << std::endl;

    return 0;
}
