#include "ui/mainwindow.h"

#include "wavefunctioncollapse/state.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFile>
#include <QSurfaceFormat>

#include <iostream>
#include <unordered_set>

std::vector<Biome> loadBiomes() {
    std::vector<Biome> biomes;
    QFile file(":/assets/biomes.json");

    if(!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Couldn't read biomes json");
    }

    const QByteArray biomes_data = file.readAll();

    const QJsonDocument biomes_json = QJsonDocument::fromJson(biomes_data);

    const QJsonArray biomes_array = biomes_json.array();

    for(const QJsonValue& biome: biomes_array){
        const QJsonObject biome_object = biome.toObject();
        Biome b;
        b.id = biome_object["id"].toInt();
        b.name = biome_object["name"].toString().toStdString();

        const QJsonArray height = biome_object["height"].toArray();
        b.min_height = height[0].toDouble();
        b.max_height = height[1].toDouble();

        const QJsonArray color = biome_object["color"].toArray();
        b.color = QColor(color[0].toInt(), color[1].toInt(), color[2].toInt());

        const QJsonArray tiles = biome_object["tiles"].toArray();
        for (const QJsonValue& tile_id: tiles) {
            b.tiles.insert(tile_id.toInt());
        }

        const QJsonArray border_tiles = biome_object["tiles"].toArray();
        for (const QJsonValue& tile_id: border_tiles) {
            b.border_tiles.insert(tile_id.toInt());
        }

        biomes.push_back(b);
    };

    return biomes;
}

std::pair<std::vector<QImage>, std::vector<WFC::State>> loadTiles(const std::vector<Biome>& biomes) {
    std::vector<QImage> textures;
    std::vector<WFC::State> tiles;
    QFile file(":/assets/tiles.json");

    if(!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Couldn't read tiles json");
    }

    const QByteArray tiles_data = file.readAll();

    const QJsonDocument tiles_json = QJsonDocument::fromJson(tiles_data);

    const QJsonArray tiles_array = tiles_json.array();

    for(const QJsonValue& tile: tiles_array){
        const QJsonObject tile_object = tile.toObject();
        int id = tile_object["id"].toInt();
        std::string name = tile_object["name"].toString().toStdString();
        std::unordered_set<int> possible_neighbours;
        for (const Biome& biome: biomes) {
            if (biome.tiles.contains(id)) {
                for (int tile_id: biome.tiles) {
                    possible_neighbours.insert(tile_id);
                }
            } else {
                for (int tile_id: biome.border_tiles) {
                    possible_neighbours.insert(tile_id);
                }
            }
        }
        tiles.emplace_back(id, name, possible_neighbours);
        QString texture_name = tile_object["texture"].toString();
        // We mirror the QImage because it gets mirrored automatically when we create an OpenGL texture
        textures.push_back(QImage(QString(":/assets/textures/").append(texture_name))
                           .convertToFormat(QImage::Format::Format_RGBA8888)
                           );
    }

    return std::pair(textures, tiles);
}

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);
    std::vector<Biome> biomes = loadBiomes();
    auto [textures, tiles] = loadTiles(biomes);
    MainWindow w(textures, tiles, biomes);
    w.show();
    return a.exec();
}
