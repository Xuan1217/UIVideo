cmake_minimum_required(VERSION 3.23)
project(Player)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Player
        buttons.cpp
        buttons.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        player.cpp
        player.h
        the_button.cpp
        the_button.h
        the_player.cpp
        the_player.h
        videowidget.cpp
        videowidget.h
        widget.cpp
        widget.h)
