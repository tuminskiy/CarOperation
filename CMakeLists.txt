cmake_minimum_required(VERSION 3.15)

project(CarOperation LANGUAGES CXX)

if (WIN32)
  # set(EXEC_FLAG WIN32)
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

set(WINDOW_SRC
  include/window/connectiondialog.hpp   src/window/connectiondialog.cpp
  include/window/mainwindow.hpp         src/window/mainwindow.cpp
  include/window/queriesdialog.hpp      src/window/queriesdialog.cpp
  include/window/selecttabledialog.hpp  src/window/selecttabledialog.cpp
  include/window/selectiddialog.hpp     src/window/selectiddialog.cpp
  include/window/countdialog.hpp        src/window/countdialog.cpp
  include/window/orderdialog.hpp        src/window/orderdialog.cpp
  include/window/startstationdialog.hpp src/window/startstationdialog.cpp
)

set(STORAGE_SRC src/storage/query.cpp)

set(TABWORKER_SRC
  src/tabworker/basetabworker.cpp
  src/tabworker/routetabworker.cpp
  src/tabworker/routesheettabworker.cpp
  src/tabworker/bustabworker.cpp
  src/tabworker/drivertabworker.cpp
  src/tabworker/viewworker.cpp
)

set(SRC main.cpp
  ${WINDOW_SRC}
  ${STORAGE_SRC}
  ${TABWORKER_SRC}
)


find_package(Qt6 COMPONENTS Widgets Sql REQUIRED)

include_directories(include)

target_sources(${BIN_NAME} PRIVATE ${SRC})

target_link_libraries(${BIN_NAME} PRIVATE Qt6::Widgets Qt6::Sql)
