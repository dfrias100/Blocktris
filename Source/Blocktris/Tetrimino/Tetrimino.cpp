#include "Tetrimino.h"

Tetrimino::Tetrimino() {
    int nPiecePivot = rand() % 7;
    m_sfPiecePivot = m_aPiecePivots[nPiecePivot];
    m_vCurrentPiece = m_aPieces[nPiecePivot];
    m_CurrentPieceType = static_cast<PieceTypes>(nPiecePivot);

    for (auto& sfPiece : m_asfPieceViz) {
	sfPiece.setSize(sf::Vector2f(TrueSquareSize, TrueSquareSize));
	sfPiece.setFillColor(sf::Color::Red);
	sfPiece.setOutlineThickness(SquareOutlineThickness);
	sfPiece.setOutlineColor(sf::Color::Transparent);
    }

    for (int i = 0; i < 4; i++) {
	m_asfPieceViz[i].setPosition(
	    BlockTris::LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
	m_asfPieceViz[i].setFillColor(
	    m_aPieceColors[nPiecePivot]
	);
    }
}

void Tetrimino::ResetPieceAndPivot() {
    int nPiecePivot = rand() % 7;
    m_sfPiecePivot = m_aPiecePivots[nPiecePivot];
    m_vCurrentPiece = m_aPieces[nPiecePivot];
    m_CurrentPieceType = static_cast<PieceTypes>(nPiecePivot);

    for (int i = 0; i < 4; i++) {
	m_asfPieceViz[i].setPosition(
	    BlockTris::LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
	m_asfPieceViz[i].setFillColor(
	    m_aPieceColors[nPiecePivot]
	);
    }
}

void Tetrimino::TranslatePivot(sf::Vector2f sfTranslation) {
    m_sfPiecePivot += sfTranslation;
}

void Tetrimino::RotateTetrimino(sf::Vector2f sfRotationCoefficents, 
    Board& brdGameField) {
    if (m_CurrentPieceType == PieceTypes::O_Piece)
	return;

    bool bCanRotate = true;

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

    for (auto& sfLogicalCoord : vTetriminoCoords) {
	if (
	    sfLogicalCoord.x < 0 ||
	    sfLogicalCoord.x >  9 ||
	    sfLogicalCoord.y < 0 ||
	    sfLogicalCoord.y > 19 ||
	    !brdGameField[sfLogicalCoord.y][sfLogicalCoord.x].m_bHidden
	    ) {
	    bCanRotate = false;
	    break;
	}
    }

    if (bCanRotate) {
	m_vCurrentPiece = vTetriminoCoords;
	for (int i = 0; i < 4; i++) {
	    m_asfPieceViz[i].setPosition(
		BlockTris::LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	    );
	}
    }
}

void Tetrimino::TranslateTetriminoHorizontal(bool bLeft, bool bRight, Board& brdGameField) {
    auto vTetriminoLogicalCoordsTest = m_vCurrentPiece;
    bool bHorizontalCollision = false;

    for (auto& sfCoords : vTetriminoLogicalCoordsTest) {
	// Clamp the new x values to allowable values
	if (bLeft) sfCoords.x--;
	if (bRight) sfCoords.x++;

	if (sfCoords.x < 0 || sfCoords.x > 9 || !brdGameField[sfCoords.y][sfCoords.x].m_bHidden) {
	    bHorizontalCollision = true;
	    break;
	}
    }

    if (!bHorizontalCollision) {
	if (bLeft) TranslatePivot({ -1, 0 });
	if (bRight) TranslatePivot({ 1, 0 });

	m_vCurrentPiece = vTetriminoLogicalCoordsTest;
	for (int i = 0; i < 4; i++) {
	    m_asfPieceViz[i].setPosition(
		BlockTris::LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	    );
	}
    }
}

const std::vector<sf::Vector2i>& Tetrimino::GetLogicalCoords() {
    return m_vCurrentPiece;
}

std::array<sf::RectangleShape, 4>& Tetrimino::GetPieceShapes() {
    return m_asfPieceViz;
}

sf::Color Tetrimino::GetColor() {
    return m_aPieceColors[static_cast<int>(m_CurrentPieceType)];
}

void Tetrimino::MoveDown() {
    TranslatePivot({ 0, 1 });

    for (int i = 0; i < 4; i++) {
	m_vCurrentPiece[i].y++;
	m_asfPieceViz[i].setPosition(
	    BlockTris::LogicalCoordsToScreenCoords(m_vCurrentPiece[i])
	);
    }
}
