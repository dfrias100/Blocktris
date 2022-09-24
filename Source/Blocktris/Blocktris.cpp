#include "Blocktris.h"

#include <iostream>

bool BlockTris::OnInitialize() {
    m_sfBoardOutline = sf::RectangleShape(sf::Vector2f(BoardSizeX + SquareOutlineThickness, 
	BoardSizeY + SquareOutlineThickness));
    
    m_sfBoardOutline.setFillColor(sf::Color::Transparent);
    m_sfBoardOutline.setOutlineColor(sf::Color::White);
    m_sfBoardOutline.setOutlineThickness(5.0f);
    m_sfBoardOutline.setPosition(BoardOffsetX, BoardOffsetY);

    m_sfLocationCalculationTest.setFillColor(sf::Color::Red);
    m_sfLocationCalculationTest.setSize(
	sf::Vector2f(SquareSize - SquareOutlineThickness,
	    SquareSize - SquareOutlineThickness)
	);
    m_sfLocationCalculationTest.setOutlineColor(sf::Color::Transparent);
    m_sfLocationCalculationTest.setOutlineThickness(SquareOutlineThickness);

    return true;
}

bool BlockTris::OnUpdate(float fElapsedTime) {
    PushDrawableObject(&m_sfBoardOutline);
    float fEndCondX = BoardOffsetX + 10.0f * SquareSize;
    float fEndCondY = BoardOffsetY + 20.0f * SquareSize;

    if (m_vpsfBlocks.size() > 0) {
	for (auto ptr : m_vpsfBlocks)
	    delete ptr;
	m_vpsfBlocks.clear();
    }

    for (float xF = BoardOffsetX; xF < fEndCondX; xF += SquareSize) {
	for (float yF = BoardOffsetY; yF < fEndCondY; yF += SquareSize) {
	    sf::RectangleShape* psfShape 
		= new sf::RectangleShape(
		    sf::Vector2f(SquareSize - SquareOutlineThickness, 
			SquareSize - SquareOutlineThickness)
		);

	    psfShape->setPosition(xF + SquareOutlineThickness, yF + SquareOutlineThickness);
	    psfShape->setFillColor(sf::Color::White);
	    psfShape->setOutlineThickness(SquareOutlineThickness);
	    psfShape->setOutlineColor(sf::Color::Transparent);

	    m_vpsfBlocks.push_back(psfShape);
	    PushDrawableObject(psfShape);
	}
    }

    if (GetKeyStatus(sf::Keyboard::K) == KeyStatus::Pressed) {
	m_sfLocationCalculationTest.setPosition(sf::Vector2f(toScreenX, toScreenY));
	PushDrawableObject(&m_sfLocationCalculationTest);
    } else if (GetKeyStatus(sf::Keyboard::K) == KeyStatus::Released) {
	int xRandom = rand() % 10;
	int yRandom = rand() % 20;

	toScreenX = BoardOffsetX + xRandom * SquareSize + SquareOutlineThickness;
	toScreenY = BoardOffsetY + yRandom * SquareSize + SquareOutlineThickness;
    }

    return true;
}

BlockTris::BlockTris() 
    : GameApp(sf::VideoMode(ScreenWidth, ScreenHeight), "Blocktris") {
}
