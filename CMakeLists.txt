# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(KingdomBuilder)

# Specify the path to vcpkg.cmake
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML components
find_package(SFML COMPONENTS system window graphics audio REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} main.cpp
        Game.cpp
        Game.h
        MainMenu.cpp
        MainMenu.h
        pch.h
        Button.cpp
        Button.h
        initGameStart.cpp
        initGameStart.h
        InteractiveButton.h
        inGame.cpp
        inGame.h
        pauseMenu.cpp
        pauseMenu.h
)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

# Copy DLLs to output directory (Windows only)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()