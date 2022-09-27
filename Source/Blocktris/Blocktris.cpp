/*----------------------------------------------------------------------------------|
|  Blocktris: Tetris-like clone written in C++ using SFML. 			    |
|                                                                                   |
|  Copyright(C) 2022  Daniel Frias						    |
|                                                                                   |
|  This program is free software; you can redistribute it and/or		    |
|  modify it under the terms of the GNU General Public License			    |
|  as published by the Free Software Foundation; either version 2		    |
|  of the License, or (at your option) any later version.			    |
|  This program is distributed in the hope that it will be useful,		    |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of		    |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the			    |
|  GNU General Public License for more details.					    |
|  You should have received a copy of the GNU General Public License		    |
|  along with this program; if not, write to the Free Software			    |
|  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  |
|-----------------------------------------------------------------------------------*/

#include "Blocktris.h"

#include <iostream>

bool BlockTris::OnInitialize() {
    m_sfBoardOutline = sf::RectangleShape(sf::Vector2f(BoardSizeX + SquareOutlineThickness, 
	BoardSizeY + SquareOutlineThickness));
    
    // This formats the rectangle holding the board
    m_sfBoardOutline.setFillColor(sf::Color::Transparent);
    m_sfBoardOutline.setOutlineColor(sf::Color::White);
    m_sfBoardOutline.setOutlineThickness(5.0f);
    m_sfBoardOutline.setPosition(BoardOffsetX, BoardOffsetY);

    // Pre-formatting the blocks that will make up the tetrimino pile
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

    // Setup the row metadata. Each element will hold how many full spots there are and whether or not
    // it has been marked as cleared for a particular check iteration.
    for (auto& prRow : m_aRowMetaData) {
	prRow.first = 0;
	prRow.second = false;
    }

    auto prPieceData = m_ActiveTetrimino.GetPieceData();

    // Make sure first tetrimino is drawn properly
    for (int i = 0; i < 4; i++) {
	prPieceData.second[i].setPosition(LogicalCoordsToScreenCoords(prPieceData.first[i]));
    }
    
    // For now, we will set up the initial state as falling
    m_gsState = GameStates::BlockFalling;
    return true;
}

