#include "ui/mapwidget.h"
#include <QMouseEvent>
#include <iostream>

MapWidget::~MapWidget() {
    makeCurrent();
    world_map_vbo.destroy();
    tile_vertices_vbo.destroy();
    tile_instances_offsets_vbo.destroy();
    tile_instances_texture_indices_vbo.destroy();

    delete world_map_program;
    delete world_program;
    doneCurrent();
}

void MapWidget::setTileTextures(const std::vector<QImage>& textures) {
    tile_textures = textures;
}

void MapWidget::updateWorldMap(const QImage& image) {
    makeCurrent();
    world_map_texture->destroy();
    world_map_texture->create();
    world_map_texture->setData(image);
}

void MapWidget::updateTilesTextureIndices(const std::vector<std::vector<int>> &indices) {
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++) {
            tiles_texture_indices[i][j] = static_cast<GLfloat>(indices[i][j]);
        }
    }
}

void MapWidget::updateTilesTextureIndicesVbo() {
    makeCurrent();
    std::vector<GLfloat> new_indices;
    new_indices.reserve(32*32);
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            std::cout << tiles_texture_indices[x+i][y+j] << std::endl;
            new_indices.push_back(tiles_texture_indices[x+i][y+j]);
        }
    }
    tile_instances_texture_indices_vbo.bind();
    tile_instances_texture_indices_vbo.write(0, new_indices.data(), 32*32*sizeof(GL_FLOAT));
}

void MapWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(clear_color.redF(), clear_color.greenF(), clear_color.blueF(), clear_color.alphaF());


    // OpenGL program for rendering the world map
    world_map_program = new QOpenGLShaderProgram;
    world_map_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/world_map.vert");
    world_map_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/world_map.frag");
    world_map_program->bindAttributeLocation("vertex", 0);
    world_map_program->link();

    world_map_program->bind();

    world_map_vao.create();
    QOpenGLVertexArrayObject::Binder world_vao_binder(&world_map_vao);

    world_map_vbo.create();
    world_map_vbo.bind();

    std::vector<GLfloat> world_map_vertices{
        -1., -1.,
         1., -1.,
        -1.,  1.,
        -1.,  1.,
         1., -1.,
         1.,  1.
    };
    world_map_vbo.allocate(world_map_vertices.data(), 2 * 6 * sizeof(GL_FLOAT)); // 2 floats per vertex, 6 vertices

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GL_FLOAT), nullptr);

    world_map_vbo.release();

    world_map_texture = new QOpenGLTexture(QImage(512, 512, QImage::Format::Format_RGB888).convertToFormat(QImage::Format::Format_RGBA8888));

    // OpenGL program for rendering the world
    world_program = new QOpenGLShaderProgram;
    world_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/world.vert");
    world_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/world.frag");
    world_program->bindAttributeLocation("vertex", 0);
    world_program->bindAttributeLocation("offset", 1);
    world_program->bindAttributeLocation("textureIndex", 2);
    world_program->link();

    world_program->bind();

    world_projection.setToIdentity();
    world_projection.ortho(QRect(0, 0, 32, 32));
    world_program->setUniformValue("mvp", world_projection);

    world_vao.create();
    QOpenGLVertexArrayObject::Binder world_binder(&world_vao);

    // Vertices for each instance of a tile
    std::vector<GLfloat> tile_vertices {
        0., 0.,
        1., 0.,
        0., 1.,
        0., 1.,
        1., 0.,
        1., 1.
    };

    tile_vertices_vbo.create();
    tile_vertices_vbo.bind();
    tile_vertices_vbo.allocate(tile_vertices.data(), 2 * 6 * sizeof(GL_FLOAT));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GL_FLOAT), nullptr);
    tile_vertices_vbo.release();

    // Offset for each tile instance
    std::vector<GLfloat> tile_offsets;
    for (int x = 0.; x < 32; x += 1.0) {
        for (int y = 0.; y < 32; y += 1.0) {
            tile_offsets.push_back(static_cast<GLfloat>(x));
            tile_offsets.push_back(static_cast<GLfloat>(y));
        }
    }
    tile_instances_offsets_vbo.create();
    tile_instances_offsets_vbo.bind();
    tile_instances_offsets_vbo.allocate(tile_offsets.data(), tile_offsets.size() * sizeof(GL_FLOAT));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GL_FLOAT), nullptr);
    glVertexAttribDivisor(1, 1);
    tile_instances_offsets_vbo.release();

    // Texture index for each tile instance
    tile_instances_texture_indices_vbo.create();
    tile_instances_texture_indices_vbo.bind();
    tile_instances_texture_indices_vbo.allocate(std::vector<GLfloat>(32*32, 0.0).data(), 32*32*sizeof(GL_FLOAT));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1*sizeof(GL_FLOAT), nullptr);
    glVertexAttribDivisor(2, 1);
    tile_instances_texture_indices_vbo.release();

    tiles_texture_array = new QOpenGLTexture(QOpenGLTexture::Target::Target2DArray);
    tiles_texture_array->setSize(16, 16);
    tiles_texture_array->setLayers(tile_textures.size());
    tiles_texture_array->setFormat(QOpenGLTexture::TextureFormat::RGBA8_UNorm);
    tiles_texture_array->setAutoMipMapGenerationEnabled(false);
    tiles_texture_array->setMagnificationFilter(QOpenGLTexture::Linear);
    tiles_texture_array->setMinificationFilter(QOpenGLTexture::Linear);
    tiles_texture_array->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);
    tiles_texture_array->allocateStorage(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::UInt8);

    for (int i = 0; i < tile_textures.size(); i++) {
        tiles_texture_array->setData(0, i, QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8, tile_textures[i].bits());
    }

    emit ready();
}

void MapWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (render_world_map) paintWorldMap();
    else paintWorld();
}

void MapWidget::paintWorldMap(){
    makeCurrent();
    QOpenGLVertexArrayObject::Binder world_vao_binder(&world_map_vao);
    world_map_program->bind();
    world_map_texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    world_map_texture->release();
    world_map_program->release();
    world_vao_binder.release();
};

void MapWidget::paintWorld(){
    makeCurrent();
    QOpenGLVertexArrayObject::Binder world_binder(&world_vao);
    updateTilesTextureIndicesVbo();
    world_program->bind();
    tiles_texture_array->bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 32*32);
    tiles_texture_array->release();
    world_program->release();
    world_binder.release();
};

void MapWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event) {
    render_world_map = !render_world_map;
    std::cout << "Clicked!" << std::endl;
    if (render_world_map) {
        std::cout << "Rendering world map" << std::endl;
    } else {
        std::cout << "Rendering world" << std::endl;
    }
    QPoint pos = event->pos();
    x = std::max(0, std::min(511 - 32, pos.x() - 16));
    y = std::max(0, std::min(511 - 32, pos.y() - 16));
    update();
    emit clicked();
}
