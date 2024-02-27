#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <memory>
#include "tile.h"

#define CHUNK_SIZE 16

namespace WFC {

class Chunk
{
public:
    Chunk(std::vector<std::pair<int, unsigned int>> tilesStates);
    template <typename T> requires std::uniform_random_bit_generator<T> void collapse(T &gen);
    void contrainTile(int x, int y, std::vector<int> states);
    std::shared_ptr<Tile> operator[](size_t x, size_t y);
    std::shared_ptr<Tile> operator[](std::pair<size_t, size_t> coords);
    std::shared_ptr<Tile> getTile(size_t x ,size_t y);
private:
    std::array<std::array<std::shared_ptr<Tile>, CHUNK_SIZE>, CHUNK_SIZE> tiles;
    std::shared_ptr<Chunk> north_neighbour;
    std::shared_ptr<Chunk> south_neighbour;
    std::shared_ptr<Chunk> east_neighbour;
    std::shared_ptr<Chunk> west_neighbour;
    std::pair<size_t, size_t> findLowestEntropyTile();
};

}

#endif // CHUNK_H
