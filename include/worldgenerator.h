#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <vector>
#include <random>
#include <string>
#include <QColor>
#include <QImage>
#include <unordered_set>

#include "wavefunctioncollapse/state.h"
#include "wavefunctioncollapse/tile.h"

struct Biome {
    unsigned int id;
    std::string name;
    double min_height;
    double max_height;
    QColor color;
    std::unordered_set<unsigned int> tiles;
    std::unordered_set<unsigned int> border_tiles;
};

class WorldGenerator
{
public:
    WorldGenerator(int seed, const std::vector<WFC::State>& states, const std::vector<Biome>& biomes);
    void setSeed(int seed);
    void generate();
    QImage getWorldMap();
    std::vector<std::vector<int>> getWorld();
private:
    std::vector<WFC::State> states;
    std::vector<Biome> biomes_list;
    QImage world_map;
    std::vector<std::vector<Biome>> biomes;
    std::vector<std::vector<WFC::Tile>> world;
    std::mt19937_64 rng;
};

#endif // WORLDGENERATOR_H
