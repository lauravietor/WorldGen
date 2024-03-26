#include "wavefunctioncollapse/chunk.h"
#include <stdexcept>

namespace WFC {

Chunk::Chunk(std::shared_ptr<std::vector<State>> all_states, std::vector<std::pair<int, unsigned int>> possible_tiles_states)
{
    this->all_states = all_states;

    if(possible_tiles_states.size() == 0) {
        throw std::logic_error("Chunk initialized with no possible state for its tiles.");
    }
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            this->tiles[i][j] = std::make_shared<Tile>(possible_tiles_states);
        }
    }
}

std::shared_ptr<Tile> Chunk::operator[](size_t x, size_t y) {
    return this->tiles[x][y];
}

std::shared_ptr<Tile> Chunk::operator[](std::pair<size_t, size_t> coords) {
    return this->tiles[coords.first][coords.second];
}

std::shared_ptr<Tile> Chunk::getTile(size_t x ,size_t y) {
    return this->tiles[x][y];
}

std::shared_ptr<Tile> Chunk::getTile(std::pair<size_t, size_t> coords) {
    return this->tiles[coords.first][coords.second];
}

std::pair<size_t, size_t> Chunk::findLowestEntropyTile() {
    std::pair current_min = std::make_pair(CHUNK_SIZE, CHUNK_SIZE); // invalid tile index, sentinel value
    double min_entropy = std::numeric_limits<double>::infinity();

    for (size_t x = 0; x < CHUNK_SIZE; x++) {
        for (size_t y = 0; y < CHUNK_SIZE; y++) {
            double entropy = this->getTile(x, y)->entropy();
            if (this->getTile(x, y)->state == TILE_UNCOLLAPSED && entropy < min_entropy) {
                current_min = std::make_pair(x, y);
                min_entropy = entropy;
            }
        }
    }
    return current_min;
}

template <typename T>
requires std::uniform_random_bit_generator<T>
bool Chunk::collapse(T &gen) {
    std::pair coords = findLowestEntropyTile();
    if(coords == std::pair<size_t, size_t>(CHUNK_SIZE, CHUNK_SIZE)) {
        return false;
    }
    std::shared_ptr<Tile> LETile = this->getTile(coords);
    LETile->collapse(gen);
    State new_state = all_states->at(LETile->state);
    LETile->constrain(new_state.possible_neighbours);
    return true;
}
}
