cmake_minimum_required(VERSION 3.5)

project(DungeonAdventure)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

set(HEADERS
    include/mainwindow.h
)

qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
