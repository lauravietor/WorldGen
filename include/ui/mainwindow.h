#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "worldgenerator.h"
#include "wavefunctioncollapse/state.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const std::vector<QImage>& tile_textures, const std::vector<WFC::State> states, const std::vector<Biome>& biomes, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void mapWidgetReady();
    void regenerateClicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<WorldGenerator> world_generator;
};
#endif // MAINWINDOW_H
