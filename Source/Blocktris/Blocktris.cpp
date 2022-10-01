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

bool BlockTris::OnInitialize() {
    m_sfBoardOutline = sf::RectangleShape(sf::Vector2f(BoardSizeX,
	BoardSizeY));
    m_sfPreviewOutline = sf::RectangleShape(sf::Vector2f(PreviewRectSizeX,
	PreviewRectSizeY));
    m_sfHeldOutline = sf::RectangleShape(sf::Vector2f(PreviewRectSizeX,
	PreviewRectSizeX));

    m_sfBgTexture.loadFromFile("bg.png");
    m_sfDigitsTexture.loadFromFile("digit_atlas.png");

    m_sfBackground.setTexture(m_sfBgTexture, true);

    float xfOffset = 0.0f;
    for (auto& sfSprite : m_asfScoreSprites) {
	sfSprite.setPosition(FirstDigitLineX + xfOffset, FirstDigitLineY);
	sfSprite.setTexture(m_sfDigitsTexture);
	sfSprite.setTextureRect(sf::IntRect(0, 0, FontWidth, FontHeight));
	xfOffset += FontWidth;
    }

    xfOffset = 0.0f;
    for (auto& sfSprite : m_asfLevelSprites) {
	sfSprite.setPosition(FirstDigitLineX + xfOffset, FirstDigitLineY + DigitLineGap);
	sfSprite.setTexture(m_sfDigitsTexture);
	sfSprite.setTextureRect(sf::IntRect(0, 0, FontWidth, FontHeight));
	xfOffset += FontWidth;
    }

    xfOffset = 0.0f;
    for (auto& sfSprite : m_asfLinesSprites) {
	sfSprite.setPosition(FirstDigitLineX + xfOffset, FirstDigitLineY + 2.0f * DigitLineGap);
	sfSprite.setTexture(m_sfDigitsTexture);
	sfSprite.setTextureRect(sf::IntRect(0, 0, FontWidth, FontHeight));
	xfOffset += FontWidth;
    }

    m_pvbWaitingBlocks = new VirtualBag();

    // Get our first mino
    m_pActiveTetrimino = m_pvbWaitingBlocks->GetNextPiece();
    m_aPreviewTetriminos = m_pvbWaitingBlocks->PeekNextPieces();
    m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
    
    // This formats the rectangle holding the board
    SetupOutline(m_sfBoardOutline, BoardOffsetX, BoardOffsetY);
    SetupOutline(m_sfPreviewOutline, PreviewRectOffsetX, PreviewRectOffsetY);
    SetupOutline(m_sfHeldOutline, 
	ScreenWidth - (PreviewRectOffsetX + PreviewRectSizeX), 
	PreviewRectOffsetY);

    // Pre-formatting the blocks that will make up the tetrimino pile
    for (int x = 0; x < 10; x++) {
	for (int y = 0; y < 20; y++) {
	    sf::RectangleShape* psfShape = &m_aLogicalBoard[y][x].m_sfBlockViz;
	    psfShape->setSize(sf::Vector2f(SquareSize, SquareSize));
	    psfShape->setPosition(LogicalCoordsToScreenCoords(x, y));
	    psfShape->setFillColor(sf::Color::Red);
	}
    }

    // Setup the row metadata. Each element will hold how many full spots there are and whether or not
    // it has been marked as cleared for a particular check iteration.
    for (auto& prRow : m_aRowMetaData) {
	prRow.first = 0;
	prRow.second = false;
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

    if (m_gsState != GameStates::BlockGeneration)
	CalculateHardDropPreview();

    // Input affecting game states
    bool bHeldKeyPressed = GetKeyStatus(sf::Keyboard::S) == KeyStatus::Pressed;
    bool bHardDropKeyReleased = GetKeyStatus(sf::Keyboard::Up) == KeyStatus::Released;
    bool bPauseKeyReleased = GetKeyStatus(sf::Keyboard::P) == KeyStatus::Released;
    bool bStateKeyPressed = false;
    bool bJustEnteredPause = false;

    if (m_gsState != GameStates::Pause) {
	if (bPauseKeyReleased) {
	    m_bSkipInput = true;

	    m_gsSavedState = m_gsState;
	    m_gsState = GameStates::Pause;

	    m_ullGameTicks = 0;
	    m_unStateInterval = 1;
	
	    bJustEnteredPause = true;
	    bStateKeyPressed = true;
	}

	if (bHeldKeyPressed && !m_bAlreadyPressedHeld && !bStateKeyPressed) {
	    m_bAlreadyPressedHeld = true;
	    m_bSkipInput = true;

	    m_gsState = GameStates::HoldPieceAttempt;

	    m_ullGameTicks = 0;
	
	    bStateKeyPressed = true;
	}

	if (bHardDropKeyReleased && !bStateKeyPressed) {
	    std::swap(m_pActiveTetrimino, m_pHardDropPreview);

	    for (auto& sfShape : m_pActiveTetrimino->GetPieceShapes()) {
		sf::Color sfShapeColor = sfShape.getFillColor();
		sfShapeColor.a = 255;
		sfShape.setFillColor(sfShapeColor);
	    }

	    m_gsState = GameStates::BlockHit;

	    m_ullGameTicks = 0;
	
	    m_bSkipInput = true;
	}
    }

    // Input affecting the blocks
    if (!m_bSkipInput) {
	bool bDownIsPressed = GetKeyStatus(sf::Keyboard::Down) == KeyStatus::Pressed;

	// Fast drop control
	if (bDownIsPressed &&
	    m_gsState != GameStates::BlockGeneration &&
	    m_gsState != GameStates::BlockHit) {
	    m_unStateInterval = 1;
	} else {
	    m_unStateInterval = 15;
	}

	/*--------------------------------------------------------------------------------|
	|   Here we record the current state of the keyboard scoped to			  |
	|   the left and right keys. We will then check for two conditions:		  |
	|										  |
	|   1. If a key is being pressed when it was previously not -- a "rising edge".   |
	|   2. If a key is being pressed when it was previously recorded as being 	  |
	|      pressed beforehand -- a "high signal" or a "held state".		          |
	|										  |
	|   This will allow us to ascertain what to do with regards to the move timer;    |
	|   if we have a "rising edge" we will interrupt the timer so that the process    |
	|   input routine will run and move the block. If we don't have a rising edge,    |
	|   the held state will be used to check if we need to move a block. 	          |
	|										  |
	|   The effect is that when individual button presses are made, the block will    |
	|   move more or less instantaneously. But when held will move the block in	  |
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
	    m_pActiveTetrimino->TranslateTetriminoHorizontal(
		m_bKeyHeldLeft || m_bKeyPressedInitialLeft,
		m_bKeyHeldRight || m_bKeyPressedInitialRight,
		m_aLogicalBoard
	    );

	    m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
	    CalculateHardDropPreview();
	}

	bool bLeftRotation = GetKeyStatus(sf::Keyboard::Z) == KeyStatus::Pressed;
	bool bRightRotation = GetKeyStatus(sf::Keyboard::X) == KeyStatus::Pressed;
	bool bRotationKeyPressed = bLeftRotation || bRightRotation;

	if (bRotationKeyPressed && !m_bRotationKeyHeld && m_gsState != GameStates::BlockHit) {
	    sf::Vector2f sfRotationCoefficients = bLeftRotation ?
		sf::Vector2f(-1.0f, 1.0f) : sf::Vector2f(1.0f, -1.0f);
	    m_pActiveTetrimino->RotateTetrimino(sfRotationCoefficients, m_aLogicalBoard);

	    m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
	    CalculateHardDropPreview();
	    
	    m_bRotationKeyHeld = true;
	} else if (!bRotationKeyPressed) {
	    m_bRotationKeyHeld = false;
	}
    }

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
	    if (m_pActiveTetrimino)
		m_bAlreadyPressedHeld = false;

	    m_pActiveTetrimino = m_pvbWaitingBlocks->GetNextPiece();
	    m_aPreviewTetriminos = m_pvbWaitingBlocks->PeekNextPieces();
	    m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);

	    m_bRefreshPreview = true;

	    m_gsState = GameStates::BlockFalling;
	}
	    break;
	case GameStates::BlockFalling:
	{
	    // Store the candidate coordinates
	    auto& vTetriminoLogicalCoords = m_pActiveTetrimino->GetLogicalCoords();
	    bool bVerticalCollision = false;

	    for (int i = 0; i < 4; i++) {
		int xTest = vTetriminoLogicalCoords[i].x;
		int yTest = vTetriminoLogicalCoords[i].y + 1;

		// If we have a vertical hit, we switch to the hit state
		if (yTest >= 20 || !m_aLogicalBoard[yTest][xTest].m_bHidden) {
		    m_gsState = GameStates::BlockHit;
		    bVerticalCollision = true;
		    break;
		}
	    }

	    if (!bVerticalCollision) {
		m_pActiveTetrimino->MoveDown();
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

	    m_bSkipInput = false;

	    sf::Color sfTetriminoColor = m_pActiveTetrimino->GetColor();

	    for (auto& sfPileBlockLoc : m_pActiveTetrimino->GetLogicalCoords()) {
		m_aLogicalBoard[sfPileBlockLoc.y][sfPileBlockLoc.x].m_bHidden = false;
		m_aRowMetaData[sfPileBlockLoc.y].first++;
		m_aLogicalBoard[sfPileBlockLoc.y][sfPileBlockLoc.x].m_sfBlockViz.setFillColor(
		    sfTetriminoColor
		);
	    }

	    CheckLineClears();

	    m_gsState = GameStates::BlockGeneration;
	}
	    break;
	case GameStates::HoldPieceAttempt:
	{
	    std::swap(m_pHeldTetrimino, m_pActiveTetrimino);
	    m_unStateInterval = 15;

	    if (!m_pActiveTetrimino) {
		m_gsState = GameStates::BlockGeneration;
		m_ullGameTicks = m_unStateInterval - 1;
	    } else {
		m_gsState = GameStates::BlockFalling;
		m_pActiveTetrimino->ResetPieceAndPivot();

		m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
		CalculateHardDropPreview();
	    }

	    m_bSkipInput = false;
	    m_bHeldChanged = true;
	}
	    break;
	case GameStates::Pause:
	{
	    if (bPauseKeyReleased && !bJustEnteredPause) {
		m_bSkipInput = false;
		m_gsState = m_gsSavedState;
		m_unStateInterval = 15;
	    }

	    bJustEnteredPause = false;
	}
	    break;
	default:
	    // Something has gone very wrong if we end up here
	    break;
	}
    }

    // Advance the timers by 1 tick (1/30th of one second)
    m_ullGameTicks++;
    m_ullTetriminoMoveTimer = (m_ullTetriminoMoveTimer + 1) % m_unMoveInterval;

    // Drawing routine
    PushDrawableObject(&m_sfBackground);
    PushDrawableObject(&m_sfBoardOutline);
    PushDrawableObject(&m_sfPreviewOutline);
    PushDrawableObject(&m_sfHeldOutline);
    DrawPreviewAndHeld();

    if (m_gsState != GameStates::BlockGeneration) {
	if (m_gsState != GameStates::Pause)
	    DrawTetrimino(m_pHardDropPreview->GetPieceShapes());
	DrawTetrimino(m_pActiveTetrimino->GetPieceShapes());
    }

    for (auto& sfShape : m_asfScoreSprites) {
	PushDrawableObject(&sfShape);
    }

    for (auto& sfShape : m_asfLevelSprites) {
	PushDrawableObject(&sfShape);
    }

    for (auto& sfShape : m_asfLinesSprites) {
	PushDrawableObject(&sfShape);
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

void BlockTris::CalculateHardDropPreview() {
    bool bVerticalCollision = false;
    while (!bVerticalCollision) {
	auto& vTetriminoLogicalCoords = m_pHardDropPreview->GetLogicalCoords();

	for (int i = 0; i < 4; i++) {
	    int xTest = vTetriminoLogicalCoords[i].x;
	    int yTest = vTetriminoLogicalCoords[i].y + 1;

	    if (yTest >= 20 || !m_aLogicalBoard[yTest][xTest].m_bHidden) {
		bVerticalCollision = true;
		break;
	    }
	}

	if (!bVerticalCollision) {
	    m_pHardDropPreview->MoveDown();
	}
    }
    
    for (auto& sfShape : m_pHardDropPreview->GetPieceShapes()) {
	sf::Color sfShapeColor = sfShape.getFillColor();
	sfShapeColor.a = 127;
	sfShape.setFillColor(sfShapeColor);
    }
}

void BlockTris::SetupOutline(sf::RectangleShape& sfRect, float fxOffset, float fyOffset) {
    sfRect.setFillColor(sf::Color::Transparent);
    sfRect.setOutlineColor(sf::Color::White);
    sfRect.setOutlineThickness(5.0f);
    sfRect.setPosition(fxOffset, fyOffset);
}

void BlockTris::DrawTetrimino(std::array<sf::RectangleShape, 4>& aBlocksViz) {
    for (auto& sfBlock : aBlocksViz) {
	PushDrawableObject(&sfBlock);
    }
}

void BlockTris::CheckLineClears() {
    bool bLinesCleared = false;
    bool bLinesClearedSingle = false;

    bLinesCleared = LineBundle(4) || LineBundle(3);
    
    if (!bLinesCleared) {
	bLinesCleared = LineBundle(2);
	bLinesClearedSingle = LineBundle(1);
	bLinesCleared = bLinesCleared || bLinesClearedSingle;
	LineBundle(1);
    }

    if (bLinesCleared) {
	int nLinesCleared = 0;

	for (int i = 0; i < 20; i++) {
	    if (m_aRowMetaData[i].second) {
		nLinesCleared++;
		for (int j = i; j >= 1; j--) {
		    std::swap(m_aRowMetaData[j], m_aRowMetaData[j - 1]);
		    std::swap(m_aLogicalBoard[j], m_aLogicalBoard[j - 1]);
		}
	    }
	}

	for (int i = 0; i < nLinesCleared; i++) {
	    m_aRowMetaData[i].first = 0;
	    m_aRowMetaData[i].second = false;

	    for (auto& plBlock : m_aLogicalBoard[i]) {
		plBlock.m_bHidden = true;
	    }
	}

	for (int y = 0; y < 20; y++) {
	    for (int x = 0; x < 10; x++) {
		m_aLogicalBoard[y][x].m_sfBlockViz.setPosition(
		    LogicalCoordsToScreenCoords(x, y)
		);
	    }
	}

	m_unLinesCleared += nLinesCleared;

	UpdateLinesText();
    }
}

void BlockTris::DrawPreviewAndHeld() {
    if (m_bHeldChanged) {
	m_pHeldTetrimino->ResetPieceAndPivot();

	auto& aHeldPieceShapes = m_pHeldTetrimino->GetPieceShapes();
	sf::Vector2f sfHeldPiecePivot = m_pHeldTetrimino->GetPivot();
	sf::Vector2f sfBoxCenter(
	    ScreenWidth - (PreviewRectOffsetX + PreviewRectSizeX),
	    PreviewRectOffsetY
	);

	sfBoxCenter.x += PreviewRectSizeX / 2.0f;
	sfBoxCenter.y += PreviewRectSizeX / 2.0f;

	DrawTetriminoInBox(aHeldPieceShapes, sfHeldPiecePivot, sfBoxCenter, 0.0f);

	m_bHeldChanged = false;
    }
 
    if (m_bRefreshPreview) {
	int i = 0;
	for (float yOffset = 0.0f; yOffset < (PreviewRectSizeY - PreviewRectSizeX * 0.5f); yOffset += PreviewRectSizeX) {
	    m_aPreviewTetriminos[i]->ResetPieceAndPivot();
	    auto& aPreviewPieceShapes = m_aPreviewTetriminos[i]->GetPieceShapes();
	    sf::Vector2f sfPreviewPiecePivot = m_aPreviewTetriminos[i]->GetPivot();
	    sf::Vector2f sfBoxCenter(
		PreviewRectOffsetX,
		PreviewRectOffsetY
	    );

	    sfBoxCenter.x += PreviewRectSizeX / 2.0f;
	    sfBoxCenter.y += PreviewRectSizeX / 2.0f;

	    DrawTetriminoInBox(aPreviewPieceShapes, sfPreviewPiecePivot, sfBoxCenter, yOffset);
	    i++;
	}
	
	m_bRefreshPreview = false;
    }

    if (m_pHeldTetrimino) {
	auto& aHeldPieceShapes = m_pHeldTetrimino->GetPieceShapes();
	DrawTetrimino(aHeldPieceShapes);
    }

    for (auto& tmPreview : m_aPreviewTetriminos) {
	auto& aPreviewPieceShapes = tmPreview->GetPieceShapes();
	DrawTetrimino(aPreviewPieceShapes);
    }
}

void BlockTris::DrawTetriminoInBox(
    std::array<sf::RectangleShape, 4>& aBlocksViz, 
    sf::Vector2f sfPivot,
    sf::Vector2f sfCenter,
    float fVerticalOffset
) {
    sfPivot = LogicalCoordsToScreenCoords(
	sfPivot.x,
	sfPivot.y
    );

    sfPivot = sfCenter - sfPivot;

    for (auto& sfShape : aBlocksViz) {
	sf::Vector2f sfCurrrentLocation = sfShape.getPosition();
	sfCurrrentLocation.x += sfPivot.x;
	sfCurrrentLocation.y += sfPivot.y + fVerticalOffset;
	sfCurrrentLocation.x -= SquareSize / 2.0f;
	sfShape.setPosition(sfCurrrentLocation);
    }
}

void BlockTris::UpdateLinesText() {
    if (m_unLinesCleared > 999)
	m_unLinesCleared = 999;

    std::vector<unsigned int> vDigits;
    unsigned int unLinesCopy = m_unLinesCleared;

    for (int i = 0; i < 3; i++) {
	vDigits.push_back(unLinesCopy % 10);
	unLinesCopy /= 10;
    }

    for (int i = 0; i < 3; i++) {
	m_asfLinesSprites[i].setTextureRect(sf::IntRect(
	    vDigits[2 - i] * FontWidth, 0, FontWidth, FontHeight
	));
    }
}

bool BlockTris::LineBundle(int nLines) {
    typedef std::array<std::pair<unsigned int, bool>, 20> RowMetaData;

    bool bHasBundle = false;
    std::vector<RowMetaData::iterator> vRowCandidates;

    for (int i = 0; i <= 20 - nLines; i++) {
	for (int j = i; j < i + nLines; j++) {
	    if (m_aRowMetaData[j].first == 10 && !m_aRowMetaData[j].second) {
		vRowCandidates.push_back(m_aRowMetaData.begin() + j);
	    }
	}

	if (vRowCandidates.size() != nLines) {
	    vRowCandidates.clear();
	} else {
	    bHasBundle = true;
	    for (auto& itRow : vRowCandidates) {
		itRow->second = true;
	    }
	    break;
	}
    }

    return bHasBundle;
}

sf::Vector2f BlockTris::LogicalCoordsToScreenCoords(int xLogicalCoord, int yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f BlockTris::LogicalCoordsToScreenCoords(float xLogicalCoord, float yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f BlockTris::LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords) {
    return LogicalCoordsToScreenCoords(sfLogicalCoords.x, sfLogicalCoords.y);
}

BlockTris::BlockTris()
    : GameApp(sf::VideoMode(ScreenWidth, ScreenHeight), "Blocktris", FPSControl::Locked30) {
}

BlockTris::~BlockTris() {
    delete m_pvbWaitingBlocks;
}
