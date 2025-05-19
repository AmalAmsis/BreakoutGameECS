cmake_minimum_required(VERSION 3.16)
project(BAGEL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# קבצים של המשחק
add_executable(BAGEL
        main.cpp
        bagel.h
        bagel_cfg.h
        breakoutGame/breakout_game.h
        breakoutGame/breakout_game.cpp
)

# קבצי טסט
add_executable(tests
        breakoutGame/test_firstPart.cpp
        breakoutGame/breakout_game.cpp
        breakoutGame/breakout_game.h
        bagel.h
        bagel_cfg.h
)

# --- SDL3 and SDL3_image ---
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(lib/SDL)
add_subdirectory(lib/SDL_image)

target_include_directories(BAGEL PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/SDL/include
        ${CMAKE_SOURCE_DIR}/lib/SDL_image/include
)

target_link_libraries(BAGEL PRIVATE
        SDL3-static
        SDL3_image-static
)

# --- Box2D ---
set(BUILD_SHARED_LIBS OFF)
set(BOX2D_SAMPLES OFF)
set(BOX2D_BENCHMARKS OFF)
set(BOX2D_DOCS OFF)
set(BOX2D_PROFILE OFF)
set(BOX2D_VALIDATE OFF)
set(BOX2D_UNIT_TESTS OFF)
add_subdirectory(lib/box2d)

target_include_directories(BAGEL PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/box2d/include
)

target_link_libraries(BAGEL PRIVATE box2d)

# --- Copy resource folder after build ---
add_custom_command(
        TARGET BAGEL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E
        copy_directory_if_different
        "${PROJECT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:BAGEL>/res"
)
