#ifndef BLOCKTRIS_H
#define BLOCKTRIS_H

#include <array>
#include <utility>

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

struct SingleBlockTetrimino {
    sf::Vector2i m_sfLogicalCoords;
    sf::RectangleShape m_sfTetriminoViz;
};


class BlockTris : public GameApp {
private:
    enum class GameStates {
	BlockGeneration,
	BlockFalling,
	BlockHit
    };

    virtual bool OnInitialize() override;
    virtual bool OnUpdate(float fFrameTime) override;
    
    void ProcessInput();

    void DrawPile();

    std::array<std::array<PileBlock, 10>, 20> m_aLogicalBoard;
    std::pair<sf::Keyboard::Key, KeyStatus> m_BufferedInput;
    std::vector<KeyStatus> m_vPrevFrameKeyStates;
    std::vector<KeyStatus> m_vCurrFrameKeyStates;

    sf::RectangleShape m_sfBoardOutline;

    sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoordinate, int yLogicalCoordinate);
    sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords);

    SingleBlockTetrimino m_SingleBlockTetrimino;

    GameStates m_gsState;

    bool m_bKeyPressedInitialLeft;
    bool m_bKeyPressedInitialRight;
    bool m_bKeyHeldLeft;
    bool m_bKeyHeldRight;

    unsigned int m_unMoveInterval = 10;
    unsigned long long m_ullTetriminoMoveTimer = 1;
    unsigned long long m_ullGameTicks = 1;
public:
    BlockTris();
};

#endif