#include "worldgenerator.h"
#include "simplexnoise2d.h"
#include <iostream>

WorldGenerator::WorldGenerator(int seed, const std::vector<WFC::State>& states, const std::vector<Biome> &biomes_list):
    states(states),
    biomes_list(biomes_list),
    rng(seed),
    world_map(512, 512, QImage::Format::Format_RGBA8888)
{
    world.reserve(512);
    for (int i = 0; i < 512; i++) {
        world.emplace_back();
        world[i].reserve(512);
    }

    biomes.reserve(512);
    for (int i = 0; i < 512; i++) {
        biomes.emplace_back();
        biomes[i].reserve(512);
        for (int j = 0; j < 512; j++) {
            biomes[i].push_back(biomes_list[0]);
        }
    }

    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++) {
            std::vector<std::pair<int, unsigned int>> p_s = std::vector{std::pair<int, unsigned int>(0, 1)};
            world[i].emplace_back(p_s);
        }
    }
}

void WorldGenerator::setSeed(int seed) {
    rng.seed(seed);
}

void WorldGenerator::generate() {
    SimplexNoise2D noise1(rng);
    SimplexNoise2D noise2(rng);
    SimplexNoise2D noise3(rng);
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++) {
            std::vector<std::pair<int, unsigned int>> possible_states;
            double value = 80 * noise1.sample(static_cast<double>(i)/300, static_cast<double>(j)/300);
            value += 40 * noise2.sample(static_cast<double>(i)/150, static_cast<double>(j)/150);
            value += 20 * noise1.sample(static_cast<double>(i)/75, static_cast<double>(j)/75);
            if (value < -1.) value = -1.;
            if (value > 1.) value = 1.;
            for (Biome& biome: biomes_list) {
                if (biome.min_height <= value && value <= biome.max_height) {
                    world_map.setPixelColor(i, 511-j, biome.color);
                    biomes[i][j] = biome;
                    for (int state_id: biome.tiles) {
                        possible_states.push_back(std::pair(state_id, 1));
                    }
                    break;
                }
            }
            world[i][j].reset(possible_states);
        }
    }

    for (int n = 0; n < 512 * 512; n++) {
        if (n % 10000 == 0) std::cout << n << std::endl;
        std::pair min_entropy_coords = std::pair(0, 0);
        double min_entropy = std::numeric_limits<double>::infinity();
        for (int i = 0; i < 512; i++) {
            for (int j = 0; j < 512; j++) {
                double entropy = world[i][j].entropy();
                if (world[i][j].state == TILE_UNCOLLAPSED && entropy < min_entropy) {
                    min_entropy = entropy;
                    min_entropy_coords = std::pair(i, j);
                }
            }
        }
        auto [i, j] = min_entropy_coords;
        int state_id = world[i][j].collapse(rng);
        WFC::State state = states[state_id];
        if (0 < i) {
            world[i-1][j].constrain(state.possible_neighbours);
        }
        if (0 < j) {
            world[i][j-1].constrain(state.possible_neighbours);
        }
        if (i < 511) {
            world[i+1][j].constrain(state.possible_neighbours);
        }
        if (j < 511) {
            world[i][j+1].constrain(state.possible_neighbours);
        }
    }
}

QImage WorldGenerator::getWorldMap() {
    return world_map;
}

std::vector<std::vector<int>> WorldGenerator::getWorld() {
    std::vector<std::vector<int>> tiles_states;
    tiles_states.reserve(512);
    for (int i = 0; i < 512; i++) {
        tiles_states.emplace_back();
        tiles_states[i].reserve(512);
        for (int j = 0; j < 512; j++) {
            tiles_states[i].push_back(world[i][j].state);
        }
    }

    return tiles_states;
}
