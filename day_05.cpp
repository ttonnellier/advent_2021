#include <numeric>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
    constexpr int SIZE = 1000;
    std::ifstream file("day_05.txt");
    if ( !file )
    {
        std::cerr << "Nope\n";
        return 1;
    }

    std::vector<std::vector<int>> map (SIZE, std::vector<int>(SIZE, 0));
    int x1,y1,x2,y2;
    char separator;
    while ( file >> x1 >> separator >> y1 >> separator >> separator >> x2 >> separator >> y2 )
    {
        if ( x1 == x2 )
            for (auto y = std::min(y1,y2);y <= std::max(y1,y2); ++y)
                map[x1][y] += 1;
        else if ( y1 == y2 )
            for (auto x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
                map[x][y1] += 1;
        else
        {
            // verify first it's a diagonal
            if (std::abs(x2-x1) != std::abs(y2-y1))
            {
                std::cerr << "Defuck?! " << "(" << x1  << "," << y1 << ") -> (" << x2  << "," << y2 << ")" << std::endl;
                continue;
            }
            int x_grows = (x2 > x1);
            int y_grows = (y2 > y1);

            int x = x1;
            int y = y1;
            map[x][y] += 1;
            while(true)
            {
                x += (2*x_grows -1);
                y += (2*y_grows -1);
                map[x][y] += 1;
                if ((x==x2) && (y==y2))
                    break;
            }
        }
    }
    file.close();

    int count = 0;
    for (const auto& row: map)
        count += std::count_if(row.begin(), row.end(), [](const int i){return i >= 2;});

    std::cout << "Part1: " << count << std::endl;

    return 0;
}
