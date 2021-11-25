#include "dialog.h"
#include "SFML/Graphics.hpp"

void foo(sf::RenderWindow &window, sf::Sprite &sprite){
	window.draw(sprite);
}

int main(){
	Dialog::menu();

//	sf::RenderWindow window(sf::VideoMode(1000, 1000), "test");
//	window.setFramerateLimit(60);
//
//	sf::Texture texture;
//	texture.loadFromFile("Textures/GlassTexture.png");
//	sf::Sprite sprite;
//	sprite.setTexture(texture);
//
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed)
//				window.close();
//			if (event.type == sf::Event::KeyPressed) {
//				switch (event.key.code) {
//					case sf::Keyboard::Escape:
//						window.close();
//						break;
//					default :
//						//display "wrong button"
//						break;
//				}
//			}
//		}
//
//		window.clear();
//		foo(window, sprite);
//		window.display();
//	}
////	sf::Texture t(texture);
	
	return 0;
}