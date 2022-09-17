#include <iostream>
#include "GameApp/GameApp.h"

class TestApp : public GameApp {
    virtual bool OnConstruct() override {
	std::cout << "Setting up app!" << std::endl;
	return true;
    }

    virtual bool OnUpdate(float fElapsedTime) override {
	if (GetKeyStatus(sf::Keyboard::A) == KeyStatus::Pressed) {
	    std::cout << "Key A pressed at time " << fElapsedTime << "." << std::endl;
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