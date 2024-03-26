#ifndef TILE_H
#define TILE_H

#include <random>
#include <unordered_set>
#include <utility>

#define TILE_UNCOLLAPSED -1

namespace WFC {

class Tile
{
public:
    Tile(std::vector<std::pair<int, unsigned int>> possible_states);
    template <typename T> requires std::uniform_random_bit_generator<T> int collapse(T &gen);
    int collapse(std::mt19937_64 &gen);
    void constrain(std::unordered_set<int>& states);
    double entropy();
    int state;
    void reset(std::vector<std::pair<int, unsigned int>> possible_states);
private:
    std::vector<std::pair<int, unsigned int>> possible_states;
    unsigned int total_weight;
    double _entropy;
    bool dirty;
};

}

#endif // TILE_H
