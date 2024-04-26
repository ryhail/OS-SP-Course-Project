cmake_minimum_required(VERSION 3.27)
project(sfml)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.6.1 COMPONENTS graphics audio REQUIRED)
add_executable(game main.cpp
        MainMenu.h
        MainMenu.cpp
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
        Application.cpp)
target_link_libraries(game sfml-graphics sfml-audio)