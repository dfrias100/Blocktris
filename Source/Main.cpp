#include <iostream>
#include <SFML/Graphics.hpp>

int main(int argc, char* argv) {
    sf::RenderWindow sfWin(sf::VideoMode(800, 600), "Test window");
    
    while (sfWin.isOpen()) {
	sf::Event sfEvt;
	while (sfWin.pollEvent(sfEvt)) {
	    if (sfEvt.type == sf::Event::Closed)
		sfWin.close();
	}

	sfWin.clear();
	sfWin.display();
    }

    return 0;
}