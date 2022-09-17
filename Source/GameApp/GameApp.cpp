#include "GameApp.h"

void GameApp::ClearReleased() {
    auto hmIterator = m_hmKeys.begin();

    for (; hmIterator != m_hmKeys.end(); hmIterator++) {
	if (hmIterator->second == KeyStatus::Released) {
	    m_hmKeys[hmIterator->first] = KeyStatus::NotPressed;
	}
    }
}

GameApp::GameApp(std::string szWindowTitle)
    : m_szWindowTitle(szWindowTitle) {
    m_pWindow = new sf::RenderWindow(m_WindowSize, m_szWindowTitle);
}

GameApp::GameApp(sf::VideoMode sfWindowSize, std::string szWindowTitle) 
    : m_WindowSize(sfWindowSize), m_szWindowTitle(szWindowTitle) {
    m_pWindow = new sf::RenderWindow(m_WindowSize, m_szWindowTitle);
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

int GameApp::RunGame() {
    m_bCanRun = OnConstruct();
    if (!m_bCanRun) return -1;

    sf::Event sfEventCopy;

    while (m_pWindow->isOpen()) {
	// Handle possible main window events
	while (m_pWindow->pollEvent(m_WindowEvent)) {
	    if (m_WindowEvent.type == sf::Event::Closed) {
		m_pWindow->close();
	    } else if (m_WindowEvent.type == sf::Event::KeyPressed) {
		m_hmKeys[m_WindowEvent.key.code] = KeyStatus::Pressed;
	    } else if (m_WindowEvent.type == sf::Event::KeyReleased) {
		m_hmKeys[m_WindowEvent.key.code] = KeyStatus::Released;
	    }
	}
	if (!OnUpdate(m_AppClock.getElapsedTime().asSeconds())) return -1;

	ClearReleased();

	m_pWindow->clear();
	m_pWindow->display();
    }

    return 0;
}