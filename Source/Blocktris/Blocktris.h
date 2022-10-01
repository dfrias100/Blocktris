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
static constexpr float BoardSizeX = SquareSize * 10.0f;
static constexpr float BoardSizeY = SquareSize * 20.0f;

static constexpr float ScreenWidth = 2.5f * BoardSizeX;
static constexpr float ScreenHeight = 800.0f;

static constexpr float BoardOffsetX = (ScreenWidth - BoardSizeX) / 2.0f;
static constexpr float BoardOffsetY = (ScreenHeight - BoardSizeY) / 2.0f;

static constexpr float PreviewRectSizeX = SquareSize * 4.0f + 15.0f;
static constexpr float PreviewRectSizeY = 3.0f * PreviewRectSizeX;
static constexpr float PreviewRectOffsetX = BoardOffsetX * 1.5f + BoardSizeX - PreviewRectSizeX * 0.5f + 2.5f;
static constexpr float PreviewRectOffsetY = 0.15f * ScreenHeight;

static constexpr float FirstDigitLineX = (53.0f / 875.0f) * ScreenWidth;
static constexpr float FirstDigitLineY = (392.0f / 800.0f) * ScreenHeight;

static constexpr float DigitLineGap = (111.0f / 800.0f) * ScreenHeight;

static constexpr int FontWidth = 18;
static constexpr int FontHeight = 35;

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
	BlockHit,
	HoldPieceAttempt,
	Pause,
	GameOver
    };

    virtual bool OnInitialize() override;
    virtual bool OnUpdate(float fFrameTime) override;
    
    // Utility functions wrapping up functionality into a single routine
    void DrawPile();
    void CalculateHardDropPreview();
    void SetupOutline(sf::RectangleShape& sfRect, float fxOffset, float fyOffset);
    void DrawTetrimino(std::array<sf::RectangleShape, 4>& aBlocksViz);
    void CheckLineClears();
    void DrawPreviewAndHeld();
    void DrawTetriminoInBox(
	std::array<sf::RectangleShape, 4>& aBlocksViz, 
	sf::Vector2f sfPivot,
	sf::Vector2f sfCenter, 
	float fVerticalOffset
    );
    void UpdateLinesText();
    bool LineBundle(int nLines);

    // Data structures representing the state of the game and inputs
    Board m_aLogicalBoard;
    std::array<std::pair<unsigned int, bool>, 20> m_aRowMetaData;
    std::array<KeyStatus, 2> m_aPrevFrameKeyStates;
    std::array<KeyStatus, 2> m_aCurrFrameKeyStates;

    // SFML objects here
    sf::RectangleShape m_sfBoardOutline;
    sf::RectangleShape m_sfPreviewOutline;
    sf::RectangleShape m_sfHeldOutline;
    sf::Texture m_sfBgTexture;
    sf::Texture m_sfDigitsTexture;
    sf::Sprite m_sfBackground;
    std::array<sf::Sprite, 7> m_asfScoreSprites;
    std::array<sf::Sprite, 3> m_asfLinesSprites;
    std::array<sf::Sprite, 3> m_asfLevelSprites;

    // Tetrimino related objects
    VirtualBag* m_pvbWaitingBlocks;
    std::shared_ptr<Tetrimino> m_pActiveTetrimino;
    std::shared_ptr<Tetrimino> m_pHeldTetrimino;
    std::shared_ptr<Tetrimino> m_pHardDropPreview;
    std::array<std::shared_ptr<Tetrimino>, 3> m_aPreviewTetriminos;

    // Holds our state
    GameStates m_gsState;
    GameStates m_gsSavedState;

    // Other meta data about the state of the game
    bool m_bKeyPressedInitialLeft = false;
    bool m_bKeyPressedInitialRight = false;
    bool m_bKeyHeldLeft = false;
    bool m_bKeyHeldRight = false;
    bool m_bRotationKeyHeld = false;
    bool m_bAlreadyPressedHeld = false;
    bool m_bSkipInput = false;
    bool m_bHeldChanged = false;
    bool m_bRefreshPreview = true;

    // Timers and speeds
    unsigned int m_unStateInterval = 15;
    unsigned int m_unMoveInterval = 10;
    unsigned long long m_ullTetriminoMoveTimer = 1;
    unsigned long long m_ullGameTicks = 1;

    // Player statistics
    unsigned long long m_ullPoints = 0;
    unsigned int m_unLevel = 0;
    unsigned int m_unLinesCleared = 0;
public:
    // Translates logical array coordinates to screen coordinates -- used for blocks only
    static sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoordinate, int yLogicalCoordinate);
    static sf::Vector2f LogicalCoordsToScreenCoords(float xLogicalCoordinate, float yLogicalCoordinate);
    static sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords);

    BlockTris();
    ~BlockTris();
};

#endif