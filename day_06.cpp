#include <numeric>
#include <iostream>
#include <fstream>
#include <map>

int main()
{
    constexpr int DAYS = 256;

    std::ifstream file("day_06.txt");
    if ( !file )
    {
        std::cerr << "Nope\n";
        return 1;
    }

    std::map<uint8_t, uint64_t> fishes;

    int value;
    char separator;

    file >> value;
    ++fishes[value];
    while ( file >> separator >> value )
        ++fishes[value];
    file.close();


    for (auto day = 1; day <= DAYS; ++day)
    {
        auto count = fishes[0];
        for (auto timer = 0; timer < 8; ++timer)
            fishes[timer] = fishes[timer+1];
        fishes[6] += count;
        fishes[8]  = count;
    }
    std::cout << "Response: " << std::accumulate(fishes.begin(), fishes.end(), uint64_t(0),
                                              [](const uint64_t& acc, const std::pair<uint8_t, uint64_t>& p)
                                              { return acc + p.second; })
                              << std::endl;


    return 0;
}
