/*----------------------------------------------------------------------------------|
|  Blocktris: A block puzzle game written in C++ using SFML.                        |
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
    m_sfBoardOutline = sf::RectangleShape(sf::Vector2f(BoardSizeX, BoardSizeY));
    m_sfPreviewOutline = sf::RectangleShape(sf::Vector2f(PreviewRectSizeX, PreviewRectSizeY));
    m_sfHeldOutline = sf::RectangleShape(sf::Vector2f(PreviewRectSizeX, PreviewRectSizeX));

    auto szDir = szAssetFolder + "/" + szTextureFolder + "/";

    m_sfBgTexture.loadFromFile(szDir + szBackground);
    m_sfDigitsTexture.loadFromFile(szDir + szDigitAtlas);
    m_sfMovesTexture.loadFromFile(szDir + szMoves);
    PileBlock::sm_sfBlockTexture.loadFromFile(szDir + szBlockAtlas);

    szDir = szAssetFolder + "/" + szSfxFolder + "/";

    for (int i = 0; i < m_asfSoundEffect.size(); i++) {
	m_asfSoundEffect[i].loadFromFile(szDir + aSfxStrings[i]);
	RegisterSoundEffect(i, &m_asfSoundEffect[i]);
    }

    m_sfBackground.setTexture(m_sfBgTexture, true);

    SetupDigits(m_asfScoreSprites.begin(), m_asfScoreSprites.end(), 0.0f);
    SetupDigits(m_asfLevelSprites.begin(), m_asfLevelSprites.end(), DigitLineGap);
    SetupDigits(m_asfLinesSprites.begin(), m_asfLinesSprites.end(), 2.0f * DigitLineGap);
    ReassignDigits(m_asfLevelSprites.begin(), m_asfLevelSprites.end(), m_asfLevelSprites.size(), m_unLevel);

    int i = 0;
    for (auto& prSprite : m_aprHudSprites) {
	prSprite.second = false;
	prSprite.first.setTexture(m_sfMovesTexture);
	prSprite.first.setPosition(FirstMoveLineX, FirstMoveLineY + i * (MoveSpriteVGap + MoveSpriteHeight));
	i++;
    }

    ChangeMoveSpriteRect(SPRITE_B2B, m_aprHudSprites[0].first);
    ChangeMoveSpriteRect(SPRITE_COMBO, m_aprHudSprites[3].first);

    m_pvbWaitingBlocks = new VirtualBag();

    // Get our first mino
    GetNewPiece();
    CalculateHardDropPreview();
    
    // This formats the rectangle holding the board
    SetupOutline(m_sfBoardOutline, BoardOffsetX, BoardOffsetY);
    SetupOutline(m_sfPreviewOutline, PreviewRectOffsetX, PreviewRectOffsetY);
    SetupOutline(m_sfHeldOutline, 
	ScreenWidth - (PreviewRectOffsetX + PreviewRectSizeX), 
	PreviewRectOffsetY);

    // Pre-formatting the blocks that will make up the tetrimino pile
    for (int x = 0; x < CELLS_HORIZONTAL; x++) {
	for (int y = 0; y < CELLS_VERTICAL; y++) {
	    sf::RectangleShape* psfShape = &m_aLogicalBoard[y][x].m_sfBlockViz;
	    psfShape->setSize(sf::Vector2f(SquareSize, SquareSize));
	    psfShape->setPosition(LogicalCoordsToScreenCoords(x, y));
	    psfShape->setFillColor(sf::Color::White);
	    psfShape->setTexture(&PileBlock::sm_sfBlockTexture);
	}
    }

    // Setup the row metadata. Each element will hold how many full spots there are and 
    // whether or not it has been marked as cleared for a particular check iteration.
    for (auto& prRow : m_aRowMetaData) {
	prRow.first = 0;
	prRow.second = false;
    }
    
    // For now, we will set up the initial state as falling
    m_gsState = GameStates::BlockFalling;
    return true;
}

bool BlockTris::OnUpdate(float fFrameTime) {
    // Input affecting game states
    bool bHeldKeyPressed = GetKeyStatus(sf::Keyboard::S) == KeyStatus::Pressed;
    bool bHardDropKeyReleased = GetKeyStatus(sf::Keyboard::Up) == KeyStatus::Released;
    bool bPauseKeyReleased = GetKeyStatus(sf::Keyboard::P) == KeyStatus::Released;
    bool bStateKeyPressed = false;
    bool bJustEnteredPause = false;

    if (m_gsState != GameStates::Pause && m_gsState != GameStates::GameOver) {
	if (bPauseKeyReleased) 
	{
	    m_bSkipInput = true;

	    m_gsSavedState = m_gsState;
	    m_gsState = GameStates::Pause;

	    m_ullGameTicks = 0;

	    MarkSoundForPlay(SFX_PAUSE_IN);
	
	    bJustEnteredPause = true;
	    bStateKeyPressed = true;
	}

	if (
	    bHeldKeyPressed && 
	    !m_bAlreadyPressedHeld && 
	    !bStateKeyPressed && 
	    (m_gsState == GameStates::BlockFalling || m_gsState == GameStates::BlockLockDelay)
	) {
	    m_bAlreadyPressedHeld = true;
	    m_bSkipInput = true;

	    m_gsState = GameStates::HoldPieceAttempt;

	    m_ullGameTicks = 0;
	
	    bStateKeyPressed = true;
	}

	if (
	    bHardDropKeyReleased && 
	    !bStateKeyPressed && 
	    (m_gsState == GameStates::BlockFalling || m_gsState == GameStates::BlockLockDelay)
	) {
	    auto aActiveMinoCoords = m_pActiveTetrimino->GetLogicalCoords()[0];
	    auto aHardDropMinoCoords = m_pHardDropPreview->GetLogicalCoords()[0];

	    m_unCellsHardDropped = aHardDropMinoCoords.y - aActiveMinoCoords.y;

	    std::swap(m_pActiveTetrimino, m_pHardDropPreview);

	    m_pActiveTetrimino->SetAlphaLevel(255);
	    m_fAlphaT = 0.0f;

	    m_gsState = GameStates::BlockHit;

	    m_ullLockDelayTimer = LOCK_DELAY;
	    m_ullGameTicks = 0;
	
	    m_bSkipInput = true;
	}
    }

    // Input affecting the blocks
    if (!m_bSkipInput) {
	bool bDownIsPressed = GetKeyStatus(sf::Keyboard::Down) == KeyStatus::Pressed;

	// Fast drop control
	// TODO: have this not be calculated every frame 
	if (bDownIsPressed &&
	    m_gsState != GameStates::BlockGeneration &&
	    m_gsState != GameStates::BlockHit) 
	{
	    m_unFallRate = std::round(std::max(1.0f, 0.055f * LevelCurveFunction(m_unLevel, m_unLinesPerInterval)));
	} 
	else 
	{
	    m_unFallRate = LevelCurveFunction(m_unLevel, m_unLinesPerInterval);
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
	|   is called Delayed Auto Shift.                                                 |
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
	if (m_bKeyPressedInitialLeft || m_bKeyPressedInitialRight)
	{
	    m_ullTetriminoMoveTimer = 0;
	}

	// Check this next frame
	m_aPrevFrameKeyStates = m_aCurrFrameKeyStates;

	bool bMovedOrRotated = false;
	bool bMoveLeft = m_bKeyHeldLeft || m_bKeyPressedInitialLeft;
	bool bMoveRight = m_bKeyHeldRight || m_bKeyPressedInitialRight;

	// Process input if the timer is tripped
	if (
	    (m_gsState == GameStates::BlockFalling || m_gsState == GameStates::BlockLockDelay) && 
	    m_ullTetriminoMoveTimer == 0
	) {
	    
	    if (m_pActiveTetrimino->TranslateTetriminoHorizontal(bMoveLeft, bMoveRight, m_aLogicalBoard)) 
	    {
		m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
		CalculateHardDropPreview();
		bMovedOrRotated = true;
	    }

	    if (m_bKeyPressedInitialLeft || m_bKeyPressedInitialRight) 
	    {
		m_unHorizontalMoveRate = DAS_INITIAL;
	    }
	    else 
	    {
		m_unHorizontalMoveRate = DAS_RATE;
	    }
	}

	bool bLeftRotation = GetKeyStatus(sf::Keyboard::Z) == KeyStatus::Pressed;
	bool bRightRotation = GetKeyStatus(sf::Keyboard::X) == KeyStatus::Pressed;
	bool bRotationKeyPressed = bLeftRotation || bRightRotation;

	if (
	    bRotationKeyPressed && 
	    !m_bRotationKeyHeld && 
	    (m_gsState == GameStates::BlockFalling || m_gsState == GameStates::BlockLockDelay)
	) {
	    sf::Vector2f sfRotationCoefficients = bLeftRotation ?
		sf::Vector2f(-1.0f, 1.0f) : sf::Vector2f(1.0f, -1.0f);

	    if (m_pActiveTetrimino->RotateTetrimino(sfRotationCoefficients, m_aLogicalBoard)) 
	    {
		m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
		CalculateHardDropPreview();
		bMovedOrRotated = true;
		m_tsTPieceSpin = m_pActiveTetrimino->CheckTSpin(m_aLogicalBoard);
		if (m_tsTPieceSpin != T_SpinTypes::NoSpin) 
		{
		    MarkSoundForPlay(SFX_DETECT_T_SPIN);
		}
	    }
	    
	    m_bRotationKeyHeld = true;
	} 
	else if (!bRotationKeyPressed) 
	{
	    m_bRotationKeyHeld = false;
	}

	if (bMovedOrRotated) 
	{
	    MarkSoundForPlay(SFX_MOVE_ROT);

	    if (m_gsState == GameStates::BlockLockDelay) 
	    {
		m_pActiveTetrimino->SetAlphaLevel(255);
		m_fAlphaT = 0.0f;
		m_ullLockDelayTimer = LOCK_DELAY;
	    }
	}
    }

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
	if (m_pActiveTetrimino)
	    m_bAlreadyPressedHeld = false;

	m_unCellsFastDropped = 0;
	m_unCellsHardDropped = 0;
	m_fAlphaT = 0.0f;
	m_tsTPieceSpin = T_SpinTypes::NoSpin;

	GetNewPiece();
	CalculateHardDropPreview();

	m_bRefreshPreview = true;

	m_gsState = GameStates::BlockFalling;
    }
	break;
    case GameStates::BlockFalling:
    {
	if (m_ullGameTicks % m_unFallRate == 0) 
	{
	    for (int k = 0; k < m_unLinesPerInterval; k++) 
	    {
		DoMoveDown(m_pActiveTetrimino, true);
	    }
	}
    }
	break;
    case GameStates::BlockHit:
    {
	if (m_ullBlockCollisionTimer == 0) {
	    /*---------------------------------------------------------------|
	    |   Take the tetrimino and "throw" its blocks onto the pile      |
	    |   In this case, we make the blocks where the tetrimino rests   |
	    |   visible in the logical board.                                |
	    |---------------------------------------------------------------*/

	    bool bBlockLandedOutside = false;

	    PieceTypes ptTetrimino = m_pActiveTetrimino->GetPieceType();

	    for (auto& sfPileBlockLoc : m_pActiveTetrimino->GetLogicalCoords()) 
	    {
		if (sfPileBlockLoc.y < 0) 
		{
		    bBlockLandedOutside = true;
		    continue;
		}

		sf::IntRect sfNewSpriteRect = sf::IntRect(static_cast<int>(ptTetrimino) * SquareSize, 0, SquareSize, SquareSize);
		m_aLogicalBoard[sfPileBlockLoc.y][sfPileBlockLoc.x].m_bHidden = false;
		m_aLogicalBoard[sfPileBlockLoc.y][sfPileBlockLoc.x].m_sfBlockViz.setTextureRect(
		    sfNewSpriteRect
		);
		m_aLogicalBoard[sfPileBlockLoc.y][sfPileBlockLoc.x].m_ptaAnim->SetNewBaseSprite(
		    sfNewSpriteRect
		);
		m_aRowMetaData[sfPileBlockLoc.y].first++;
	    }

	    if (bBlockLandedOutside) 
	    {
		m_gsState = GameStates::GameOver;

		for (auto& aRow : m_aLogicalBoard) 
		{
		    for (auto& pbBlock : aRow) 
		    {
			if (!pbBlock.m_bHidden)
			    pbBlock.m_sfBlockViz.setFillColor(sf::Color(128, 128, 128));
		    }
		}

		break;
	    }

	    if (CheckLineClears()) 
	    {
		m_gsState = GameStates::LineClearing;
	    } 
	    else 
	    {
		m_gsState = GameStates::BlockGeneration;
	    }

	    m_ullHudSpriteTimer = HUD_SPRITE_DELAY;
	    m_ullBlockCollisionTimer = BLOCK_COLLISION_DELAY;
	} 
	else 
	{
	    if (m_ullBlockCollisionTimer == BLOCK_COLLISION_DELAY) 
	    {
		m_pActiveTetrimino->m_ptaAnim->ArmAnimation();
		MarkSoundForPlay(SFX_LOCK);
	    }

	    m_ullBlockCollisionTimer--;
	}

	m_bSkipInput = false;
    }
	break;
    case GameStates::LineClearing: 
    {
	if (m_ullLineClearTimer == 0) 
	{
	    ClearLines();
	    MarkSoundForPlay(SFX_LINES_DESTROYED);
	    m_vLineClearRowIndexes.clear();
	    m_gsState = GameStates::BlockGeneration;
	    m_ullLineClearTimer = LINE_CLEAR_DELAY;
	} 
	else 
	{
	    if (m_ullLineClearTimer == LINE_CLEAR_DELAY) 
	    {
		for (size_t sztRowIndex : m_vLineClearRowIndexes) 
		{
		    for (auto& pbPileBlock : m_aLogicalBoard[sztRowIndex]) 
		    {
			pbPileBlock.m_ptaAnim->ArmAnimation();
		    }
		}
	    }
	    m_ullLineClearTimer--;
	}
    }
	break;
    case GameStates::HoldPieceAttempt:
    {
	m_ullLockDelayTimer = LOCK_DELAY;
	std::swap(m_pHeldTetrimino, m_pActiveTetrimino);

	m_fAlphaT = 0.0f;
	m_tsTPieceSpin = T_SpinTypes::NoSpin;
	m_pHeldTetrimino->SetAlphaLevel(255);

	if (!m_pActiveTetrimino) 
	{
	    GetNewPiece();
	    CalculateHardDropPreview();
	    m_bRefreshPreview = true;
	} 
	else 
	{
	    m_pActiveTetrimino->ResetPieceAndPivot();
	    m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
	    CalculateHardDropPreview();
	}

	m_gsState = GameStates::BlockFalling;

	m_bSkipInput = false;
	m_bHeldChanged = true;
    }
	break;
    case GameStates::BlockLockDelay:
    {
	auto& sfActiveCoords = m_pActiveTetrimino->GetLogicalCoords();
	auto& sfHardDropCoords = m_pHardDropPreview->GetLogicalCoords();

	int nAlphaLevel = std::max(std::min(std::round(255.0f * (0.35f * cos(2.0f * M_PI * m_fAlphaT) + 0.65f)), 255.0), 0.0);
	m_fAlphaT += fFrameTime;
	m_pActiveTetrimino->SetAlphaLevel(nAlphaLevel);

	for (int i = 0; i < 4; i++) 
	{
	    if (sfActiveCoords[i] != sfHardDropCoords[i]) 
	    {
		m_gsState = GameStates::BlockFalling;
		m_pActiveTetrimino->SetAlphaLevel(255);
		m_fAlphaT = 0.0f;
		m_ullLockDelayTimer = LOCK_DELAY;
		break;
	    }
	}

	if (m_ullLockDelayTimer == 0) 
	{
	    m_ullLockDelayTimer = LOCK_DELAY;
	    m_pActiveTetrimino->SetAlphaLevel(255);
	    m_gsState = GameStates::BlockHit;
	} 
	else 
	{
	    m_ullLockDelayTimer--;
	}
    }
	break;
    case GameStates::Pause:
    {
	if (bPauseKeyReleased && !bJustEnteredPause) 
	{
	    m_bSkipInput = false;
	    m_gsState = m_gsSavedState;
	    MarkSoundForPlay(SFX_PAUSE_OUT);
	}

	bJustEnteredPause = false;
    }
	break;
    case GameStates::GameOver:
    {
	// Stick around here forever (for now)
    }
	break;
    default:
	// Something has gone very wrong if we end up here
	break;
    }

    // Advance the timers by 1 tick (1/60th of one second)
    m_ullGameTicks++;
    m_ullTetriminoMoveTimer = (m_ullTetriminoMoveTimer + 1) % m_unHorizontalMoveRate;

    if (m_ullHudSpriteTimer == 0) 
    {
	for (auto& prSprite : m_aprHudSprites) 
	{
	    prSprite.second = false;
	}
    } 
    else 
    {
	m_ullHudSpriteTimer--;
    }

    // Drawing routine
    PushDrawableObject(&m_sfBackground);
    PushDrawableObject(&m_sfBoardOutline);
    PushDrawableObject(&m_sfPreviewOutline);
    PushDrawableObject(&m_sfHeldOutline);
    DrawPreviewAndHeld();

    if (m_gsState == GameStates::BlockHit)
	m_pActiveTetrimino->DoAnimation();

    if (m_gsState == GameStates::LineClearing) {
	for (size_t sztRowIndex : m_vLineClearRowIndexes) 
	{
	    for (auto& pbPileBlock : m_aLogicalBoard[sztRowIndex]) 
	    {
		if (pbPileBlock.m_ptaAnim->IsPlaying()) 
		{
		    auto sfAnimFrameRect = pbPileBlock.m_ptaAnim->StepAnimation();
		    pbPileBlock.m_sfBlockViz.setTextureRect(sfAnimFrameRect);
		}
	    }
	}
    }

    if (m_gsState != GameStates::BlockGeneration && m_gsState != GameStates::GameOver) 
    {
	if (m_gsState == GameStates::BlockFalling)
	    DrawTetrimino(m_pHardDropPreview, sf::IntRect(0, 0, 10, 20));

	DrawTetrimino(m_pActiveTetrimino, sf::IntRect(0, 0, 10, 20));
    }

    DrawArrayOfObjects(m_asfScoreSprites.begin(), m_asfScoreSprites.end());
    DrawArrayOfObjects(m_asfLevelSprites.begin(), m_asfLevelSprites.end());
    DrawArrayOfObjects(m_asfLinesSprites.begin(), m_asfLinesSprites.end());

    for (auto& prSprite : m_aprHudSprites) {
	if (prSprite.second)
	    PushDrawableObject(&prSprite.first);
    }

    DrawPile();
    return true;
}

