add_subdirectory(Blocktris)
add_subdirectory(GameApp)

add_executable(Blocktris Main.cpp)

target_link_libraries(Blocktris GameApp BlocktrisApp sfml-window sfml-audio sfml-system sfml-graphics)

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/../Assets
                ${CMAKE_CURRENT_BINARY_DIR}/../Assets)
