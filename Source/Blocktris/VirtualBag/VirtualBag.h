#ifndef VIRTUALBAG_H
#define VIRTUALBAG_H

#include <list>
#include <memory>
#include <random>
#include <algorithm>

#include "../PieceConstants.h"
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