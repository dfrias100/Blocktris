#include "GameApp.h"

void GameApp::ResetReleasedKeys() {
    auto hmIterator = m_hmKeys.begin();

    for (; hmIterator != m_hmKeys.end(); hmIterator++) {
	if (hmIterator->second == KeyStatus::Released) {
	    m_hmKeys[hmIterator->first] = KeyStatus::NotPressed;
	}
    }
}

inline void GameApp::HandleInput() {
    while (m_pWindow->pollEvent(m_WindowEvent)) {
	switch (m_WindowEvent.type) {
	case sf::Event::Closed:
	    m_pWindow->close();
	    break;
	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
	    m_hmKeys[m_WindowEvent.key.code] = 
		m_WindowEvent.type == sf::Event::KeyPressed ? 
		KeyStatus::Pressed : 
		KeyStatus::Released;
	    break;
	}
    }
}

void GameApp::RefreshAndDisplay() {
    m_pWindow->clear();
    
    for (auto itObjs = m_llDrawableObjects.begin(); 
	itObjs != m_llDrawableObjects.end(); itObjs++) {
	m_pWindow->draw(**itObjs);
    }

    m_pWindow->display();
}

GameApp::GameApp(std::string szWindowTitle)
    : m_szWindowTitle(szWindowTitle) {
    m_pWindow = new sf::RenderWindow(m_WindowSize, m_szWindowTitle);
}

GameApp::GameApp(sf::VideoMode sfWindowSize, std::string szWindowTitle) 
    : m_WindowSize(sfWindowSize), m_szWindowTitle(szWindowTitle) {
    m_pWindow = new sf::RenderWindow(m_WindowSize, m_szWindowTitle, 
	sf::Style::Titlebar | sf::Style::Close);
}

GameApp::~GameApp() {
    delete m_pWindow;
}

sf::VideoMode GameApp::GetWindowSize() {
    return m_WindowSize;
}

GameApp::KeyStatus GameApp::GetKeyStatus(sf::Keyboard::Key sfTestedKey) {
    return m_hmKeys[sfTestedKey];
}

void GameApp::PushDrawableObject(sf::Drawable* sfDrawableObj) {
    m_llDrawableObjects.push_back(sfDrawableObj);
}

int GameApp::RunGame() {
    if (!OnInitialize()) 
	return -1;

    float fFrameTime;

    while (m_pWindow->isOpen()) {
	fTimePoint2 = m_AppClock.getElapsedTime().asSeconds();
	fFrameTime = fTimePoint2 - fTimePoint1;
	fTimePoint1 = fTimePoint2;

	HandleInput();
	
	if (!OnUpdate(fFrameTime)) 
	    return -1;

	ResetReleasedKeys();
	RefreshAndDisplay();

	m_llDrawableObjects.clear();
    }

    return 0;
}