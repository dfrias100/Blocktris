/*----------------------------------------------------------------------------------|
|  Blocktris: A block puzzle game written in C++ using SFML.                        |
|                                                                                   |
|  Copyright(C) 2022  Daniel Frias						    |
|                                                                                   |
|  This program is free software; you can redistribute it and/or		    |
|  modify it under the terms of the GNU General Public License			    |
|  as published by the Free Software Foundation; either version 2		    |
|  of the License, or (at your option) any later version.			    |
|  This program is distributed in the hope that it will be useful,		    |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of		    |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the			    |
|  GNU General Public License for more details.					    |
|  You should have received a copy of the GNU General Public License		    |
|  along with this program; if not, write to the Free Software			    |
|  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  |
|-----------------------------------------------------------------------------------*/

#include "GameApp.h"

void GameApp::ResetReleasedKeys() {
    // Iterate over our hashmap and set released keys
    // to not pressed.
    auto hmIterator = m_hmKeys.begin();

    for (; hmIterator != m_hmKeys.end(); hmIterator++) {
	if (hmIterator->second == KeyStatus::Released) {
	    m_hmKeys[hmIterator->first] = KeyStatus::NotPressed;
	}
    }
}

inline void GameApp::HandleInput() {
    // If we have close event, send a close message,
    // otherwise we record any other keypress
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
    // Clear the current screen buffer
    m_pWindow->clear();
    
    // Draw every object in our list
    for (auto itObjs = m_llstDrawableObjects.begin(); 
	itObjs != m_llstDrawableObjects.end(); itObjs++) {
	m_pWindow->draw(**itObjs);
    }

    // Display the result
    m_pWindow->display();
}

void GameApp::PlaySoundEffects() {
    for (auto itSfxMap = m_hmSoundEffects.begin(); itSfxMap != m_hmSoundEffects.end(); itSfxMap++) {
	auto& prEntry = itSfxMap->second;
	if (prEntry.second) {
	    prEntry.first->play();
	    prEntry.second = false;
	}
    }
}

void GameApp::MarkSoundForPlay(int nSfxNo) {
    m_hmSoundEffects[nSfxNo].second = true;
}

void GameApp::RegisterSoundEffect(int nSfxNo, sf::SoundBuffer* psfSoundBuffer) {
    m_hmSoundEffects[nSfxNo] = std::make_pair<std::shared_ptr<sf::Sound>, bool>(
	    std::make_shared<sf::Sound>(*psfSoundBuffer),
	    false
	);
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
    /*-----------------------------------------------------------------|
    |	After the base and derived class are constructed, we will      |
    |   want to initialize the derived class, so that it can get       |
    |   ready to run its implementation of their game. If for	       |
    |   whatever reason it cannot start, the engine immediately quits  |
    |   in an error state.					       |
    |-----------------------------------------------------------------*/
    if (!OnInitialize()) 
	return -1;

    float fFrameTime;

    // Main game loop
    while (m_pWindow->isOpen()) {
	/*---------------------------------------------------------------------|
	|   Measure how much time the previous took in totaland calculate      |
	|   as seconds. SFML also has a seconds function, but I would like to  |
	|   store them as 64-bit integers.                                     |
	----------------------------------------------------------------------*/
	m_ullTimePoint2 = m_AppClock.getElapsedTime().asMicroseconds();
	fFrameTime = (m_ullTimePoint2 - m_ullTimePoint1) / 1e6f;
	m_ullTimePoint1 = m_ullTimePoint2;

	// Handle our input and store the state of the keyboard
	HandleInput();
	
	// Time to let the derived class handle its logic
	if (!OnUpdate(fFrameTime)) 
	    return -1;

	PlaySoundEffects();

	// If we had any 'released' keys clear them here, OnUpdate has already been
	// made aware of any released keys this frame
	ResetReleasedKeys();

	// Draw all the objects in our linked list
	RefreshAndDisplay();

	// Clear all the pointers in our linked list for the next frame
	m_llstDrawableObjects.clear();

	// If we have FPS control enabled, handle it here
	if (m_ctrlMode != FPSControl::NONE) {
	    LockFrameRate();
	}
    }

    return 0;
}

/*---------------------------------------------------------------------|
|   This uses a spin-lock, there are better ways to control FPS, but   |
|   this does an acceptable job for small projects like this and has   |
|   good precision.                                                    |
----------------------------------------------------------------------*/ 
void GameApp::LockFrameRate() {
    unsigned long long ullEndTime = m_ullTimePoint2 + m_fFrametime;
    while (m_AppClock.getElapsedTime().asMicroseconds() < ullEndTime) {}
}