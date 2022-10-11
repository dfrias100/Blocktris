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

#ifndef PILEBLOCK_H
#define PILEBLOCK_H

/////////////////////////////////
// STL and C/C++ lib includes  //
/////////////////////////////////
#include <array>

/////////////////////////////////
//    3rd-party lib includes   //
/////////////////////////////////
#include <SFML/Graphics.hpp>

/////////////////////////////////
//    Local folder includes    //
/////////////////////////////////
#include "../ConstantsEnums.h"
#include "../TetriminoAnimation/TetriminoAnimation.h"

struct PileBlock {
    bool m_bHidden = true;
    sf::RectangleShape m_sfBlockViz;
    static sf::Texture sm_sfBlockTexture;
    TetriminoAnimation* m_ptaAnim;
    PileBlock();
};

/////////////////////////////////
//          Typedefs           //
/////////////////////////////////
typedef std::array<std::array<PileBlock, CELLS_HORIZONTAL>, CELLS_VERTICAL> Board;

/////////////////////////////////
//     Function Definitons     //
/////////////////////////////////

// Translates logical array coordinates to screen coordinates -- used for blocks only
sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoord, int yLogicalCoord);
sf::Vector2f LogicalCoordsToScreenCoords(float xLogicalCoord, float yLogicalCoord);
sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords);

#endif