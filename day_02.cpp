#include <iostream>
#include <fstream>
#include <string>
#include <map>

int main()
{
    enum direction {forward=1, down=2, up=3};

    // create a map of direction and register possible values
    std::map<std::string, direction> directions;
    directions["forward"] = forward;
    directions["down"]    = down;
    directions["up"]      = up;

    std::ifstream file("day_02.txt");
    if (!file)
    {
        std::cerr << "Nope\n";
        return 1;
    }

    std::string instruction;
    int value;

    int horizontal = 0;
    int depth      = 0;
    int aim        = 0;

    while ( file >> instruction >> value )
    {
        switch (directions[instruction])
        {
            case forward:
                horizontal += value;
                depth += aim*value;
                break;
            case down:
                aim += value;
                break;
            case up:
                aim -= value;
                break;
        }
    }
    file.close();
    std::cout << horizontal*depth << std::endl;
    return 0;
}
