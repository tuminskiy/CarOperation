cmake_minimum_required(VERSION 3.15)

project(CarOperation LANGUAGES CXX)

add_compile_options(-Wall -Werror -Wextra -Wpedantic)


set(BIN_NAME carop)

add_executable(${BIN_NAME})


set_target_properties(${BIN_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  AUTOMOC ON
  AUTOUIC ON
  AUTOUIC_SEARCH_PATHS ui

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release
)


set(SRC main.cpp)


find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

target_include_directories(${BIN_NAME} INTERFACE include)

target_sources(${BIN_NAME} PRIVATE ${SRC})

target_link_libraries(${BIN_NAME} PRIVATE Qt5::Widgets Qt5::Sql)