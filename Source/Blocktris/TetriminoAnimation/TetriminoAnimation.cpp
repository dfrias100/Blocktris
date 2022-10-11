#include "TetriminoAnimation.h"

void TetriminoAnimation::ArmAnimation() {
    m_unCurrIndex = 0;
    m_unFrames = 0;
    m_bAnimating = true;
}

void TetriminoAnimation::SetFrameDelay(unsigned int unDelay) {
    m_unFrameDelay = unDelay;
}

void TetriminoAnimation::SetNewBaseSprite(sf::IntRect sfBaseSprite) {
    m_sfBaseSprite = sfBaseSprite;
}

bool TetriminoAnimation::IsPlaying() {
    return m_bAnimating;
}

sf::IntRect TetriminoAnimation::StepAnimation() {
    if (m_unCurrIndex < m_vAnimIndexes.size() && m_bAnimating) {
	if (m_unFrames++ % m_unFrameDelay == 0) { 
	    auto sfNextSprite = m_sfBaseSprite;
	    sfNextSprite.top = m_vAnimIndexes[m_unCurrIndex++] * sfNextSprite.height;
	    return sfNextSprite;
	} else {
	    auto sfNextSprite = m_sfBaseSprite;
	    sfNextSprite.top = m_vAnimIndexes[m_unCurrIndex] * sfNextSprite.height;
	    return sfNextSprite;
	}
    } else {
	m_bAnimating = false;
	return m_sfBaseSprite;
    }
}

TetriminoAnimation::TetriminoAnimation(std::vector<unsigned int> vAnimIndexes, sf::IntRect sfBaseSprite) :
    m_vAnimIndexes(vAnimIndexes), m_sfBaseSprite(sfBaseSprite) {
}