bool BlockTris::OnUpdate(float fFrameTime) {
    /*-----------------------------------------------------------|
    |  The game will be locked at 30 fps, processing the states  |
    |  every 15 ticks (~500 ms). Every update call advances the  |
    |  tick counter by 1.                                        |
    |-----------------------------------------------------------*/

    auto prPieceData = m_ActiveTetrimino.GetPieceData();

    if (m_ullGameTicks % m_unStateInterval == 0) {
	/*---------------------------------------------------------------------|
	|    The game can be easily interpreted as a finite state machine.     |
	|    Every time the tick counter is tripped, we process the current    |
	|    or next state that should occur. For example, when we detect a    |
	|    vertical collision, we should transition to the collision state   |
	|    where it will contain the logic needed to add the tetrimino to    |
	|    the pile. This greatly simplifies the logic needed to process     |
	|    all the states that need the game could be in.                    |
	|---------------------------------------------------------------------*/

	switch (m_gsState)
	{
	case GameStates::BlockGeneration:
	{
	    // Reset the position of our tetrimino now that we've thrown it into the pile
	    m_ActiveTetrimino.ResetPieceAndPivot();

	    for (int i = 0; i < 4; i++) {
		prPieceData.second[i].setPosition(LogicalCoordsToScreenCoords(prPieceData.first[i]));
	    }

	    m_gsState = GameStates::BlockFalling;
	}
	    break;
	case GameStates::BlockFalling:
	{
	    // Store the candidate coordinates
	    auto vTetriminoLogicalCoords = prPieceData.first;
	    bool bVerticalCollision = false;

	    for (auto& sfCoords : vTetriminoLogicalCoords) {
		sfCoords.y++;
	    }

	    for (int i = 0; i < 4; i++) {
		int xTest = vTetriminoLogicalCoords[i].x;
		int yTest = vTetriminoLogicalCoords[i].y;

		// If we have a vertical hit, we switch to the hit state
		if (yTest >= 20 || !m_aLogicalBoard[yTest][xTest].m_bHidden) {
		    m_gsState = GameStates::BlockHit;
		    bVerticalCollision = true;
		    break;
		}
	    }

	    if (!bVerticalCollision) {
		prPieceData.first = vTetriminoLogicalCoords;
		for (int i = 0; i < 4; i++) {
		    prPieceData.second[i].setPosition(LogicalCoordsToScreenCoords(prPieceData.first[i]));
		}
	    }
	}
	    break;
	case GameStates::BlockHit:
	{
	    /*---------------------------------------------------------------|
	    |	Take the tetrimino and "throw" its blocks onto the pile      |
	    |	In this case, we make the blocks where the tetrimino rests   |
	    |	visible in the logical board.                                |
	    |---------------------------------------------------------------*/

	    for (auto sfPileBlockLoc : prPieceData.first) {
		m_aLogicalBoard[sfPileBlockLoc.y][sfPileBlockLoc.x].m_bHidden = false;
		m_aRowMetaData[sfPileBlockLoc.y].first++;
	    }

	    m_gsState = GameStates::BlockGeneration;
	}
	    break;
	default:
	    // Something has gone very wrong if we end up here
	    break;
	}
    }

    bool bDownIsPressed = GetKeyStatus(sf::Keyboard::Down) == KeyStatus::Pressed;

    if (bDownIsPressed && 
	m_gsState != GameStates::BlockGeneration &&
	m_gsState != GameStates::BlockHit) {
	m_unStateInterval = 1;
    } else {
	m_unStateInterval = 15;
    }

    /*--------------------------------------------------------------------------------|	
    |	Here we record the current state of the keyboard scoped to		      |
    |	the left and right keys. We will then check for two conditions:		      |
    |										      |
    |	1. If a key is being pressed when it was previously not -- a "rising edge".   |
    |   2. If a key is being pressed when it was previously recorded as being 	      |
    |      pressed beforehand -- a "high signal" or a "held state".		      |
    |										      |
    |	This will allow us to ascertain what to do with regards to the move timer;    |
    |   if we have a "rising edge" we will interrupt the timer so that the process    |
    |   input routine will run and move the block. If we don't have a rising edge,    |
    |   the held state will be used to check if we need to move a block. 	      |
    |										      |
    |   The effect is that when individual button presses are made, the block will    |
    |   move more or less instantaneously. But when held will move the block in	      |
    |   that direction at the move timer's speed. This is the intended behavior and   |
    |   is present in the original Game Boy release of tetris.                        |
    |--------------------------------------------------------------------------------*/

    m_aCurrFrameKeyStates[0] = GetKeyStatus(sf::Keyboard::Left);
    m_aCurrFrameKeyStates[1] = GetKeyStatus(sf::Keyboard::Right);

    // Check for the "rising edge"
    m_bKeyPressedInitialLeft = m_aPrevFrameKeyStates[0] == KeyStatus::NotPressed && 
	m_aCurrFrameKeyStates[0] == KeyStatus::Pressed;

    m_bKeyPressedInitialRight = m_aPrevFrameKeyStates[1] == KeyStatus::NotPressed &&
	m_aCurrFrameKeyStates[1] == KeyStatus::Pressed;

    // Check for the "high signal"
    m_bKeyHeldLeft = m_aPrevFrameKeyStates[0] == KeyStatus::Pressed &&
	m_aCurrFrameKeyStates[0] == KeyStatus::Pressed;

    m_bKeyHeldRight = m_aPrevFrameKeyStates[1] == KeyStatus::Pressed &&
	m_aCurrFrameKeyStates[1] == KeyStatus::Pressed;
    
    // If we have a rising edge, interrupt the timer so that the routine fires
    if (m_bKeyPressedInitialLeft || m_bKeyPressedInitialRight) {
	m_ullTetriminoMoveTimer = 0;
    }

    // Check this next frame
    m_aPrevFrameKeyStates = m_aCurrFrameKeyStates;

    // Process input if the timer is tripped
    if (m_gsState == GameStates::BlockFalling
	&& m_ullTetriminoMoveTimer == 0) {
	ProcessInput(prPieceData.first, prPieceData.second);
    } 

    // Advance the timers by 1 tick (1/30th of one second)
    m_ullGameTicks++;
    m_ullTetriminoMoveTimer = (m_ullTetriminoMoveTimer + 1) % m_unMoveInterval;

    // Drawing routine
    PushDrawableObject(&m_sfBoardOutline);
    DrawTetrimino(prPieceData.second);
    DrawPile();
    return true;
}

void BlockTris::ProcessInput(std::vector<sf::Vector2i>& vTetriminoLogicalCoords, 
    std::array<sf::RectangleShape, 4>& aBlocksViz) {
    
    auto vTetriminoLogicalCoordsTest = vTetriminoLogicalCoords;
    bool bHorizontalCollision = false;

    for (auto& sfCoords : vTetriminoLogicalCoordsTest) {
	// Clamp the new x values to allowable values
	if (m_bKeyHeldLeft || m_bKeyPressedInitialLeft) sfCoords.x--;
	if (m_bKeyHeldRight|| m_bKeyPressedInitialRight) sfCoords.x++;

	if (sfCoords.x < 0 || sfCoords.x > 9 || !m_aLogicalBoard[sfCoords.y][sfCoords.x].m_bHidden) {
	    bHorizontalCollision = true;
	    break;
	}
    }

    if (!bHorizontalCollision) {
	vTetriminoLogicalCoords = vTetriminoLogicalCoordsTest;
	for (int i = 0; i < 4; i++) {
	    aBlocksViz[i].setPosition(LogicalCoordsToScreenCoords(vTetriminoLogicalCoords[i]));
	}
    }
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

void BlockTris::DrawTetrimino(std::array<sf::RectangleShape, 4>& aBlocksViz) {
    for (auto& sfBlock : aBlocksViz) {
	PushDrawableObject(&sfBlock);
    }
}

void BlockTris::CheckLineClears() {
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
