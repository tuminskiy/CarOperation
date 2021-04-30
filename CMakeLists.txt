cmake_minimum_required(VERSION 3.15)

project(CarOperation LANGUAGES CXX)

if (WIN32)
  set(EXEC_FLAG WIN32)
endif()

set(BIN_NAME carop)
add_executable(${BIN_NAME} ${EXEC_FLAG})


set_target_properties(${BIN_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  AUTOMOC ON
  AUTOUIC ON
  AUTOUIC_SEARCH_PATHS ui

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release
)

add_compile_options(-Wall -Werror -Wextra -Wpedantic)


set(SRC main.cpp
  include/window/connectiondialog.hpp   src/connectiondialog.cpp
  include/window/mainwindow.hpp         src/mainwindow.cpp
  include/window/queriesdialog.hpp      src/queriesdialog.cpp
  include/window/selecttabledialog.hpp  src/selecttabledialog.cpp
  
  src/query.cpp
)


find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

include_directories(include)

target_sources(${BIN_NAME} PRIVATE ${SRC})

target_link_libraries(${BIN_NAME} PRIVATE Qt5::Widgets Qt5::Sql)