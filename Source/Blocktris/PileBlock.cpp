#include "PileBlock.h"

sf::Texture PileBlock::sm_sfBlockTexture;

sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoord, int yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f LogicalCoordsToScreenCoords(float xLogicalCoord, float yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords) {
    return LogicalCoordsToScreenCoords(sfLogicalCoords.x, sfLogicalCoords.y);
}