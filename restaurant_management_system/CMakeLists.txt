cmake_minimum_required(VERSION 3.16)
project(RestaurantSystem)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(RestaurantSystem main.cpp)

target_link_libraries(RestaurantSystem Qt6::Widgets)

