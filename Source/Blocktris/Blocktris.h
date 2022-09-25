#ifndef BLOCKTRIS_H
#define BLOCKTRIS_H

#include <array>

#include "PileBlock.h"

#include "../GameApp/GameApp.h"

static constexpr float ScreenWidth = 800.0f;
static constexpr float ScreenHeight = 800.0f;

static constexpr float SquareSize = 35.0f;
static constexpr float SquareOutlineThickness = 4.0f;
static constexpr float TrueSquareSize = SquareSize - SquareOutlineThickness;

static constexpr float BoardSizeX = SquareSize * 10.0f;
static constexpr float BoardSizeY = SquareSize * 20.0f;

static constexpr float BoardOffsetX = 1.0f * ScreenWidth / 8.0f;
static constexpr float BoardOffsetY = (ScreenHeight - BoardSizeY) / 2.0f;


class BlockTris : public GameApp {
private:
    virtual bool OnInitialize() override;
    virtual bool OnUpdate(float fFrameTime) override;

    void DrawPile();

    std::array<std::array<PileBlock, 10>, 20> m_aLogicalBoard;

    sf::RectangleShape m_sfBoardOutline;

    sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoordinate, int yLogicalCoordinate);
    sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords);

    sf::Vector2i m_sfDummyBlockLoc;
    sf::Vector2i m_sfDummyBlockLocPrev;

    float fAccumulatedTime = 0.0f;
public:
    BlockTris();
};

#endif