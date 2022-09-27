#include "Tetrimino.h"

Tetrimino::Tetrimino() {
    m_vCurrentPiece = m_vT_Piece;

    for (auto& sfPiece : m_asfPieceViz) {
	sfPiece.setSize(sf::Vector2f(TrueSquareSize, TrueSquareSize));
	sfPiece.setFillColor(sf::Color::Red);
	sfPiece.setOutlineThickness(SquareOutlineThickness);
	sfPiece.setOutlineColor(sf::Color::Transparent);
    }
}

void Tetrimino::ResetPieceAndPivot() {
    m_sfPiecePivot = m_aPiecePivots[rand() % 7];
    m_vCurrentPiece = m_aPieces[rand() % 7];
}

std::pair<std::vector<sf::Vector2i>&,
    std::array<sf::RectangleShape, 4>&> Tetrimino::GetPieceData() {

    return std::pair<std::vector<sf::Vector2i>&, 
	std::array<sf::RectangleShape, 4>&>(std::ref(m_vCurrentPiece), std::ref(m_asfPieceViz));
}
