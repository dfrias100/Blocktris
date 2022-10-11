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
