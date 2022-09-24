#include <iostream>
#include "Blocktris/Blocktris.h"

int main(int argc, char* argv) {
    srand(time(NULL));
    BlockTris blocktrisGame;
    return blocktrisGame.RunGame();
}