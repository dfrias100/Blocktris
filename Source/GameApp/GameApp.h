/*----------------------------------------------------------------------------------|
|  Blocktris: Tetris-like clone written in C++ using SFML. 			    |
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

#ifndef GAMEAPP_H
#define GAMEAPP_H

/////////////////////////////////
// STL and C/C++ lib includes  //
/////////////////////////////////
#include <string>
#include <unordered_map>
#include <list>

/////////////////////////////////
//    3rd-party lib includes   //
/////////////////////////////////
#include <SFML/Graphics.hpp>

/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
  Class:    GameApp

  Summary:  Abstract class that serves as a simple game engine for
            simple games. All games using this engine must be derived
	    classes and implement OnInitialize and OnUpdate. Uses
	    SFML to facilitate windowing and drawing.
 C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GameApp {
public:
    // Enums for common FPS locks
    enum class FPSControl {
	NONE = 0,
	Locked30 = 30,
	Locked60 = 60
    };

protected:
    // Provides a way to easily store keyboard presses
    enum class KeyStatus {
	NotPressed = 0,
	Pressed = 1,
	Released = 2
    };

private:
    // SFML specific objects to facilitate visuals, input, and timing
    sf::RenderWindow* m_pWindow = nullptr;
    sf::Event m_WindowEvent = sf::Event();
    sf::VideoMode m_WindowSize = sf::VideoMode(800, 600);
    sf::Clock m_AppClock;

    // Meta-data data structures
    std::string m_szWindowTitle;
    std::unordered_map<sf::Keyboard::Key, KeyStatus> m_hmKeys;
    std::list<sf::Drawable*> m_llstDrawableObjects;

    FPSControl m_ctrlMode;

    // Subroutines that handle different aspects of the engine
    void ResetReleasedKeys();
    void HandleInput();
    void RefreshAndDisplay();

    void LockFrameRate();

    // For managing time
    unsigned long long m_ullTimePoint1 = 0;
    unsigned long long m_ullTimePoint2 = 0;

    // This will hold the target frametime as required by the FPS control mode
    float m_fFrametime = 0.0f;

protected:
    // As stated earlier, derived class must implement these
    virtual bool OnInitialize() = 0;
    virtual bool OnUpdate(float fFrameTime) = 0;

    // Helpful utliliy functions that the derived class may 
    // want to use
    sf::VideoMode GetWindowSize();
    KeyStatus GetKeyStatus(sf::Keyboard::Key sfTestedKey);

    // This function is what the derived class will use when
    // it wants the engine to draw something
    void PushDrawableObject(sf::Drawable* sfDrawableObj);

public:
    // Constructors, default title and FPS control is specified.
    GameApp(std::string szWindowTitle = "Default Title", 
	FPSControl ctrlFpsControl = FPSControl::NONE);

    GameApp(sf::VideoMode sfWindowSize, 
	std::string szWindowTitle = "Default Title",
	FPSControl ctrlFpsControl = FPSControl::NONE);

    ~GameApp();

    // Main function should call this to start the game
    int RunGame();
};

#endif
