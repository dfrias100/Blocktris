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

#ifndef VIRTUALBAG_H
#define VIRTUALBAG_H

#include <list>
#include <memory>
#include <random>
#include <algorithm>

#include "../ConstantsEnums.h"
#include "../Tetrimino/Tetrimino.h"

class Tetrimino;

class VirtualBag {
private:
    std::array<PieceTypes, 7> m_aShuffledPieces = {
	PieceTypes::I_Piece,
	PieceTypes::J_Piece,
	PieceTypes::L_Piece,
	PieceTypes::O_Piece,
	PieceTypes::S_Piece,
	PieceTypes::T_Piece,
	PieceTypes::Z_Piece
    };

    std::list<std::shared_ptr<Tetrimino>> m_lstPieceQueue;
    std::random_device m_rndDev;
    std::default_random_engine m_rndGenerator;
public:
    std::shared_ptr<Tetrimino> GetNextPiece();
    std::array<std::shared_ptr<Tetrimino>, 3> PeekNextPieces();
    void ReshuffleAndRefill();
    VirtualBag();
};

#endif