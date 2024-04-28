cmake_minimum_required(VERSION 3.27)
project(sfml)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.6.1 COMPONENTS graphics audio REQUIRED)
add_executable(game main.cpp
        server/client.c
        TestServer.h
        TestServer.cpp
        UserInterface.h
        UserInterface.cpp
        ConnectToServer.h
        ConnectToServer.cpp
        states/StateStack.h
        states/State.h
        states/StateIdentifiers.h
        states/State.cpp
        states/StateStack.cpp
        Application.h
        Application.cpp
        resources/ResourceHolder.inl
        states/TitleState.h
        states/TitleState.cpp
        states/MenuState.h
        states/MenuState.cpp
        states/ConnectState.h
        states/ConnectState.cpp
        states/utility.h
        states/utility.cpp
        states/GameState.h
        states/GameState.cpp
        player_managment/player/Player.h
        player_managment/player/Player.cpp
        entity_managment/Bullet/Bullet.h
        entity_managment/sever_structures.h
        entity_managment/SceneNode.h
        entity_managment/SceneNode.cpp
        levelDesign/Level.h
        levelDesign/Level.cpp
        levelDesign/MapTile.h
        levelDesign/MapTile.cpp
)
target_link_libraries(game sfml-graphics sfml-audio)