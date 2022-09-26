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
    
    for (auto itObjs = m_llstDrawableObjects.begin(); 
	itObjs != m_llstDrawableObjects.end(); itObjs++) {
	m_pWindow->draw(**itObjs);
    }

    m_pWindow->display();
}

GameApp::GameApp(std::string szWindowTitle,
    FPSControl ctrlFpsControl)
    : m_szWindowTitle(szWindowTitle), m_ctrlMode(ctrlFpsControl) {
    m_pWindow = new sf::RenderWindow(m_WindowSize, m_szWindowTitle,
	sf::Style::Titlebar | sf::Style::Close);
    m_fFrametime = 1e6f / int(m_ctrlMode);
}

GameApp::GameApp(sf::VideoMode sfWindowSize, std::string szWindowTitle, 
    FPSControl ctrlFpsControl)
    : m_WindowSize(sfWindowSize), m_szWindowTitle(szWindowTitle),
    m_ctrlMode(ctrlFpsControl) {
    m_pWindow = new sf::RenderWindow(m_WindowSize, m_szWindowTitle, 
	sf::Style::Titlebar | sf::Style::Close);
    m_fFrametime = 1e6f / int(m_ctrlMode);
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
    m_llstDrawableObjects.push_back(sfDrawableObj);
}

int GameApp::RunGame() {
    if (!OnInitialize()) 
	return -1;

    float fFrameTime;

    while (m_pWindow->isOpen()) {
	m_ullTimePoint2 = m_AppClock.getElapsedTime().asMicroseconds();
	fFrameTime = (m_ullTimePoint2 - m_ullTimePoint1) / 1e6f;
	m_ullTimePoint1 = m_ullTimePoint2;

	HandleInput();
	
	if (!OnUpdate(fFrameTime)) 
	    return -1;

	ResetReleasedKeys();
	RefreshAndDisplay();

	m_llstDrawableObjects.clear();

	if (m_ctrlMode != FPSControl::NONE) {
	    LockFrameRate();
	}
    }

    return 0;
}

void GameApp::LockFrameRate() {
    unsigned long long ullEndTime = m_ullTimePoint2 + m_fFrametime;
    while (m_AppClock.getElapsedTime().asMicroseconds() < ullEndTime) {}
}