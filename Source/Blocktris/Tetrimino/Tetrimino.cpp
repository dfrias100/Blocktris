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

#include "Tetrimino.h"

// NORMAL PIECE WALL KICKS
/*------------------------------------------|
|           1        2       3        4	    |
|   0>>1 (-1, 0) (-1,  1) (0, -2) (-1, -2)  |
|   1>>0 ( 1, 0) ( 1, -1) (0,  2) ( 1,  2)  |
|   					    |
|   1>>2 ( 1, 0) ( 1, -1) (0,  2) ( 1,  2)  |
|   2>>1 (-1, 0) (-1,  1) (0, -2) (-1, -2)  |
|   					    |
|   2>>3 ( 1, 0) ( 1,  1) (0, -2) ( 1, -2)  |
|   3>>2 (-1, 0) (-1, -1) (0,  2) (-1,  2)  |
|   					    |
|   3>>0 (-1, 0) (-1, -1) (0,  2) (-1,  2)  |
|   0>>3 ( 1, 0) ( 1,  1) (0, -2) ( 1, -2)  |
|------------------------------------------*/

const std::array<std::array<sf::Vector2i, 4>, 4> Tetrimino::m_aWallKickData = {
    {   
	{ sf::Vector2i(-1, 0), sf::Vector2i(-1, -1), sf::Vector2i(0,  2), sf::Vector2i(-1,  2) }, // 0 >> 1
	{ sf::Vector2i( 1, 0), sf::Vector2i( 1,  1), sf::Vector2i(0, -2), sf::Vector2i( 1, -2) }, // 1 >> 2
	{ sf::Vector2i(-1, 0), sf::Vector2i(-1,  1), sf::Vector2i(0, -2), sf::Vector2i(-1, -2) }, // 3 >> 2
	{ sf::Vector2i( 1, 0), sf::Vector2i( 1, -1), sf::Vector2i(0,  2), sf::Vector2i( 1,  2) }  // 0 >> 3
    }
};

// I PIECE WALL KICKS
/*------------------------------------------|
|           1        2       3        4	    |
|   0>>1 (-2, 0) ( 1, 0) (-2, -1) ( 1,  2)  |
|   1>>0 ( 2, 0) (-1, 0) ( 2,  1) (-1, -2)  |
|	 				    |
|   1>>2 (-1, 0) ( 2, 0) (-1,  2) ( 2, -1)  |
|   2>>1 ( 1, 0) (-2, 0) ( 1, -2) (-2,  1)  |
|	 				    |
|   2>>3 ( 2, 0) (-1, 0) ( 2,  1) (-1, -2)  |
|   3>>2 (-2, 0) ( 1, 0) (-2, -1) ( 1,  2)  |
|	 				    |
|   3>>0 ( 1, 0) (-2, 0) ( 1, -2) (-2,  1)  |
|   0>>3 (-1, 0) ( 2, 0) (-1,  2) ( 2, -1)  |
|------------------------------------------*/

const std::array<std::array<sf::Vector2i, 4>, 4> Tetrimino::m_aWallKickDataIPiece = {
    {
	{ sf::Vector2i(-2, 0), sf::Vector2i( 1, 0), sf::Vector2i(-2,  1), sf::Vector2i( 1, -2) }, // 0 >> 1
	{ sf::Vector2i( 2, 0), sf::Vector2i(-1, 0), sf::Vector2i( 2, -1), sf::Vector2i(-1,  2) }, // 2 >> 3
	{ sf::Vector2i(-1, 0), sf::Vector2i( 2, 0), sf::Vector2i(-1, -2), sf::Vector2i( 2,  1) }, // 0 >> 3
	{ sf::Vector2i( 1, 0), sf::Vector2i(-2, 0), sf::Vector2i( 1,  2), sf::Vector2i(-2, -1) }  // 2 >> 1
    }
};

// First 2 vec2is: Corners in front, Last 2 vec2is: Corners in back
const std::array<std::array<sf::Vector2i, 4>, 4> Tetrimino::m_aT_SpinCorners = {
    {
	{ sf::Vector2i(-1, -1), sf::Vector2i( 1, -1), sf::Vector2i(-1,  1), sf::Vector2i( 1,  1) }, // Orientation 0
	{ sf::Vector2i( 1, -1), sf::Vector2i( 1,  1), sf::Vector2i(-1, -1), sf::Vector2i(-1,  1) }, // Orientation 1
	{ sf::Vector2i(-1,  1), sf::Vector2i( 1,  1), sf::Vector2i(-1, -1), sf::Vector2i( 1, -1) }, // Orientation 2
	{ sf::Vector2i(-1, -1), sf::Vector2i(-1,  1), sf::Vector2i( 1, -1), sf::Vector2i( 1,  1) }  // Orientation 3
    }
};

