cmake_minimum_required(VERSION 3.27)
project(sfml)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR /home/iceloca/CLionProjects/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML 2.6.1 COMPONENTS graphics audio)
add_executable(game main.cpp
        server/client.cpp
        server/client.h
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
        entity_managment/SceneNode.h
        entity_managment/SceneNode.cpp
        levelDesign/Level.h
        levelDesign/Level.cpp
        levelDesign/MapTile.h
        levelDesign/MapTile.cpp
        input_handler/InputHandler.cpp
        input_handler/InputHandler.h
        command/Command.cpp
        command/Command.h
        command/CommandQueue.cpp
        command/CommandQueue.h
        entity_managment/Entity/Entity.cpp
        input_handler/InputHandler.cpp
        levelDesign/Tile.h
        levelDesign/Tile.cpp
        entity_managment/Bullet/Bullet.cpp
        states/LobbyState.h
        states/LobbyState.cpp
        states/HostState.cpp
        entity_managment/PickUp/Pickup.cpp
        entity_managment/PickUp/Pickup.h
        entity_managment/Data.cpp
        entity_managment/Data.h
        player_managment/Heart.cpp
        player_managment/Heart.h
        states/IngameMenuState.cpp
        entity_managment/Boss.cpp
)
target_link_libraries(game sfml-graphics sfml-audio)

set(CMAKE_C_STANDARD 11)
add_executable(server server/server.c
        server/server_structures.h)
target_link_libraries(server m)