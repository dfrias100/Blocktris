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

#include "VirtualBag.h"

std::shared_ptr<Tetrimino> VirtualBag::GetNextPiece() {
    std::shared_ptr<Tetrimino> tmNextPiece = m_lstPieceQueue.front();
    tmNextPiece->ResetPieceAndPivot();
    m_lstPieceQueue.pop_front();
    return tmNextPiece;
}

std::array<std::shared_ptr<Tetrimino>, 3> VirtualBag::PeekNextPieces() {
    if (m_lstPieceQueue.size() == 3)
	ReshuffleAndRefill();

    std::array<std::shared_ptr<Tetrimino>, 3> aTetriminoPieces;

    auto itPieces = m_lstPieceQueue.begin();

    for (int i = 0; i < 3; i++) {
	aTetriminoPieces[i] = *itPieces;
	itPieces++;
    }

    return aTetriminoPieces;
}

void VirtualBag::ReshuffleAndRefill() {
    std::shuffle(
	m_aShuffledPieces.begin(),
	m_aShuffledPieces.end(),
	m_rndGenerator
    );

    for (auto ptType : m_aShuffledPieces) {
	m_lstPieceQueue.push_back(
	    std::make_shared<Tetrimino>(ptType)
	);
    };
}

VirtualBag::VirtualBag() {
    m_rndGenerator = std::default_random_engine(m_rndDev());
    ReshuffleAndRefill();
}
