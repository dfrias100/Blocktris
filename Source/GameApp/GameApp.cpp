#include "GameApp.h"

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

int GameApp::RunGame() {
    m_bCanRun = OnConstruct();
    if (!m_bCanRun) return -1;

    sf::Event sfEventCopy;

    while (m_pWindow->isOpen()) {
	// Handle possible main window events
	while (m_pWindow->pollEvent(m_WindowEvent)) {
	    if (m_WindowEvent.type == sf::Event::Closed) {
		m_pWindow->close();
	    } 
	}

	if (!OnUpdate(m_fElapsedTime)) return -1;

	m_pWindow->clear();
	m_pWindow->display();
    }

    return 0;
}