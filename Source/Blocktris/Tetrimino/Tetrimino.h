#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <array>
#include <vector>

#include "../PieceConstants.h"
#include "../PileBlock.h"
#include "../Blocktris.h"

#include <SFML/Graphics.hpp>

class Tetrimino {
private:
    // Y X: 0 1 2 3 *4* 5 6 7 8 9
    // 0
    // 1
    const std::vector<sf::Vector2i> m_vI_Piece = { {3, 0}, {4, 0}, {5, 0}, {6, 0} };
    const std::vector<sf::Vector2i> m_vJ_Piece = { {3, 0}, {3, 1}, {4, 1}, {5, 1} };
    const std::vector<sf::Vector2i> m_vL_Piece = { {5, 0}, {3, 1}, {4, 1}, {5, 1} };
    const std::vector<sf::Vector2i> m_vO_Piece = { {4, 0}, {5, 0}, {4, 1}, {5, 1} };
    const std::vector<sf::Vector2i> m_vS_Piece = { {4, 0}, {5, 0}, {3, 1}, {4, 1} };
    const std::vector<sf::Vector2i> m_vT_Piece = { {4, 0}, {3, 1}, {4, 1}, {5, 1} };
    const std::vector<sf::Vector2i> m_vZ_Piece = { {3, 0}, {4, 0}, {4, 1}, {5, 1} };

    const std::array<std::vector<sf::Vector2i>, 7> m_aPieces = {
	m_vI_Piece,
	m_vJ_Piece,
	m_vL_Piece,
	m_vO_Piece,
	m_vS_Piece,
	m_vT_Piece,
	m_vZ_Piece
    };

    static const std::array<std::array<sf::Vector2i, 4>, 4> m_aWallKickData;
    static const std::array<std::array<sf::Vector2i, 4>, 4> m_aWallKickDataIPiece;

    const std::array<sf::Color, 7> m_aPieceColors = {
	sf::Color(0x59C9E7FF), // cyan
	sf::Color(0x1F55EAFF), // light blue
	sf::Color(0xFF9A3CFF), // orange
	sf::Color(0xE0C230FF), // gold
	sf::Color(0x329F4BFF), // green
	sf::Color(0xB152CBFF), // purple
	sf::Color(0xF33232FF)  // red
    };

    const std::array<sf::Vector2f, 7> m_aPiecePivots = {
	sf::Vector2f(4.5f, 0.5f),
	sf::Vector2f(4.0f, 1.0f),
	sf::Vector2f(4.0f, 1.0f),
	sf::Vector2f(0.0f, 0.0f), // The O-piece pivot doesn't matter anyway
	sf::Vector2f(4.0f, 1.0f),
	sf::Vector2f(4.0f, 1.0f),
	sf::Vector2f(4.0f, 1.0f),
    };
    
    std::array<sf::RectangleShape, 4> m_asfPieceViz;
    std::vector<sf::Vector2i> m_vCurrentPiece;
    sf::Vector2f m_sfPiecePivot;
    PieceTypes m_CurrentPieceType;

    int m_nCurrentOrientation = 0;
    int m_nPrevOrientation = 0;

    const sf::Vector2i ReturnWallKickVector(int nPrevOrientation, int nNewOrientation, int nTestNo);
    const sf::Vector2i ReturnWallKickVectorIPiece(int nPrevOrientation, int nNewOrientation, int nTestNo);

public:
    Tetrimino(PieceTypes ptType);
    
    void MoveDown();
    void TranslatePivot(sf::Vector2f sfTranslation);
    void RotateTetrimino(sf::Vector2f sfRotationCoefficents, Board& brdGameField);
    void TranslateTetriminoHorizontal(bool bLeft, bool bRight, Board& brdGameField);
    const std::vector<sf::Vector2i>& GetLogicalCoords();
    std::array<sf::RectangleShape, 4>& GetPieceShapes();
    sf::Color GetColor();
};

#endif