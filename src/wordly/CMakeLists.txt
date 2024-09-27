# CMake configuration for the Wordly target

add_executable(wordly
    main.cpp
    src/wordly.cpp
    src/player.cpp
)

target_include_directories(wordly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})