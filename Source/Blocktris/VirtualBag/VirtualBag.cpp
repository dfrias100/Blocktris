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
