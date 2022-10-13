add_subdirectory(PileBlock)
add_subdirectory(Tetrimino)
add_subdirectory(TetriminoAnimation)
add_subdirectory(VirtualBag)

add_library(
    BlocktrisApp
    Blocktris.cpp
    Blocktris.h
)

target_link_libraries(BlocktrisApp PileBlock TetriminoAnimation Tetrimino VirtualBag Blocktris)
