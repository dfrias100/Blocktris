#include "Blocktris.h"

#include <iostream>

bool BlockTris::OnInitialize() {
    m_sfBoardOutline = sf::RectangleShape(sf::Vector2f(BoardSizeX + SquareOutlineThickness, 
	BoardSizeY + SquareOutlineThickness));
    
    m_sfBoardOutline.setFillColor(sf::Color::Transparent);
    m_sfBoardOutline.setOutlineColor(sf::Color::White);
    m_sfBoardOutline.setOutlineThickness(5.0f);
    m_sfBoardOutline.setPosition(BoardOffsetX, BoardOffsetY);

    for (int x = 0; x < 10; x++) {
	for (int y = 0; y < 20; y++) {
	    sf::RectangleShape* psfShape = &m_aLogicalBoard[y][x].m_sfBlockViz;
	    psfShape->setSize(sf::Vector2f(TrueSquareSize, TrueSquareSize));
	    psfShape->setPosition(LogicalCoordsToScreenCoords(x, y));
	    psfShape->setFillColor(sf::Color::Red);
	    psfShape->setOutlineThickness(SquareOutlineThickness);
	    psfShape->setOutlineColor(sf::Color::Transparent);
	}
    }

    m_gsState = GameStates::BlockFalling;

    sf::RectangleShape* psfShape = &m_SingleBlockTetrimino.m_sfTetriminoViz;
    psfShape->setSize(sf::Vector2f(TrueSquareSize, TrueSquareSize));
    psfShape->setPosition(LogicalCoordsToScreenCoords(5, 0));
    psfShape->setFillColor(sf::Color::Red);
    psfShape->setOutlineThickness(SquareOutlineThickness);
    psfShape->setOutlineColor(sf::Color::Transparent);

    m_SingleBlockTetrimino.m_sfLogicalCoords.x = 5;
    m_SingleBlockTetrimino.m_sfLogicalCoords.y = 0;

    m_vPrevFrameKeyStates.resize(2);
    m_vCurrFrameKeyStates.resize(2);

    return true;
}

bool BlockTris::OnUpdate(float fFrameTime) {
    if (m_ullGameTicks % 15 == 0) {
	switch (m_gsState)
	{
	case GameStates::BlockGeneration:
	{
	    m_SingleBlockTetrimino.m_sfLogicalCoords.x = 5;
	    m_SingleBlockTetrimino.m_sfLogicalCoords.y = 0;
	    m_SingleBlockTetrimino.m_sfTetriminoViz.setPosition(
		LogicalCoordsToScreenCoords(m_SingleBlockTetrimino.m_sfLogicalCoords)
	    );
	    m_gsState = GameStates::BlockFalling;
	}
	    break;
	case GameStates::BlockFalling:
	{
	    int xTest = m_SingleBlockTetrimino.m_sfLogicalCoords.x;
	    int yTest = m_SingleBlockTetrimino.m_sfLogicalCoords.y + 1;

	    if (yTest >= 20 || !m_aLogicalBoard[yTest][xTest].m_bHidden)
		m_gsState = GameStates::BlockHit;
	    else {
		m_SingleBlockTetrimino.m_sfLogicalCoords.y = yTest;
		m_SingleBlockTetrimino.m_sfLogicalCoords.x = xTest;
		m_SingleBlockTetrimino.m_sfTetriminoViz.setPosition(
		    LogicalCoordsToScreenCoords(m_SingleBlockTetrimino.m_sfLogicalCoords)
		);
	    }
	}
	    break;
	case GameStates::BlockHit:
	{
	    auto sfNewPileBlockLoc = m_SingleBlockTetrimino.m_sfLogicalCoords;
	    m_aLogicalBoard[sfNewPileBlockLoc.y][sfNewPileBlockLoc.x].m_bHidden = false;
	    m_gsState = GameStates::BlockGeneration;
	}
	    break;
	default:
	    break;
	}
    }

    m_vCurrFrameKeyStates[0] = GetKeyStatus(sf::Keyboard::Left);
    m_vCurrFrameKeyStates[1] = GetKeyStatus(sf::Keyboard::Right);

    // Check for the "rising edge"
    m_bKeyPressedInitialLeft = m_vPrevFrameKeyStates[0] == KeyStatus::NotPressed && 
	m_vCurrFrameKeyStates[0] == KeyStatus::Pressed;

    m_bKeyPressedInitialRight = m_vPrevFrameKeyStates[1] == KeyStatus::NotPressed &&
	m_vCurrFrameKeyStates[1] == KeyStatus::Pressed;

    // Check for the "high signal"
    m_bKeyHeldLeft = m_vPrevFrameKeyStates[0] == KeyStatus::Pressed &&
	m_vCurrFrameKeyStates[0] == KeyStatus::Pressed;

    m_bKeyHeldRight = m_vPrevFrameKeyStates[1] == KeyStatus::Pressed &&
	m_vCurrFrameKeyStates[1] == KeyStatus::Pressed;
    
    // If we have a rising edge, immediately activate the call to process an input
    if (m_bKeyPressedInitialLeft || m_bKeyPressedInitialRight) {
	m_ullTetriminoMoveTimer = 0;
    }

    // Check this next frame
    m_vPrevFrameKeyStates = m_vCurrFrameKeyStates;

    // Process input
    if (m_gsState == GameStates::BlockFalling
	&& m_ullTetriminoMoveTimer == 0) {
	ProcessInput();
    } 

    m_ullGameTicks++;
    m_ullTetriminoMoveTimer = (m_ullTetriminoMoveTimer + 1) % m_unMoveInterval;

    // Drawing routine
    PushDrawableObject(&m_sfBoardOutline);
    PushDrawableObject(&m_SingleBlockTetrimino.m_sfTetriminoViz);
    DrawPile();
    return true;
}

void BlockTris::ProcessInput() {
    int xTest = m_SingleBlockTetrimino.m_sfLogicalCoords.x;

    if (m_bKeyHeldLeft || m_bKeyPressedInitialLeft) xTest = std::max(std::min(xTest - 1, 9), 0);
    if (m_bKeyHeldRight || m_bKeyPressedInitialRight) xTest = std::max(std::min(xTest + 1, 9), 0);

    m_SingleBlockTetrimino.m_sfLogicalCoords.x = xTest;
    m_SingleBlockTetrimino.m_sfTetriminoViz.setPosition(
	LogicalCoordsToScreenCoords(m_SingleBlockTetrimino.m_sfLogicalCoords)
    );
}

void BlockTris::DrawPile() {
    for (auto& aLine : m_aLogicalBoard) {
	for (auto& scPileBlock : aLine) {
	    if (!scPileBlock.m_bHidden) {
		PushDrawableObject(&scPileBlock.m_sfBlockViz);
	    }
	}
    }
}

sf::Vector2f BlockTris::LogicalCoordsToScreenCoords(int xLogicalCoord, int yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize
	+ SquareOutlineThickness;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize
	+ SquareOutlineThickness;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f BlockTris::LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords) {
    return LogicalCoordsToScreenCoords(sfLogicalCoords.x, sfLogicalCoords.y);
}

BlockTris::BlockTris()
    : GameApp(sf::VideoMode(ScreenWidth, ScreenHeight), "Blocktris", FPSControl::Locked30) {
}
