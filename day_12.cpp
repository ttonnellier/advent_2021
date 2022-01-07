#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

std::multimap<std::string, std::string> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Nope\n";
        exit(1);
    }

    std::multimap<std::string, std::string> cave_system;

    std::string line;
    while (std::getline(file, line))
    {
        auto        separator = line.find("-");
        std::string cave1     = line.substr(0, separator);
        std::string cave2     = line.substr(separator + 1, line.size());

        cave_system.insert(std::pair<std::string, std::string>(cave1, cave2));
        cave_system.insert(std::pair<std::string, std::string>(cave2, cave1));
    }

    file.close();

    return cave_system;
}

bool is_allowed_p1(const std::string& cave, const std::vector<std::string>& path)
{
    if (std::all_of(cave.begin(), cave.end(), [](auto c)
                    { return std::isupper(c); }))
        return true;
    if (std::find(path.begin(), path.end(), cave) == path.end())
        return true;
    return false;
}

bool is_allowed_p2(const std::string& cave, const std::vector<std::string>& path)
{
    if (std::all_of(cave.begin(), cave.end(), [](auto c)
                    { return std::isupper(c); }))
        return true; // it's a big cave, go!
    if (std::find(path.begin(), path.end(), cave) == path.end())
        return true; // never used small cave
    if (cave == "start" || cave == "end")
        return false;

    std::vector<std::string> small_caves_vector;
    std::set<std::string>    small_caves_set;
    for (auto& str : path)
    {
        if (std::all_of(str.begin(), str.end(), [](auto c)
                        { return std::islower(c); }))
        {
            small_caves_set.insert(str);
            small_caves_vector.push_back(str);
        }
    }
    if (small_caves_set.size() == small_caves_vector.size())
        return true; // no other duplicates

    return false;
}

std::vector<std::vector<std::string>> build_paths(const std::multimap<std::string, std::string>&                           cave_system,
                                                  std::function<bool(const std::string&, const std::vector<std::string>&)> is_allowed)
{
    // this function performs quite a lot of vectors copy..

    std::vector<std::vector<std::string>> paths_next;
    std::vector<std::vector<std::string>> paths;

    paths.push_back(std::vector<std::string>{"start"});

    bool at_least_one_allowed;
    do
    {
        at_least_one_allowed = false;
        for (const auto& path : paths)
        {
            auto last_elem = path.back();
            if (last_elem == "end")
            {
                paths_next.push_back(path);
                continue;
            }
            auto range = cave_system.equal_range(last_elem);

            for (auto i = range.first; i != range.second; ++i)
            {
                if (is_allowed(i->second, path))
                {
                    at_least_one_allowed = true;
                    paths_next.push_back(path);
                    paths_next.back().push_back(i->second);
                }
            }
        }
        paths = paths_next;
        paths_next.clear();
    } while (at_least_one_allowed);

    return paths;
}

template <class T, class idxT>
void remove_indices(std::vector<T>& container, const std::vector<idxT>& indices)
{
    // Exit early if there is nothing to do.
    if (indices.empty())
        return;

    auto destination = container.begin();
    auto source      = container.begin();
    auto idx_it      = indices.begin();

    // Skip until we reach the first index.
    std::advance(destination, *idx_it);
    std::advance(source, *idx_it + 1);

    // Delete all the elements at the given indices.
    while (++idx_it != indices.end())
    {
        auto nextDelete = container.begin() + *idx_it;
        while (source < nextDelete)
            *destination++ = std::move(*source++);
        source++;
    }

    // Copy the remainder until the end of the container.
    while (source != container.end())
        *destination++ = std::move(*source++);

    container.resize(container.size() - indices.size());
}

std::vector<std::vector<std::string>> build_paths_v2(const std::multimap<std::string, std::string>&                           cave_system,
                                                     std::function<bool(const std::string&, const std::vector<std::string>&)> is_allowed)
{
    // doing quite a lot of moves instead :)

    std::vector<std::vector<std::string>> paths;

    paths.push_back(std::vector<std::string>{"start"});

    bool at_least_one_allowed;
    do
    {
        at_least_one_allowed     = false;
        auto                size = paths.size();
        std::vector<size_t> to_erase;
        for (auto p = 0u; p < size; ++p)
        {
            bool will_be_erased = true;
            auto last_elem      = paths[p].back();
            if (last_elem == "end")
            {
                will_be_erased = false;
                continue;
            }
            auto range = cave_system.equal_range(last_elem);

            auto cur_path = paths[p];
            for (auto i = range.first; i != range.second; ++i)
            {
                if (is_allowed(i->second, cur_path))
                {
                    at_least_one_allowed = true;
                    if (will_be_erased)
                    {
                        paths[p].push_back(i->second);
                        will_be_erased = false;
                    }
                    else
                    {
                        paths.push_back(cur_path);
                        paths.back().push_back(i->second);
                    }
                }
            }
            if (will_be_erased)
                to_erase.push_back(p);
        }
        remove_indices(paths, to_erase);
    } while (at_least_one_allowed);

    return paths;
}
int main()
{
    auto cave_system = read_file("day_12.txt");

    std::cout << "Part1: " << build_paths_v2(cave_system, is_allowed_p1).size() << std::endl;
    std::cout << "Part2: " << build_paths_v2(cave_system, is_allowed_p2).size() << std::endl;

    return 0;
}