void BlockTris::DrawPile() {
    for (auto& aLine : m_aLogicalBoard) 
    {
	for (auto& scPileBlock : aLine) 
	{
	    if (!scPileBlock.m_bHidden) {
		PushDrawableObject(&scPileBlock.m_sfBlockViz);
	    }
	}
    }
}

void BlockTris::CalculateHardDropPreview() {
    while (!DoMoveDown(m_pHardDropPreview, false));
    m_pHardDropPreview->SetAlphaLevel(128);
}

void BlockTris::SetupOutline(sf::RectangleShape& sfRect, float fxOffset, float fyOffset) {
    sfRect.setFillColor(sf::Color::Transparent);
    sfRect.setOutlineColor(sf::Color::White);
    sfRect.setOutlineThickness(5.0f);
    sfRect.setPosition(fxOffset, fyOffset);
}

void BlockTris::DrawTetrimino(std::shared_ptr<Tetrimino> pttMino, sf::IntRect sfBoundingRect) {
    auto itMinoLogicalCoord = pttMino->GetLogicalCoords().begin();
    for (auto& sfBlock : pttMino->GetPieceShapes()) 
    {
	if (itMinoLogicalCoord->y >= 0)
	    PushDrawableObject(&sfBlock);

	itMinoLogicalCoord++;
    }
}

