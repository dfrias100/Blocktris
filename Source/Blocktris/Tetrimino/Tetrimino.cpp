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

sf::Texture Tetrimino::sm_sfBlockTexture;

const std::array<std::array<sf::Vector2i, 4>, 4> Tetrimino::m_aWallKickDataIPiece = {
    {
	{ sf::Vector2i(-2, 0), sf::Vector2i( 1, 0), sf::Vector2i(-2,  1), sf::Vector2i( 1, -2) }, // 0 >> 1
	{ sf::Vector2i( 2, 0), sf::Vector2i(-1, 0), sf::Vector2i( 2, -1), sf::Vector2i(-1,  2) }, // 2 >> 3
	{ sf::Vector2i(-1, 0), sf::Vector2i( 2, 0), sf::Vector2i(-1, -2), sf::Vector2i( 2,  1) }, // 0 >> 3
	{ sf::Vector2i( 1, 0), sf::Vector2i(-2, 0), sf::Vector2i( 1,  2), sf::Vector2i(-2, -1) }  // 2 >> 1
    }
};

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
    int nPossibleRotation = (4 + ((m_nCurrentOrientation + nRotationIncrement) % 4)) % 4;
    sf::Vector2i sfTranslationVector = sf::Vector2i(0, 0);

    for (int i = 0; i < 5; i++) {
	if (i != 0) {
	    if (m_CurrentPieceType != PieceTypes::I_Piece)
		sfTranslationVector = ReturnWallKickVector(m_nCurrentOrientation, nPossibleRotation, i - 1);
	    else
		sfTranslationVector = ReturnWallKickVectorIPiece(m_nCurrentOrientation, nPossibleRotation, i - 1);
	}

	bCanRotate = true;

	for (auto& sfLogicalCoord : vTetriminoCoords) {
	    sfLogicalCoord += sfTranslationVector;
	    if (
		sfLogicalCoord.x <  0 ||
		sfLogicalCoord.x >  9 ||
		sfLogicalCoord.y > 19 ||
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

	    TranslatePivot(sf::Vector2f(sfTranslationVector));
	    m_vCurrentPiece = vTetriminoCoords;
	    for (int i = 0; i < 4; i++) {
		m_asfPieceViz[i].setPosition(
		    LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
		);
	    }
	    return true;
	} else {
	    for (auto& sfLogicalCoord : vTetriminoCoords) {
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

	if (sfCoords.x < 0 || sfCoords.x > 9 || (sfCoords.y >= 0 && !brdGameField[sfCoords.y][sfCoords.x].m_bHidden)) {
	    bHorizontalCollision = true;
	    return false;
	}
    }

    if (!bHorizontalCollision) {
	if (bLeft) TranslatePivot({ -1, 0 });
	if (bRight) TranslatePivot({ 1, 0 });

	m_vCurrentPiece = vTetriminoLogicalCoordsTest;
	for (int i = 0; i < 4; i++) {
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

sf::Vector2f Tetrimino::GetPivot() {
    return m_sfPiecePivot;
}

sf::Color Tetrimino::GetColor() {
    return m_aPieceColors[static_cast<int>(m_CurrentPieceType)];
}

PieceTypes Tetrimino::GetPieceType() {
    return m_CurrentPieceType;
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

    for (int i = 0; i < 4; i++) {
	m_asfPieceViz[i].setPosition(
	    LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
    }
}

void Tetrimino::SetPivot(sf::Vector2f sfPivotCoords) {
    m_sfPiecePivot = sfPivotCoords;
}