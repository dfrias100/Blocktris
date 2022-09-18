#include <iostream>
#include "GameApp/GameApp.h"

class TestApp : public GameApp {
    sf::RectangleShape* m_pTestSprite;
    bool m_bA_Held = false;

    virtual bool OnInitialize() override {
	std::cout << "Setting up app!" << std::endl;
	m_pTestSprite = new sf::RectangleShape(sf::Vector2f(120.0f, 50.0f));
	m_pTestSprite->setPosition(20, 50);
	return true;
    }

    virtual bool OnUpdate(float fElapsedTime) override {
	if (GetKeyStatus(sf::Keyboard::A) == KeyStatus::Pressed) {
	    std::cout << "Key A pressed at time " << fElapsedTime << "." << std::endl;
	    PushDrawableObject(m_pTestSprite);
	} else if (GetKeyStatus(sf::Keyboard::A) == KeyStatus::Released) {
	    std::cout << "Key A released at time " << fElapsedTime << "." << std::endl;
	}

	return true;
    }
};

int main(int argc, char* argv) {
    TestApp app;
    return app.RunGame();
}