sf::Texture Tetrimino::sm_sfBlockTexture;

const sf::Vector2i Tetrimino::ReturnWallKickVector(int nPrevOrientation, int nNewOrientation, int nTestNo) {
    switch (nNewOrientation) {
    case 0:
	switch (nPrevOrientation)
	{
	case 1:
	    return m_aWallKickData[1][nTestNo];
	case 3:
	    return m_aWallKickData[2][nTestNo];
	}
	break;
    case 1:
	switch (nPrevOrientation)
	{
	case 0:
	case 2:
	    return m_aWallKickData[0][nTestNo];
	}
	break;
    case 2:
	switch (nPrevOrientation)
	{
	case 1:
	    return m_aWallKickData[1][nTestNo];
	case 3:
	    return m_aWallKickData[2][nTestNo];
	}
	break;
    case 3:
	switch (nPrevOrientation)
	{
	case 0:
	case 2:
	    return m_aWallKickData[3][nTestNo];
	}
	break;
    }

    return sf::Vector2i(0, 0);
}

const sf::Vector2i Tetrimino::ReturnWallKickVectorIPiece(int nPrevOrientation, int nNewOrientation, int nTestNo) {
    switch (nNewOrientation) {
    case 0:
	switch (nPrevOrientation)
	{
	case 1:
	    return m_aWallKickDataIPiece[1][nTestNo];
	case 3:
	    return m_aWallKickDataIPiece[3][nTestNo];
	}
	break;
    case 1:
	switch (nPrevOrientation)
	{
	case 0:
	    return m_aWallKickDataIPiece[0][nTestNo];
	case 2:
	    return m_aWallKickDataIPiece[3][nTestNo];
	}
	break;
    case 2:
	switch (nPrevOrientation)
	{
	case 1:
	    return m_aWallKickDataIPiece[2][nTestNo];
	case 3:
	    return m_aWallKickDataIPiece[0][nTestNo];
	}
	break;
    case 3:
	switch (nPrevOrientation)
	{
	case 0:
	    return m_aWallKickDataIPiece[2][nTestNo];
	case 2:
	    return m_aWallKickDataIPiece[1][nTestNo];
	}
	break;
    }

    return sf::Vector2i(0, 0);
}

