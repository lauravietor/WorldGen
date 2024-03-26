#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <memory>
#include "tile.h"
#include "state.h"

#define CHUNK_SIZE 16

namespace WFC {

class Chunk
{
public:
    Chunk(std::shared_ptr<std::vector<State>> all_states, std::vector<std::pair<int, unsigned int>> possible_tiles_states);
    template <typename T> requires std::uniform_random_bit_generator<T> bool collapse(T &gen);
    void constrainTile(int x, int y, std::vector<int> states);
    std::shared_ptr<Tile> operator[](size_t x, size_t y);
    std::shared_ptr<Tile> operator[](std::pair<size_t, size_t> coords);
    std::shared_ptr<Tile> getTile(size_t x ,size_t y);
    std::shared_ptr<Tile> getTile(std::pair<size_t, size_t> coords);
private:
    std::array<std::array<std::shared_ptr<Tile>, CHUNK_SIZE>, CHUNK_SIZE> tiles;
    std::shared_ptr<std::vector<State>> all_states;
    std::pair<size_t, size_t> findLowestEntropyTile();
};

}

#endif // CHUNK_H
