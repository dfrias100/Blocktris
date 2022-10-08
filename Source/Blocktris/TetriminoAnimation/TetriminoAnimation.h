#ifndef TETRIMINOANIMATION_H
#define TETRIMINOANIMATION_H

#include <vector>

#include <SFML/Graphics.hpp>

class TetriminoAnimation {
private:
    std::vector<unsigned int> m_vAnimIndexes;
    unsigned int m_nCurrIndex = 0;
    bool m_bAnimating = false;
    sf::IntRect m_sfBaseSprite;
public:
    void ArmAnimation();
    bool IsPlaying();
    sf::IntRect StepAnimation();
    TetriminoAnimation(std::vector<unsigned int> vAnimIndexes, sf::IntRect sfBaseSprite);
};

#endif