Tetrimino::Tetrimino(PieceTypes ptType) {
    std::vector<unsigned int> vAnim = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    sf::IntRect sfBaseSprite = sf::IntRect(static_cast<int>(ptType) * SquareSize, 0, SquareSize, SquareSize);
    m_ptaAnim = new TetriminoAnimation(vAnim, sfBaseSprite);
    m_sfPiecePivot = m_aPiecePivots[static_cast<int>(ptType)];
    m_vCurrentPiece = m_aPieces[static_cast<int>(ptType)];
    m_CurrentPieceType = ptType;

    for (int i = 0; i < 4; i++) {
	m_asfPieceViz[i].setPosition(
	    LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
	m_asfPieceViz[i].setFillColor(
	    sf::Color::White
	);
	m_asfPieceViz[i].setSize(sf::Vector2f(SquareSize, SquareSize));
	m_asfPieceViz[i].setTexture(&PileBlock::sm_sfBlockTexture);
	m_asfPieceViz[i].setTextureRect(sf::IntRect(static_cast<int>(ptType) * SquareSize, 0, SquareSize, SquareSize));
    }
}

void Tetrimino::TranslatePivot(sf::Vector2f sfTranslation) {
    m_sfPiecePivot += sfTranslation;
}

bool Tetrimino::RotateTetrimino(sf::Vector2f sfRotationCoefficents, 
    Board& brdGameField) {
    if (m_CurrentPieceType == PieceTypes::O_Piece)
	return false;

    std::vector<sf::Vector2i> vTetriminoCoords;

    for (auto sfLogicalCoord : m_vCurrentPiece) {
	// Some pivots are fractional, to help the math work out better
	sf::Vector2f sfLogicalCoordFloats = sf::Vector2f(sfLogicalCoord.x, sfLogicalCoord.y);

	// Subtract the pivot from the point
	sfLogicalCoordFloats -= m_sfPiecePivot;

	// Coordinate will rotate up along Quadrant I, but on the logical board
	// the Y-axis grows downwards, this will give us the correct numbers
	sfLogicalCoordFloats.y *= -1.0f;

	// Swap coordinates 
	std::swap(sfLogicalCoordFloats.x, sfLogicalCoordFloats.y);

	// Negate X axis as per rotation matrix
	sfLogicalCoordFloats.x *= sfRotationCoefficents.x;

	// Or negate the Y axis ...
	sfLogicalCoordFloats.y *= sfRotationCoefficents.y;

	// Bring it back to our coordinate space
	sfLogicalCoordFloats.y *= -1.0f;

	// Add the pivot back
	sfLogicalCoordFloats += m_sfPiecePivot;

	// Push back the rotated point, with points rounded in case there's
	// some funky float result
	vTetriminoCoords.push_back(
	    sf::Vector2i(
		std::round(sfLogicalCoordFloats.x),
		std::round(sfLogicalCoordFloats.y)
	    )
	);
    }

    bool bCanRotate;
    int nRotationIncrement = sfRotationCoefficents.y == -1 ? 1 : -1;
    int nPossibleRotation = (NUM_ORIENTATIONS + ((m_nCurrentOrientation + nRotationIncrement) % NUM_ORIENTATIONS)) % NUM_ORIENTATIONS;
    sf::Vector2i sfTranslationVector = sf::Vector2i(0, 0);

    for (int i = 0; i < NUM_ROTATE_TESTS; i++) {
	if (i != 0) 
	{
	    if (m_CurrentPieceType != PieceTypes::I_Piece)
		sfTranslationVector = ReturnWallKickVector(m_nCurrentOrientation, nPossibleRotation, i - 1);
	    else
		sfTranslationVector = ReturnWallKickVectorIPiece(m_nCurrentOrientation, nPossibleRotation, i - 1);
	}

	bCanRotate = true;

	for (auto& sfLogicalCoord : vTetriminoCoords) {
	    sfLogicalCoord += sfTranslationVector;
	    if (
		sfLogicalCoord.x <  0                ||
		sfLogicalCoord.x >= CELLS_HORIZONTAL ||
		sfLogicalCoord.y >= CELLS_VERTICAL   ||
		(
		    sfLogicalCoord.y >= 0 && 
		    !brdGameField[sfLogicalCoord.y][sfLogicalCoord.x].m_bHidden
		)
	    ) {
		bCanRotate = false;
	    }
	}

	if (bCanRotate) {
	    m_nPrevOrientation = m_nCurrentOrientation;
	    m_nCurrentOrientation = nPossibleRotation;
	    m_nLastRotationTest = i;

	    TranslatePivot(sf::Vector2f(sfTranslationVector));
	    m_vCurrentPiece = vTetriminoCoords;

	    for (int j = 0; j < 4; j++) 
	    {
		m_asfPieceViz[j].setPosition(
		    LogicalCoordsToScreenCoords(m_vCurrentPiece[j])
		);
	    }

	    return true;
	} 
	else 
	{
	    for (auto& sfLogicalCoord : vTetriminoCoords) 
	    {
		sfLogicalCoord -= sfTranslationVector;
	    }
	}
    }

    return false;
}

bool Tetrimino::TranslateTetriminoHorizontal(bool bLeft, bool bRight, Board& brdGameField) {
    if ((!bLeft && !bRight) || (bLeft && bRight))
	return false;

    auto vTetriminoLogicalCoordsTest = m_vCurrentPiece;
    bool bHorizontalCollision = false;

    for (auto& sfCoords : vTetriminoLogicalCoordsTest) {
	if (bLeft) sfCoords.x--;
	if (bRight) sfCoords.x++;

	if (sfCoords.x < 0 || sfCoords.x >= CELLS_HORIZONTAL || (sfCoords.y >= 0 && !brdGameField[sfCoords.y][sfCoords.x].m_bHidden)) 
	{
	    bHorizontalCollision = true;
	    return false;
	}
    }

    if (!bHorizontalCollision) 
    {
	if (bLeft) TranslatePivot({ -1, 0 });
	if (bRight) TranslatePivot({ 1, 0 });

	m_vCurrentPiece = vTetriminoLogicalCoordsTest;
	for (int i = 0; i < 4; i++) 
	{
	    m_asfPieceViz[i].setPosition(
		LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	    );
	}
    }

    return true;
}

const std::vector<sf::Vector2i>& Tetrimino::GetLogicalCoords() {
    return m_vCurrentPiece;
}

std::array<sf::RectangleShape, 4>& Tetrimino::GetPieceShapes() {
    return m_asfPieceViz;
}

void Tetrimino::SetAlphaLevel(int nAlphaLevel) {
    for (auto& sfShape : m_asfPieceViz) {
	auto sfColor = sfShape.getFillColor();
	sfColor.a = nAlphaLevel;
	sfShape.setFillColor(sfColor);
    }
}

sf::Vector2f Tetrimino::GetPivot() {
    return m_sfPiecePivot;
}

PieceTypes Tetrimino::GetPieceType() {
    return m_CurrentPieceType;
}

T_SpinTypes Tetrimino::CheckTSpin(Board& brdGameField) {
    if (m_CurrentPieceType != PieceTypes::T_Piece)
	return T_SpinTypes::NoSpin;

    auto& aTestCorners = m_aT_SpinCorners[m_nCurrentOrientation];
    sf::Vector2i sfPivot = sf::Vector2i(m_sfPiecePivot);

    int nBackCorners = 0;
    int nFrontCorners = 0;
    
    for (int i = 0; i < 4; i++) {
	sfPivot += aTestCorners[i];

	if (
	    sfPivot.x >= CELLS_HORIZONTAL ||
	    sfPivot.x <  0                ||
	    sfPivot.y >= CELLS_VERTICAL   ||
	    sfPivot.y <  0                ||
	    !brdGameField[sfPivot.y][sfPivot.x].m_bHidden
	) {
	    if (i < 2) nFrontCorners++;
	    else nBackCorners++;
	}

	sfPivot -= aTestCorners[i];
    }

    if (nFrontCorners == 2 && nBackCorners == 1) {
	return T_SpinTypes::FullSpin;
    } 
    else if (nFrontCorners == 1 && nBackCorners == 2) 
    {
	if (m_nLastRotationTest == 4)
	    return T_SpinTypes::FullSpin;

	return T_SpinTypes::MiniSpin;
    }

    return T_SpinTypes::NoSpin;
}

void Tetrimino::DoAnimation() {
    if (m_ptaAnim->IsPlaying()) {
	auto sfAnimFrameRect = m_ptaAnim->StepAnimation();

	for (auto& sfShape : m_asfPieceViz) {
	    sfShape.setTextureRect(sfAnimFrameRect);
	}
    }
}

void Tetrimino::MoveDown() {
    TranslatePivot({ 0, 1 });

    for (int i = 0; i < 4; i++) {
	m_vCurrentPiece[i].y++;
	m_asfPieceViz[i].setPosition(
	    LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
    }
}

void Tetrimino::ResetPieceAndPivot() {
    m_sfPiecePivot = m_aPiecePivots[static_cast<int>(m_CurrentPieceType)];
    m_vCurrentPiece = m_aPieces[static_cast<int>(m_CurrentPieceType)];
    m_nCurrentOrientation = 0;

    for (int i = 0; i < 4; i++) 
    {
	m_asfPieceViz[i].setPosition(
	    LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
    }
}

Tetrimino::~Tetrimino() {
    delete m_ptaAnim;
}

Tetrimino::Tetrimino(const Tetrimino& ttMino) {
    this->m_asfPieceViz = ttMino.m_asfPieceViz;
    this->m_vCurrentPiece = ttMino.m_vCurrentPiece;
    this->m_sfPiecePivot = ttMino.m_sfPiecePivot;
    this->m_CurrentPieceType = ttMino.m_CurrentPieceType;
    this->m_nCurrentOrientation = ttMino.m_nCurrentOrientation;

    std::vector<unsigned int> vAnim = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    sf::IntRect sfBaseSprite = sf::IntRect(static_cast<int>(this->m_CurrentPieceType) * SquareSize, 0, SquareSize, SquareSize);
    this->m_ptaAnim = new TetriminoAnimation(vAnim, sfBaseSprite);
}

void Tetrimino::SetPivot(sf::Vector2f sfPivotCoords) {
    m_sfPiecePivot = sfPivotCoords;
}