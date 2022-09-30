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

#ifndef BLOCKTRIS_H
#define BLOCKTRIS_H

/////////////////////////////////
// STL and C/C++ lib includes  //
/////////////////////////////////
#include <array>
#include <utility>
#include <memory>

/////////////////////////////////
//    Local folder includes    //
/////////////////////////////////
#include "PileBlock.h"
#include "PieceConstants.h"
#include "Tetrimino/Tetrimino.h"
#include "VirtualBag/VirtualBag.h"

//////////////////////////////////
// Other project-local includes //
//////////////////////////////////
#include "../GameApp/GameApp.h"

///////////////////////////////////////
// Pre-processor/constexpr constants //
///////////////////////////////////////
static constexpr float ScreenWidth = 800.0f;
static constexpr float ScreenHeight = 800.0f;

static constexpr float BoardSizeX = SquareSize * 10.0f;
static constexpr float BoardSizeY = SquareSize * 20.0f;

static constexpr float BoardOffsetX = 1.0f * ScreenWidth / 8.0f;
static constexpr float BoardOffsetY = (ScreenHeight - BoardSizeY) / 2.0f;

/////////////////////////////////
//     Forward Declarations    //
/////////////////////////////////

class Tetrimino;
class VirtualBag;

/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
  Class:    BlockTris

  Summary:  The implementation class of the Tetris game,
	    inherits GameApp -- the base engine.
 C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class BlockTris : public GameApp {
private:
    // Enum holding all possible states of the game
    enum class GameStates {
	BlockGeneration,
	BlockFalling,
	BlockHit
    };

    virtual bool OnInitialize() override;
    virtual bool OnUpdate(float fFrameTime) override;
    
    // Utility functions wrapping up functionality into a single routine
    void DrawPile();
    void DrawTetrimino(std::array<sf::RectangleShape, 4>& aBlocksViz);
    void CheckLineClears();
    bool LineBundle(int nLines);

    // Data structures representing the state of the game and inputs
    Board m_aLogicalBoard;
    std::array<std::pair<unsigned int, bool>, 20> m_aRowMetaData;
    std::array<KeyStatus, 2> m_aPrevFrameKeyStates;
    std::array<KeyStatus, 2> m_aCurrFrameKeyStates;

    // SFML objects here
    sf::RectangleShape m_sfBoardOutline;

    // Tetrimino related objects
    VirtualBag* m_pvbWaitingBlocks;
    std::shared_ptr<Tetrimino> m_pActiveTetrimino;

    // Holds our state
    GameStates m_gsState;

    // Other meta data about the state of the game
    bool m_bKeyPressedInitialLeft = false;
    bool m_bKeyPressedInitialRight = false;
    bool m_bKeyHeldLeft = false;
    bool m_bKeyHeldRight = false;
    bool m_bRotationKeyHeld = false;

    // Timers and speeds
    unsigned int m_unStateInterval = 15;
    unsigned int m_unMoveInterval = 10;
    unsigned long long m_ullTetriminoMoveTimer = 1;
    unsigned long long m_ullGameTicks = 1;
public:
    // Translates logical array coordinates to screen coordinates -- used for blocks only
    static sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoordinate, int yLogicalCoordinate);
    static sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords);

    BlockTris();
    ~BlockTris();
};

#endif