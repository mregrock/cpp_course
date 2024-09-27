# CMake configuration for the SearchingTree target

add_executable(searching_tree
    main.cpp
    src/searching_tree.cpp
)

target_include_directories(searching_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})