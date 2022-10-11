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

#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <array>
#include <vector>

#include "../ConstantsEnums.h"
#include "../PileBlock/PileBlock.h"
#include "../Blocktris.h"
#include "../TetriminoAnimation/TetriminoAnimation.h"

#include <SFML/Graphics.hpp>

class Tetrimino {
private:
    // Y X: 0 1 2 3 *4* 5 6 7 8 9
    // 0
    // 1
    // TODO: Make these static
    const std::vector<sf::Vector2i> m_vI_Piece = { {3, -2}, {4, -2}, {5, -2}, {6, -2} };
    const std::vector<sf::Vector2i> m_vJ_Piece = { {3, -2}, {3, -1}, {4, -1}, {5, -1} };
    const std::vector<sf::Vector2i> m_vL_Piece = { {5, -2}, {3, -1}, {4, -1}, {5, -1} };
    const std::vector<sf::Vector2i> m_vO_Piece = { {4, -2}, {5, -2}, {4, -1}, {5, -1} };
    const std::vector<sf::Vector2i> m_vS_Piece = { {4, -2}, {5, -2}, {3, -1}, {4, -1} };
    const std::vector<sf::Vector2i> m_vT_Piece = { {4, -2}, {3, -1}, {4, -1}, {5, -1} };
    const std::vector<sf::Vector2i> m_vZ_Piece = { {3, -2}, {4, -2}, {4, -1}, {5, -1} };

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
    static const std::array<std::array<sf::Vector2i, 4>, 4> m_aT_SpinCorners;

    const std::array<sf::Vector2f, 7> m_aPiecePivots = {
	sf::Vector2f(4.5f, -1.5f),
	sf::Vector2f(4.0f, -1.0f),
	sf::Vector2f(4.0f, -1.0f),
	sf::Vector2f(4.5f, -1.0f), // The O-piece now matters, for drawing a held piece...
	sf::Vector2f(4.0f, -1.0f),
	sf::Vector2f(4.0f, -1.0f),
	sf::Vector2f(4.0f, -1.0f),
    };
    
    std::array<sf::RectangleShape, 4> m_asfPieceViz;
    std::vector<sf::Vector2i> m_vCurrentPiece;
    sf::Vector2f m_sfPiecePivot;
    PieceTypes m_CurrentPieceType;

    int m_nCurrentOrientation = 0;
    int m_nPrevOrientation = 0;
    int m_nLastRotationTest;

    const sf::Vector2i ReturnWallKickVector(int nPrevOrientation, int nNewOrientation, int nTestNo);
    const sf::Vector2i ReturnWallKickVectorIPiece(int nPrevOrientation, int nNewOrientation, int nTestNo);

    static sf::Texture sm_sfBlockTexture;

public:
    Tetrimino(PieceTypes ptType);
    Tetrimino(const Tetrimino& ttMino);
    ~Tetrimino();
    
    void MoveDown();
    void ResetPieceAndPivot();
    void TranslatePivot(sf::Vector2f sfTranslation);
    void SetPivot(sf::Vector2f sfPivotCoords);
    void SetAlphaLevel(int nAlphaLevel);
    void DoAnimation();
    T_SpinTypes CheckTSpin(Board& brdGameField);
    bool TranslateTetriminoHorizontal(bool bLeft, bool bRight, Board& brdGameField);
    bool RotateTetrimino(sf::Vector2f sfRotationCoefficents, Board& brdGameField);
    const std::vector<sf::Vector2i>& GetLogicalCoords();
    std::array<sf::RectangleShape, 4>& GetPieceShapes();
    sf::Vector2f GetPivot();
    PieceTypes GetPieceType();

    TetriminoAnimation* m_ptaAnim;
};

#endif