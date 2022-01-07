#include <bitset>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

constexpr int PATTERNS = 10;
constexpr int OUTPUTS  = 4;
constexpr int LINES    = 1000;

void read_line(std::string& observation, std::vector<uint8_t>& result, const int size)
{
    int    idx = 0;
    size_t separator;

    while ((separator = observation.find(" ")) != std::string::npos)
    {
        std::string segments_on = observation.substr(0, separator);
        observation.erase(0, separator + 1);

        for (const auto c : segments_on)
            result[idx] += (1 << (c - 'a')); // c ranges up to g, we therefore shift up to 6
        ++idx;
    }
    for (const auto c : observation) // observation contains the last 7-segments observation
        result[idx] += (1 << (c - 'a'));
    assert(idx == size - 1);
}

std::tuple<std::vector<std::vector<uint8_t>>, std::vector<std::vector<uint8_t>>> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Nope\n";
        exit(1);
    }

    std::vector<std::vector<uint8_t>> patterns(0, std::vector<uint8_t>(PATTERNS, 0));
    std::vector<std::vector<uint8_t>> outputs(0, std::vector<uint8_t>(OUTPUTS, 0));

    patterns.reserve(LINES);
    outputs.reserve(LINES);

    std::string line;
    while (std::getline(file, line))
    {
        patterns.push_back(std::vector<uint8_t>(PATTERNS, 0));
        outputs.push_back(std::vector<uint8_t>(OUTPUTS, 0));

        size_t      separator = line.find(" | ");
        std::string pattern_s = line.substr(0, separator);
        std::string output_s  = line.substr(separator + 3, line.length());

        read_line(pattern_s, patterns.back(), PATTERNS);
        read_line(output_s, outputs.back(), OUTPUTS);
    }

    file.close();

    return {patterns, outputs};
}

std::map<uint8_t, uint8_t> pattern_analysis(std::vector<uint8_t>& pattern)
{
    // here, we simply count the  number of turned on segment to deduce the number.
    // masking with already found numbers is also used to refine the search.

    std::map<uint8_t, uint8_t> numbers;

    auto it      = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                           { return __builtin_popcount(i) == 2; });
    auto one     = *it;
    numbers[one] = 1;
    pattern.erase(it);

    it            = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 4; });
    auto four     = *it;
    numbers[four] = 4;
    pattern.erase(it);

    it             = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 3; });
    auto seven     = *it;
    numbers[seven] = 7;
    pattern.erase(it);

    it             = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 7; });
    auto eight     = *it;
    numbers[eight] = 8;
    pattern.erase(it);

    it = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 6; });
    while (((*it) & one) == one)
        it = std::find_if(it + 1, pattern.end(), [](uint8_t i)
                          { return __builtin_popcount(i) == 6; });
    auto six     = *it;
    numbers[six] = 6;
    pattern.erase(it);

    it = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 6; });
    while ((*it & four) != four)
        it = std::find_if(it + 1, pattern.end(), [](uint8_t i)
                          { return __builtin_popcount(i) == 6; });
    auto nine     = *it;
    numbers[nine] = 9;
    pattern.erase(it);

    it            = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 6; });
    auto zero     = *it;
    numbers[zero] = 0;
    pattern.erase(it);

    it = std::find_if(pattern.begin(), pattern.end(), [](uint8_t i)
                      { return __builtin_popcount(i) == 5; });
    while ((*it & seven) != seven)
        it = std::find_if(it + 1, pattern.end(), [](uint8_t i)
                          { return __builtin_popcount(i) == 5; });
    auto three     = *it;
    numbers[three] = 3;
    pattern.erase(it);

    it = pattern.begin();
    if (__builtin_popcount((*it) & nine) != 5)
        ++it;
    numbers[*it] = 5;
    pattern.erase(it);

    numbers[pattern.back()] = 2;

    return numbers;
}

int main()
{
    const auto [patterns, outputs] = read_file("day_08.txt");

    auto count = 0;
    for (const auto& line : outputs)
        for (const auto& number : line)
        {
            const auto pcount = __builtin_popcount(number);
            if (pcount == 2 || pcount == 4 || pcount == 3 || pcount == 7)
                ++count;
        }

    std::cout << "Part1: " << count << std::endl;

    uint64_t sum = 0;
    for (auto line = 0u; line < patterns.size(); line++)
    {
        auto       pattern = patterns[line];
        const auto numbers = pattern_analysis(pattern);

        const auto output = outputs[line];

        auto value = 0;
        for (const auto& i : output)
            value = value * 10 + numbers.at(i);

        sum += value;
    }
    std::cout << "Part2: " << sum << std::endl;

    return 0;
}
