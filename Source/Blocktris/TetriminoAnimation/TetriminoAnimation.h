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