#ifndef BLOCKTRIS_H
#define BLOCKTRIS_H

#include "../GameApp/GameApp.h"

static constexpr float ScreenWidth = 800.0f;
static constexpr float ScreenHeight = 800.0f;

static constexpr float SquareSize = 35.0f;
static constexpr float SquareOutlineThickness = 4.0f;

static constexpr float BoardSizeX = SquareSize * 10.0f;
static constexpr float BoardSizeY = SquareSize * 20.0f;

static constexpr float BoardOffsetX = 1.0f * ScreenWidth / 8.0f;
static constexpr float BoardOffsetY = (ScreenHeight - BoardSizeY) / 2.0f;

class BlockTris : public GameApp {
private:
    virtual bool OnInitialize() override;
    virtual bool OnUpdate(float fElapsedTime) override;
    
    std::vector<sf::RectangleShape*> m_vpsfBlocks;

    sf::RectangleShape m_sfLocationCalculationTest;
    sf::RectangleShape m_sfBoardOutline;

    float toScreenX;
    float toScreenY;
public:
    BlockTris();
};

#endif