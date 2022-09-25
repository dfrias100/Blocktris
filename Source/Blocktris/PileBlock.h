#ifndef PILEBLOCK_H
#define PILEBLOCK_H

#include <SFML/Graphics.hpp>

struct PileBlock {
    bool m_bHidden = true;
    sf::RectangleShape m_sfBlockViz;
};

#endif