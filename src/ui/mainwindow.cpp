#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const std::vector<QImage>& tile_textures, const std::vector<WFC::State> states, const std::vector<Biome>& biomes, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mapWidget->setTileTextures(tile_textures);
    connect(ui->mapWidget, &MapWidget::ready, this, &MainWindow::mapWidgetReady);
    connect(ui->regenerateButton, &QPushButton::clicked, this, &MainWindow::regenerateClicked);
    size_t seed = qHash(ui->seed->text());
    world_generator = std::make_unique<WorldGenerator>(seed, states, biomes);
    world_generator->generate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mapWidgetReady() {
    ui->mapWidget->updateWorldMap(world_generator->getWorldMap());
    ui->mapWidget->updateTilesTextureIndices(world_generator->getWorld());
}

void MainWindow::regenerateClicked() {
    world_generator->setSeed(qHash(ui->seed->text()));
    world_generator->generate();
    ui->mapWidget->updateWorldMap(world_generator->getWorldMap());
    ui->mapWidget->updateTilesTextureIndices(world_generator->getWorld());
    ui->mapWidget->update();
}

