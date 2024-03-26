#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLDebugLogger>

class MapWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    using QOpenGLWidget::QOpenGLWidget;
    ~MapWidget();
    void setTileTextures(const std::vector<QImage>& textures);
    void updateWorldMap(const QImage& image);
    void updateTilesTextureIndices(const std::vector<std::vector<int>> &indices);

signals:
    void clicked();
    void ready();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void updateTilesTextureIndicesVbo();

    std::vector<QImage> tile_textures;

    QColor clear_color = Qt::black;

    QMatrix4x4 world_projection;

    std::array<std::array<GLfloat, 512>, 512> tiles_texture_indices;

    int x = 0;
    int y = 0;

    QOpenGLShaderProgram *world_map_program = nullptr;
    QOpenGLVertexArrayObject world_map_vao;
    QOpenGLBuffer world_map_vbo;
    QOpenGLTexture *world_map_texture;

    QOpenGLShaderProgram *world_program = nullptr;
    QOpenGLVertexArrayObject world_vao;
    QOpenGLBuffer tile_vertices_vbo;
    QOpenGLBuffer tile_instances_offsets_vbo;
    QOpenGLBuffer tile_instances_texture_indices_vbo;
    QOpenGLTexture *tiles_texture_array;

    bool render_world_map = true;

    void paintWorldMap();
    void paintWorld();
};

#endif // MAPWIDGET_H
