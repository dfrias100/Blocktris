#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <string>
#include <SFML/Graphics.hpp>

class GameApp {
private:
    sf::RenderWindow* m_pWindow = nullptr;
    sf::Event m_WindowEvent = sf::Event();
    sf::VideoMode m_WindowSize = sf::VideoMode(800, 600);

    std::string m_szWindowTitle;

    float m_fElapsedTime = 0.0f;
    bool m_bCanRun = false;

protected:
    virtual bool OnConstruct() = 0;
    virtual bool OnUpdate(float fElapsedTime) = 0;

public:
    GameApp(std::string szWindowTitle = "Default Title");
    GameApp(sf::VideoMode sfWindowSize, std::string szWindowTitle = "Default Title");
    ~GameApp();

    sf::VideoMode GetWindowSize();

    int RunGame();
};

#endif