#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <string>
#include <unordered_map>
#include <list>
#include <SFML/Graphics.hpp>

class GameApp {
public:
    enum class FPSControl {
	NONE = 0,
	Locked30 = 30,
	Locked60 = 60
    };

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
    std::list<sf::Drawable*> m_llstDrawableObjects;

    FPSControl m_ctrlMode;

    void ResetReleasedKeys();
    void HandleInput();
    void RefreshAndDisplay();

    void LockFrameRate();

    unsigned long long m_ullTimePoint1 = 0;
    unsigned long long m_ullTimePoint2 = 0;

    float m_fFrametime = 0.0f;

protected:
    virtual bool OnInitialize() = 0;
    virtual bool OnUpdate(float fFrameTime) = 0;

    sf::VideoMode GetWindowSize();
    KeyStatus GetKeyStatus(sf::Keyboard::Key sfTestedKey);
    void PushDrawableObject(sf::Drawable* sfDrawableObj);

public:
    GameApp(std::string szWindowTitle = "Default Title", 
	FPSControl ctrlFpsControl = FPSControl::NONE);

    GameApp(sf::VideoMode sfWindowSize, 
	std::string szWindowTitle = "Default Title",
	FPSControl ctrlFpsControl = FPSControl::NONE);

    ~GameApp();

    int RunGame();
};

#endif
