#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

int main()
{
    constexpr int NB_BITS   = 12;
    constexpr unsigned MASK = 0xFFF;

    std::ifstream file("day_03.txt");
    if ( !file )
    {
        std::cerr << "Nope\n";
        return 1;
    }

    std::string str_bin;
    std::vector<unsigned> counter(NB_BITS, 0);
    unsigned lines = 0;
    std::set<int> set;

    while ( file >> str_bin )
    {
        int value = std::stoi(str_bin, nullptr, 2);
        set.insert(value);
        ++lines;
        int idx = 0;
        while( value )
        {
            counter[idx] += value&1;
            value >>= 1;
            ++idx;
        }
    }
    file.close();

    unsigned gamma_rate = 0;
    for (auto rit = counter.rbegin(); rit != counter.rend(); ++rit)
        gamma_rate = ( gamma_rate << 1 ) | ( (*rit) > lines/2 );

    std::cout << "Part1: " << gamma_rate*((~gamma_rate)&MASK) << std::endl;

    auto set_cpy = set;
    unsigned oxygen = 0;
    int i = 0;
    while (set.size() > 1)
    {
        int value = 1 << (NB_BITS-1-i);
        auto lower_it          = set.lower_bound(oxygen + value);
        auto number_with_ones  = std::distance(lower_it, set.end());
        auto number_with_zeros = std::distance(set.begin(), lower_it);
        if ( number_with_ones >= number_with_zeros )
        {
            set.erase(set.begin(), lower_it);
            oxygen += value;
        }
        else
        {
            set.erase(lower_it, set.end());
        }
        ++i;
    }

    unsigned co2 = 0;
    i = 0;
    while (set_cpy.size() > 1)
    {
        int value = 1 << (NB_BITS-1-i);

        auto lower_it          = set_cpy.lower_bound(co2 + value);
        auto number_with_ones  = std::distance(lower_it, set_cpy.end());
        auto number_with_zeros = std::distance(set_cpy.begin(), lower_it);

        if ( number_with_ones >= number_with_zeros )
        {
            set_cpy.erase(lower_it, set_cpy.end());
        }
        else
        {
            set_cpy.erase(set_cpy.begin(), lower_it);
            co2 += value;
        }
        ++i;
    }

    oxygen = *set.begin();
    co2    = *set_cpy.begin();

    std::cout << "Part2: " << oxygen*co2 << std::endl;

    return 0;
}