void BlockTris::DrawTetrimino(std::shared_ptr<Tetrimino> pttMino) {
    for (auto& sfBlock : pttMino->GetPieceShapes()) {
	PushDrawableObject(&sfBlock);
    }
}

bool BlockTris::CheckLineClears() {
    bool bLinesCleared = false;
    bool bFourClear = false;
    bool bTripleClear = false;
    bool bDoubleClear = false;
    int nSinglesClear = 0;

    bFourClear = LineBundle(4);
    bTripleClear = LineBundle(3);
    
    bLinesCleared = bFourClear || bTripleClear;

    if (!bLinesCleared) {
	bDoubleClear = LineBundle(2);
	nSinglesClear = LineBundle(1) + LineBundle(1);
	bLinesCleared = bDoubleClear || nSinglesClear;
    }

    m_bCombo = bLinesCleared && m_bClearedLinesPreviously;

    m_ullScore += CalculateScore(bFourClear, bTripleClear, bDoubleClear, nSinglesClear, m_vLineClearRowIndexes.size());
    UpdateText();

    m_bClearedLinesPreviously = bLinesCleared;
    m_unLinesCleared += m_vLineClearRowIndexes.size();

    return bLinesCleared;
}

void BlockTris::ClearLines() {
    std::sort(m_vLineClearRowIndexes.begin(), m_vLineClearRowIndexes.end());

    for (size_t& sztRowIdx : m_vLineClearRowIndexes) {
	m_aRowMetaData[sztRowIdx].first = 0;
	m_aRowMetaData[sztRowIdx].second = false;

	for (auto& plBlock : m_aLogicalBoard[sztRowIdx]) {
	    plBlock.m_bHidden = true;
	}

	for (size_t i = sztRowIdx; i >= 1; i--) {
	    std::swap(m_aRowMetaData[i], m_aRowMetaData[i - 1]);
	    std::swap(m_aLogicalBoard[i], m_aLogicalBoard[i - 1]);
	}
    }

    for (int y = 0; y < CELLS_VERTICAL; y++) {
	for (int x = 0; x < CELLS_HORIZONTAL; x++) {
	    m_aLogicalBoard[y][x].m_sfBlockViz.setPosition(
		LogicalCoordsToScreenCoords(x, y)
	    );
	}
    }

    RecalculateLevel();
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
	DrawTetrimino(m_pHeldTetrimino);
    }

    for (auto& ptmPreview : m_aPreviewTetriminos) {
	DrawTetrimino(ptmPreview);
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

void BlockTris::UpdateText() {
    if (m_unLinesCleared > MAX_LINES)
	m_unLinesCleared = MAX_LINES;

    if (m_ullScore > MAX_SCORE)
	m_ullScore = MAX_SCORE;

    auto itLinesSprBegin = m_asfLinesSprites.begin(), itLinesSprEnd = m_asfLinesSprites.end();
    auto itLevelsSprBegin = m_asfLevelSprites.begin(), itLevelsSprEnd = m_asfLevelSprites.end();
    auto itScoreSprBegin = m_asfScoreSprites.begin(), itScoreSprEnd = m_asfScoreSprites.end();

    ReassignDigits(itLinesSprBegin, itLinesSprEnd, m_asfLinesSprites.size(), m_unLinesCleared);
    ReassignDigits(itLevelsSprBegin, itLevelsSprEnd, m_asfLevelSprites.size(), m_unLevel);
    ReassignDigits(itScoreSprBegin, itScoreSprEnd, m_asfScoreSprites.size(), m_ullScore);
}

void BlockTris::RecalculateLevel() {
    if (m_unLinesCleared / (m_unLevel * 10) > 0 && m_unLevel < MAX_LEVEL) {
	m_unLevel++;
	MarkSoundForPlay(SFX_LEVEL_UP);
	m_unFallRate = LevelCurveFunction(m_unLevel, m_unLinesPerInterval);
    }
}

void BlockTris::GetNewPiece() {
    m_pActiveTetrimino = m_pvbWaitingBlocks->GetNextPiece();
    m_aPreviewTetriminos = m_pvbWaitingBlocks->PeekNextPieces();
    m_pHardDropPreview = std::make_shared<Tetrimino>(*m_pActiveTetrimino);
}

bool BlockTris::DoMoveDown(std::shared_ptr<Tetrimino> pTetrimino, bool bIsActiveTetrimino) {
    auto& vTetriminoLogicalCoords = pTetrimino->GetLogicalCoords();
    bool bVerticalCollision = false;

    for (int i = 0; i < 4; i++) {
	int xTest = vTetriminoLogicalCoords[i].x;
	int yTest = vTetriminoLogicalCoords[i].y + 1;

	if (yTest >= CELLS_VERTICAL || (yTest >= 0 && !m_aLogicalBoard[yTest][xTest].m_bHidden))
	{
	    bVerticalCollision = true;
	    break;
	}
    }

    if (!bVerticalCollision)
    {
	pTetrimino->MoveDown();

	if (bIsActiveTetrimino) 
	{
	    if (GetKeyStatus(sf::Keyboard::Down) == KeyStatus::Pressed) 
	    {
		m_unCellsFastDropped++;
	    }

	    CalculateHardDropPreview();
	}
    } 
    else if (bIsActiveTetrimino)
    {
	m_gsState = GameStates::BlockLockDelay;
	MarkSoundForPlay(SFX_HIT);
    }

    return bVerticalCollision;
}

unsigned int BlockTris::CalculateScore(bool bFour, bool bTriple, bool bDouble, int nSingles, int nLinesCleared) {
    unsigned int unSoftAndHardDropPts = m_unCellsFastDropped + 2.0 * m_unCellsHardDropped;
    unsigned int unSinglesPts = 100 * nSingles;
    unsigned int unDoublePts = 300 * bDouble;
    unsigned int unTriplePts = 500 * bTriple;
    unsigned int unQuadPts = 800 * bFour;
    unsigned int unT_SpinPts = 0;
    unsigned int unTotal = 0;

    if (m_tsTPieceSpin == T_SpinTypes::FullSpin) 
    {
	unT_SpinPts = (nLinesCleared + 1) * 400;
	ChangeMoveSpriteRect(SPRITE_T_SPIN, m_aprHudSprites[1].first);
    } 
    else if (m_tsTPieceSpin == T_SpinTypes::MiniSpin) 
    {
	unT_SpinPts = std::round(std::pow(2.0, nLinesCleared)) * 100;
	ChangeMoveSpriteRect(SPRITE_MINI_T_SPIN, m_aprHudSprites[1].first);
    }

    if (unT_SpinPts > 0) 
    {
	MarkSoundForPlay(SFX_T_SPIN_AWARD);

	m_bDifficultMove = nLinesCleared ? true : m_bDifficultMove;
	m_aprHudSprites[1].second = true;

	if (nLinesCleared > 0) 
	{
	    ChangeMoveSpriteRect(nLinesCleared, m_aprHudSprites[2].first);
	    m_aprHudSprites[2].second = true;;
	}
    } 
    else 
    {
	if (bFour)
	{
	    m_bDifficultMove = true;
	    MarkSoundForPlay(SFX_LINE_CLEAR_QUADRUPLE);
	    ChangeMoveSpriteRect(SPRITE_QUAD, m_aprHudSprites[2].first);
	    m_aprHudSprites[2].second = true;
	}
	else if (bTriple || bDouble || nSingles)
	{
	    MarkSoundForPlay(SFX_LINE_CLEAR_NORMAL);

	    if (bTriple) {
		ChangeMoveSpriteRect(SPRITE_TRIPLE, m_aprHudSprites[2].first);
	    } 
	    else if (bDouble) 
	    {
		ChangeMoveSpriteRect(SPRITE_DOUBLE, m_aprHudSprites[2].first);
	    }
	    else 
	    {
		ChangeMoveSpriteRect(SPRITE_SINGLE, m_aprHudSprites[2].first);
	    }

	    m_aprHudSprites[2].second = true;
	}
    }

    if (nLinesCleared > 0) 
    {
	m_bBackToBack = m_bDifficultMove && m_bLastDidDifficultMove;
	m_bLastDidDifficultMove = m_bDifficultMove;
	m_bDifficultMove = false;

	if (m_bBackToBack) 
	{
	    m_aprHudSprites[0].second = true;
	    if (m_tsTPieceSpin == T_SpinTypes::MiniSpin && nLinesCleared == 2)
	    {
		unT_SpinPts += 200;
	    }
	    else if ((m_tsTPieceSpin == T_SpinTypes::FullSpin && nLinesCleared == 1) || bFour)
	    {
		unTotal += 400;
	    }
	    else if (m_tsTPieceSpin == T_SpinTypes::FullSpin && nLinesCleared >= 3)
	    {
		unT_SpinPts = (nLinesCleared + 1) * 600;
	    }
	}
    }

    unTotal = unSinglesPts + unDoublePts + unTriplePts + unQuadPts + unT_SpinPts;

    if (m_bCombo) 
    {
	unTotal += 50;
	m_aprHudSprites[3].second = true;
	MarkSoundForPlay(SFX_COMBO);
    }

    unTotal *= m_unLevel;
    unTotal += unSoftAndHardDropPts;

    return unTotal;
}

bool BlockTris::LineBundle(int nLines) {
    typedef std::array<std::pair<unsigned int, bool>, CELLS_VERTICAL> RowMetaData;

    bool bHasBundle = false;
    std::vector<RowMetaData::iterator> vRowCandidates;

    for (int i = 0; i <= CELLS_VERTICAL - nLines; i++) 
    {
	for (int j = i; j < i + nLines; j++) 
	{
	    if (m_aRowMetaData[j].first == CELLS_HORIZONTAL && !m_aRowMetaData[j].second) 
	    {
		vRowCandidates.push_back(m_aRowMetaData.begin() + j);
	    }
	}

	if (vRowCandidates.size() != nLines) 
	{
	    vRowCandidates.clear();
	} 
	else 
	{
	    bHasBundle = true;
	    for (auto& itRow : vRowCandidates) 
	    {
		itRow->second = true;
		m_vLineClearRowIndexes.push_back(std::distance(m_aRowMetaData.begin(), itRow));
	    }
	    break;
	}
    }

    return bHasBundle;
}

unsigned int BlockTris::LevelCurveFunction(unsigned int nLevel, unsigned int& nCellsToDrop) {
    float fInterval = (double) ONE_SECOND * std::pow(0.8 - ((nLevel - 1.0) * 0.007), nLevel - 1.0);

    if (fInterval <= 1) 
    {
	nCellsToDrop = std::min(20u, (unsigned int) std::round(1.0f / fInterval));
	return 1;
    } 
    else 
    {
	return std::round(fInterval);
    }
}

void BlockTris::ChangeMoveSpriteRect(unsigned int nEntry, sf::Sprite& sfSprite) {
    sf::IntRect sfTextureRect = sf::IntRect(0, nEntry * MoveSpriteHeight, MoveSpriteWidth, MoveSpriteHeight);
    sfSprite.setTextureRect(sfTextureRect);
}

BlockTris::BlockTris()
    : GameApp(sf::VideoMode(ScreenWidth, ScreenHeight), "Blocktris", FPSControl::Locked60) {
}

BlockTris::~BlockTris() {
    delete m_pvbWaitingBlocks;
}
