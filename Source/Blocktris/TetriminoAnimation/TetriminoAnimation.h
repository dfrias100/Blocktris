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

#ifndef TETRIMINOANIMATION_H
#define TETRIMINOANIMATION_H

#include <vector>

#include <SFML/Graphics.hpp>

class TetriminoAnimation {
private:
    std::vector<unsigned int> m_vAnimIndexes;
    unsigned int m_unCurrIndex = 0;
    unsigned int m_unFrameDelay = 1;
    unsigned int m_unFrames = 0;
    bool m_bAnimating = false;
    sf::IntRect m_sfBaseSprite;
public:
    void ArmAnimation();
    void SetFrameDelay(unsigned int unDelay);
    void SetNewBaseSprite(sf::IntRect sfBaseSprite);
    bool IsPlaying();
    sf::IntRect StepAnimation();
    TetriminoAnimation(std::vector<unsigned int> vAnimIndexes, sf::IntRect sfBaseSprite);
};

#endif