cmake_minimum_required(VERSION 3.26)

project(EasyDigipickSFSE VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# CommonLibSF provides SFSE and RE wrappers for Starfield plugins.
FetchContent_Declare(
  CommonLibSF
  GIT_REPOSITORY https://github.com/CommonLibSF/CommonLibSF.git
  GIT_TAG main
)
FetchContent_MakeAvailable(CommonLibSF)

add_commonlibsse_plugin(
  ${PROJECT_NAME}
  SOURCES
    src/main.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Output directly to repo Data/SFSE/Plugins so deployment is simple.
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Data/SFSE/Plugins"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Data/SFSE/Plugins"
)
