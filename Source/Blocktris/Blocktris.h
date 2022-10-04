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
//     Forward Declarations    //
/////////////////////////////////

class Tetrimino;
class VirtualBag;

/////////////////////////////////
// STL and C/C++ lib includes  //
/////////////////////////////////
#include <array>
#include <utility>
#include <memory>
#include <cmath>

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
	BlockLockDelay,
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
    void DrawTetrimino(std::shared_ptr<Tetrimino> pttMino, sf::IntRect sfBoundingRect);
    void CheckLineClears();
    void DrawPreviewAndHeld();
    void DrawTetriminoInBox(
	std::array<sf::RectangleShape, 4>& aBlocksViz, 
	sf::Vector2f sfPivot,
	sf::Vector2f sfCenter, 
	float fVerticalOffset
    );
    void UpdateText();
    void RecalculateLevel();
    unsigned int CalculateScore(bool bFour, bool bTriple, bool bDouble, int nSingles);
    bool LineBundle(int nLines);
    float LevelCurveFunction(int nLevel);

    template<typename Iter>
    void ReassignDigits(Iter itFirst, Iter itLast, size_t stLen, unsigned long long ullNum);

    template<typename Iter>
    void SetupDigits(Iter itFirst, Iter itLast, float yOffset);

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
    unsigned int m_unStateInterval = 60;
    unsigned int m_unMoveInterval = 10;
    unsigned long long m_ullTetriminoMoveTimer = 1;
    unsigned long long m_ullGameTicks = 1;
    unsigned long long m_ullLockDelayTimer = 5;

    // Player statistics
    unsigned long long m_ullPoints = 0;
    unsigned int m_unLevel = 1;
    unsigned int m_unLinesCleared = 0;

    // Per-update statistics
    unsigned int m_unCellsFastDropped = 0;
    unsigned int m_unCellsHardDropped = 0;
public:
    BlockTris();
    ~BlockTris();
};

template<typename Iter>
void BlockTris::ReassignDigits(Iter itFirst, Iter itLast, size_t stLen, unsigned long long ullNum) {
    std::vector<unsigned int> vDigits;
    unsigned long long ullNumCopy = ullNum;

    for (int i = 0; i < stLen; i++) {
	vDigits.push_back(ullNumCopy % 10);
	ullNumCopy /= 10;
    };

    for (int i = 0; itFirst != itLast; itFirst++) {
	itFirst->setTextureRect(sf::IntRect(
	    vDigits[stLen - 1 - i] * FontWidth, 0, FontWidth, FontHeight
	));
	i++;
    }
}

template<typename Iter>
void BlockTris::SetupDigits(Iter itFirst, Iter itLast, float yOffset) {
    float xfOffset = 0;
    for (; itFirst != itLast; itFirst++) {
	itFirst->setPosition(FirstDigitLineX + xfOffset, FirstDigitLineY + yOffset);
	itFirst->setTexture(m_sfDigitsTexture);
	itFirst->setTextureRect(sf::IntRect(0, 0, FontWidth, FontHeight));
	xfOffset += FontWidth;
    }
}

#endif
