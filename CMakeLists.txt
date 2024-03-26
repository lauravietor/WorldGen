cmake_minimum_required(VERSION 3.5)

project(WorldGen VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/ui/mainwindow.cpp
        include/ui/mainwindow.h
        src/ui/mainwindow.ui
        src/simplexnoise2d.cpp
        include/simplexnoise2d.h
        src/wavefunctioncollapse/chunk.cpp
        src/wavefunctioncollapse/tile.cpp
        include/wavefunctioncollapse/chunk.h
        include/wavefunctioncollapse/tile.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WorldGen
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/wavefunctioncollapse/state.h src/wavefunctioncollapse/state.cpp
        include/ui/mapwidget.h src/ui/mapwidget.cpp
        assets.qrc
        include/worldgenerator.h src/worldgenerator.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WorldGen APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WorldGen SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WorldGen
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(WorldGen PRIVATE include/)
target_link_libraries(WorldGen PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

add_subdirectory(external/glm/)
target_link_libraries(WorldGen PRIVATE glm)

if (OPENGL_FOUND)
    target_include_directories(WorldGen PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(WorldGen PRIVATE ${OPENGL_gl_LIBRARY})
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WorldGen)
endif()
set_target_properties(WorldGen PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WorldGen
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WorldGen)
endif()
