#include <cmath>
#include "wavefunctioncollapse/tile.h"

namespace WFC {

Tile::Tile(std::vector<std::pair<int, unsigned int>> possible_states) : possible_states(possible_states)
{
    if (this->possible_states.size() == 1)
        this->state = possible_states[0].first;
    else
        this->state = TILE_UNCOLLAPSED;
    for (auto state : possible_states) {
        this->total_weight += state.second;
    }
    this->dirty = true;
}

template <typename T>
requires std::uniform_random_bit_generator<T>
void Tile::collapse(T &gen)
{
    std::uniform_int_distribution<unsigned int> distrib{1, this->total_weight};
    int val = distrib(gen);

    for (auto state: possible_states) {
        if (val <= state.second) {
            this->state = state.first;
            break;
        }
        val -= state.second;
    }
}

double Tile::entropy() {
    if (!this->dirty)
        return this->_entropy;

    // entropy = -sum(p * log2(p)) where p = weigth for the state / total_weight and sum(weight) = total_weight
    //         = -sum(weight/total * log2(weight/total))
    //         = (sum(-weight * log2(weight)) / total + log2(total)
    double entropy = 0;

    // sum(-weight * log(weight))
    for (auto state : this->possible_states) {
        entropy -= static_cast<double>(state.second) * log2(state.second);
    }
    // (sum(-weight * log2(weight)) / total + log2(total)
    entropy = (entropy / static_cast<double>(this->total_weight)) + log2(this->total_weight);

    this->_entropy = entropy;
    this->dirty = false;

    return entropy;
}

}
