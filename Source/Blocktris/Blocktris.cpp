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
    
    // For now, we will set up the initial state as falling
    m_gsState = GameStates::BlockFalling;

    // Setting up the "tetrimino" block
    sf::RectangleShape* psfShape = &m_SingleBlockTetrimino.m_sfTetriminoViz;
    psfShape->setSize(sf::Vector2f(TrueSquareSize, TrueSquareSize));
    psfShape->setPosition(LogicalCoordsToScreenCoords(5, 0));
    psfShape->setFillColor(sf::Color::Red);
    psfShape->setOutlineThickness(SquareOutlineThickness);
    psfShape->setOutlineColor(sf::Color::Transparent);

    m_SingleBlockTetrimino.m_sfLogicalCoords.x = 5;
    m_SingleBlockTetrimino.m_sfLogicalCoords.y = 0;

    /*--------------------------------------------------|
    |  TODO: Resizing the vector holding the keystates  |
    |  -- this probably should be an array              |
    |--------------------------------------------------*/
    m_vPrevFrameKeyStates.resize(2);
    m_vCurrFrameKeyStates.resize(2);

    return true;
}

bool BlockTris::OnUpdate(float fFrameTime) {
    /*-----------------------------------------------------------|
    |  The game will be locked at 30 fps, processing the states  |
    |  every 15 ticks (~500 ms). Every update call advances the  |
    |  tick counter by 1.                                        |
    |-----------------------------------------------------------*/

    if (m_ullGameTicks % 15 == 0) {
	/*---------------------------------------------------------------------|
	|    The game can be easily interpreted as a finite state machine.     |
	|    Every time the tick counter is tripped, we process the current    |
	|    or next state that should occur. For example, when we detect a    |
	|    vertical collision, we should transition to the collision state   |
	|    where it will contain the logic needed to add the tetrimino to    |
	|    the pile. This greatly simplifies the logic needed to process     |
	|    all the states that need the game could be in.                    |
	|---------------------------------------------------------------------*/


	/*------------------------------------------------------------------------|
	|   TODO: This specific logic is mostly invalid for multi-block 	  |
	|   tetriminos -- the actual blocks in Tetris. But the general process	  |
	|   is much the same. We just have to do it for all the blocks in it.	  |
	|   It will be re-written as the actual tetriminos are programmed into	  |
	|   this game.								  |
	|------------------------------------------------------------------------*/

	switch (m_gsState)
	{
	case GameStates::BlockGeneration:
	{
	    // Reset the position of our tetrimino now that we've thrown it into the pile
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
	    // Store the candidate coordinates
	    int xTest = m_SingleBlockTetrimino.m_sfLogicalCoords.x;
	    int yTest = m_SingleBlockTetrimino.m_sfLogicalCoords.y + 1;

	    // If we have a vertical hit, we switch to the hit state
	    if (yTest >= 20 || !m_aLogicalBoard[yTest][xTest].m_bHidden)
		m_gsState = GameStates::BlockHit;
	    else {
		// If not, actually set it to the candidate coordinates
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
	    /*---------------------------------------------------------------|
	    |	Take the tetrimino and "throw" its blocks onto the pile      |
	    |	In this case, we make the blocks where the tetrimino rests   |
	    |	visible in the logical board.                                |
	    |---------------------------------------------------------------*/

	    auto sfNewPileBlockLoc = m_SingleBlockTetrimino.m_sfLogicalCoords;
	    m_aLogicalBoard[sfNewPileBlockLoc.y][sfNewPileBlockLoc.x].m_bHidden = false;
	    m_gsState = GameStates::BlockGeneration;
	}
	    break;
	default:
	    // Something has gone very wrong if we end up here
	    break;
	}
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
    
    // If we have a rising edge, interrupt the timer so that the routine fires
    if (m_bKeyPressedInitialLeft || m_bKeyPressedInitialRight) {
	m_ullTetriminoMoveTimer = 0;
    }

    // Check this next frame
    m_vPrevFrameKeyStates = m_vCurrFrameKeyStates;

    // Process input if the timer is tripped
    if (m_gsState == GameStates::BlockFalling
	&& m_ullTetriminoMoveTimer == 0) {
	ProcessInput();
    } 

    // Advance the timers by 1 tick (1/30th of one second)
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

    // Clamp the new x values to allowable values
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
