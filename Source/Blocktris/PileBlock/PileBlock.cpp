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

PileBlock::PileBlock() {
    std::vector<unsigned int> vAnim = { 0, 5, 0, 5, 0, 5, 0, 5, 0 };
    sf::IntRect sfBaseSprite = sf::IntRect(0, 0, SquareSize, SquareSize);
    m_ptaAnim = new TetriminoAnimation(vAnim, sfBaseSprite);
    m_ptaAnim->SetFrameDelay(9);
}
