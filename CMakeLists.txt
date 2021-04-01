project(Rtype)
cmake_minimum_required(VERSION 2.8.12)
add_definitions("-fPIC" "-g" "-ljack")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_INCLUDE_PATH})

add_executable(r-type_server Server/main.cpp Server/Lobby.cpp Server/Room.cpp Boost/Tcp/Server/TcpServer.cpp Boost/Tcp/Server/Session.cpp Boost/Udp/Server/UdpServer.cpp Shared/Packet.cpp)
target_link_libraries(r-type_server ${CONAN_LIBS})

add_executable(r-type_client Client/main.cpp Client/Game.cpp Boost/Tcp/Client/TcpClient.cpp Boost/Udp/Client/UdpClient.cpp Shared/Packet.cpp)
target_link_libraries(r-type_client ${CONAN_LIBS})

