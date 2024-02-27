#ifndef TILE_H
#define TILE_H

#include <random>
#include <utility>

#define TILE_UNCOLLAPSED -1

namespace WFC {

class Tile
{
public:
    Tile(std::vector<std::pair<int, unsigned int>> possible_states);
    template <typename T> requires std::uniform_random_bit_generator<T> void collapse(T &gen);
    double entropy();
    int getState() { return state; };
private:
    std::vector<std::pair<int, unsigned int>> possible_states;
    unsigned int total_weight;
    int state;
    double _entropy;
    bool dirty;
};

}

#endif // TILE_H
