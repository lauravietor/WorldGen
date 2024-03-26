#include <cmath>
#include <stdexcept>
#include "wavefunctioncollapse/tile.h"

namespace WFC {

Tile::Tile(std::vector<std::pair<int, unsigned int>> possible_states) : possible_states(possible_states)
{
    if (this->possible_states.size() == 0) {
        throw std::logic_error("Tile initialized with no possible state.");
    } else if (this->possible_states.size() == 1)
        this->state = possible_states[0].first;
    else
        this->state = TILE_UNCOLLAPSED;
    this->total_weight = 0;
    for (auto [state, weight] : this->possible_states) {
        this->total_weight += weight;
    }
    this->dirty = true;
}

template <typename T>
requires std::uniform_random_bit_generator<T>
int Tile::collapse(T &gen)
{
    std::uniform_int_distribution<unsigned int> distrib{1, this->total_weight};
    int val = distrib(gen);

    for (auto [state, weight]: possible_states) {
        if (val <= weight) {
            this->state = state;
            break;
        }
        val -= weight;
    }

    return state;
}

int Tile::collapse(std::mt19937_64 &gen)
{
    std::uniform_int_distribution<unsigned int> distrib{1, this->total_weight};
    int val = distrib(gen);

    for (auto [state, weight]: possible_states) {
        if (val <= weight) {
            this->state = state;
            break;
        }
        val -= weight;
    }


    return state;
}

void Tile::constrain(std::unordered_set<int> &states) {
    std::erase_if(this->possible_states, [this, states](auto state) {
        if (!states.contains(state.first)) {
            this->total_weight -= state.second;
            return true;
        }
        return false;
    });
}

double Tile::entropy() {
    if (!this->dirty)
        return this->_entropy;

    // entropy = -sum(p * log2(p)) where p = weigth for the state / total_weight and sum(weight) = total_weight
    //         = -sum(weight/total * log2(weight/total))
    //         = (sum(-weight * log2(weight)) / total + log2(total)
    double entropy = 0;

    // sum(-weight * log(weight))
    for (auto [state, weight] : this->possible_states) {
        entropy -= static_cast<double>(weight) * log2(weight);
    }
    // (sum(-weight * log2(weight)) / total + log2(total)
    entropy = (entropy / static_cast<double>(this->total_weight)) + log2(this->total_weight);

    this->_entropy = entropy;
    this->dirty = false;

    return entropy;
}

void Tile::reset(std::vector<std::pair<int, unsigned int>> possible_states) {
    this->possible_states = possible_states;
    if (this->possible_states.size() == 0) {
        throw std::logic_error("Tile reinitialized with no possible state.");
    } else if (this->possible_states.size() == 1)
        this->state = possible_states[0].first;
    else
        this->state = TILE_UNCOLLAPSED;
    this->total_weight = 0;
    for (auto [state, weight] : possible_states) {
        this->total_weight += weight;
    }
    this->dirty = true;
}

}
