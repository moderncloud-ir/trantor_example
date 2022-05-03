cmake_minimum_required(VERSION 3.5)

project(trantor_tcp_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Trantor REQUIRED)

add_executable(${PROJECT_NAME}_client SimpleClientTest.cpp)
target_link_libraries(${PROJECT_NAME}_client
  Trantor::Trantor
  )

add_executable(${PROJECT_NAME}_server SimpleServerTest.cpp)


target_link_libraries(${PROJECT_NAME}_server
  Trantor::Trantor
  )
