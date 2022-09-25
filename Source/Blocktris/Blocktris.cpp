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

    m_aLogicalBoard[0][0].m_bHidden = false;

    return true;
}

bool BlockTris::OnUpdate(float fFrameTime) {
    PushDrawableObject(&m_sfBoardOutline);

    fAccumulatedTime += fFrameTime;

    if (fAccumulatedTime > 1.0f) {
	m_aLogicalBoard[m_sfDummyBlockLocPrev.y][m_sfDummyBlockLocPrev.x].m_bHidden = true;

	m_sfDummyBlockLoc.x += 1;

	if (m_sfDummyBlockLoc.x % 10 == 0) {
	    m_sfDummyBlockLoc.x = 0;
	    m_sfDummyBlockLoc.y = (m_sfDummyBlockLoc.y + 1) % 20;
	}

	m_aLogicalBoard[m_sfDummyBlockLoc.y][m_sfDummyBlockLoc.x].m_bHidden = false;

	m_sfDummyBlockLocPrev = m_sfDummyBlockLoc;

	fAccumulatedTime = 0.0f;
    }

    DrawPile();

    return true;
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
    : GameApp(sf::VideoMode(ScreenWidth, ScreenHeight), "Blocktris") {
}
