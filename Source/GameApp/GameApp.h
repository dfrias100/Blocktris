#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

class GameApp {
protected:
    enum class KeyStatus {
	NotPressed = 0,
	Pressed = 1,
	Released = 2
    };
private:
    sf::RenderWindow* m_pWindow = nullptr;
    sf::Event m_WindowEvent = sf::Event();
    sf::VideoMode m_WindowSize = sf::VideoMode(800, 600);
    sf::Clock m_AppClock;

    std::string m_szWindowTitle;
    std::unordered_map<sf::Keyboard::Key, KeyStatus> m_hmKeys;

    bool m_bCanRun = false;

    void ClearReleased();

protected:
    virtual bool OnConstruct() = 0;
    virtual bool OnUpdate(float fElapsedTime) = 0;

public:
    GameApp(std::string szWindowTitle = "Default Title");
    GameApp(sf::VideoMode sfWindowSize, std::string szWindowTitle = "Default Title");
    ~GameApp();

    sf::VideoMode GetWindowSize();
    KeyStatus GetKeyStatus(sf::Keyboard::Key sfTestedKey);

    int RunGame();
};

#endif