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

#include "PileBlock.h"

sf::Texture PileBlock::sm_sfBlockTexture;

sf::Vector2f LogicalCoordsToScreenCoords(int xLogicalCoord, int yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f LogicalCoordsToScreenCoords(float xLogicalCoord, float yLogicalCoord) {
    float xfScreenCoords = BoardOffsetX + xLogicalCoord * SquareSize;

    float yfScreenCoords = BoardOffsetY + yLogicalCoord * SquareSize;

    return sf::Vector2f(xfScreenCoords, yfScreenCoords);
}

sf::Vector2f LogicalCoordsToScreenCoords(sf::Vector2i& sfLogicalCoords) {
    return LogicalCoordsToScreenCoords(sfLogicalCoords.x, sfLogicalCoords.y);
}

PileBlock::PileBlock() {
    std::vector<unsigned int> vAnim = { 0, 5, 0, 5, 0, 5, 0, 5, 0 };
    sf::IntRect sfBaseSprite = sf::IntRect(0, 0, SquareSize, SquareSize);
    m_ptaAnim = new TetriminoAnimation(vAnim, sfBaseSprite);
    m_ptaAnim->SetFrameDelay(9);
}
