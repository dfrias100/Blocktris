#include "Tetrimino.h"

Tetrimino::Tetrimino() {
    m_vCurrentPiece = m_vT_Piece;
    m_sfPiecePivot = m_aPiecePivots[static_cast<int>(PieceTypes::T_Piece)];
    m_CurrentPieceType = PieceTypes::T_Piece;

    for (auto& sfPiece : m_asfPieceViz) {
	sfPiece.setSize(sf::Vector2f(TrueSquareSize, TrueSquareSize));
	sfPiece.setFillColor(sf::Color::Red);
	sfPiece.setOutlineThickness(SquareOutlineThickness);
	sfPiece.setOutlineColor(sf::Color::Transparent);
    }
}

void Tetrimino::ResetPieceAndPivot() {
    int nPiecePivot = rand() % 7;
    m_sfPiecePivot = m_aPiecePivots[nPiecePivot];
    m_vCurrentPiece = m_aPieces[nPiecePivot];
}

void Tetrimino::TranslatePivot(sf::Vector2f sfTranslation) {
    m_sfPiecePivot += sfTranslation;
}

sf::Vector2f& Tetrimino::GetPivot() {
    return m_sfPiecePivot;
}

std::pair<std::vector<sf::Vector2i>&,
    std::array<sf::RectangleShape, 4>&> Tetrimino::GetPieceData() {

    return std::pair<std::vector<sf::Vector2i>&, 
	std::array<sf::RectangleShape, 4>&>(std::ref(m_vCurrentPiece), std::ref(m_asfPieceViz));
}
