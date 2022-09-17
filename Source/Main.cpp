#include <iostream>
#include "GameApp/GameApp.h"

class TestApp : public GameApp {
    virtual bool OnConstruct() override {
	std::cout << "Setting up app!" << std::endl;
	return true;
    }

    virtual bool OnUpdate(float fElapsedTime) override {
	return true;
    }
};

int main(int argc, char* argv) {
    TestApp app;

    return app.RunGame();
}