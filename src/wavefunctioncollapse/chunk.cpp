#include "wavefunctioncollapse/chunk.h"

namespace WFC {

Chunk::Chunk(std::vector<std::pair<int, unsigned int>> tilesStates)
{
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            this->tiles[i][j] = std::make_shared<Tile>(tilesStates);
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

std::pair<size_t, size_t> Chunk::findLowestEntropyTile() {
    std::pair current_min = std::make_pair(0, 0);
    double min_entropy = (*this)[current_min]->entropy();
    for (size_t x = 0; x < CHUNK_SIZE; x++) {
        for (size_t y = 0; y < CHUNK_SIZE; y++) {
            if (double entropy = (*this)[x, y]->entropy() < min_entropy) {
                current_min = std::make_pair(x, y);
                min_entropy = entropy;
            }
        }
    }
    return current_min;
}

template <typename T>
requires std::uniform_random_bit_generator<T>
void Chunk::collapse(T &gen) {
    std::pair coords = findLowestEntropyTile();
    std::shared_ptr<Tile> LETile = (*this)[coords];
    LETile->collapse(gen);
    // TODO: update possible states on neighbouring tiles
}
}
