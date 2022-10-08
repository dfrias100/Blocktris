#include "TetriminoAnimation.h"

void TetriminoAnimation::ArmAnimation() {
    m_nCurrIndex = 0;
    m_bAnimating = true;
}

bool TetriminoAnimation::IsPlaying() {
    return m_bAnimating;
}

sf::IntRect TetriminoAnimation::StepAnimation() {
    if (m_nCurrIndex < m_vAnimIndexes.size() && m_bAnimating) {
	auto sfNextSprite = m_sfBaseSprite;
	sfNextSprite.top = m_vAnimIndexes[m_nCurrIndex++] * sfNextSprite.height;
	return sfNextSprite;
    } else {
	m_bAnimating = false;
	return m_sfBaseSprite;
    }
}

TetriminoAnimation::TetriminoAnimation(std::vector<unsigned int> vAnimIndexes, sf::IntRect sfBaseSprite) :
    m_vAnimIndexes(vAnimIndexes), m_sfBaseSprite(sfBaseSprite) {
}
