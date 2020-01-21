cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

project(RPA_Server VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add sockets library
ExternalProject_Add(SocketLibrarySource
    GIT_REPOSITORY git@github.com:KyleGonzalez/Cpp-SocketLibrary.git
    CONFIGURE_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_DISCONNECTED TRUE
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make
)

ExternalProject_Get_Property(SocketLibrarySource SOURCE_DIR)
set(SOCKET_SOURCE ${SOURCE_DIR})

# JSON Parser
ExternalProject_Add(JSON
    GIT_REPOSITORY git@github.com:nlohmann/json.git
    GIT_TAG v3.7.3
    CMAKE_CACHE_ARGS
    -DJSON_BuildTests:BOOL=OFF
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:STRING=../../install)

ExternalProject_Get_Property(JSON INSTALL_DIR)
set(JSON_DIR ${INSTALL_DIR}/install/)

# Add executables to the cmake project
add_executable(RPA_Server
        GameSession/GameStates/CharacterCreation.cpp
        GameSession/GameStates/StateManager.cpp
        Server/Server.cpp
        Server/ClientController.cpp
        GameSession/Player.cpp
        GameSession/Game.cpp
        main.cpp)

add_dependencies(RPA_Server JSON SocketLibrarySource)

message("Configuring RPA Server with source directory: ${PROJECT_SOURCE_DIR}")

target_include_directories(RPA_Server PUBLIC ${SOCKET_SOURCE} "${JSON_DIR}/include")
target_link_libraries(RPA_Server PRIVATE ${SOCKET_SOURCE}/ServerSocket.o ${SOCKET_SOURCE}/Socket.o pthread bluetooth)


