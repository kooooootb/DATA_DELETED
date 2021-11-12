//#include <SFML/Graphics.hpp>

#include <iostream>
#include "Level.h"

void func(int *a){
	a = new int;
	*a = 100;
}

int main()
{
//	// Создаем окно размером 600 на 600 и частотой обновления 60 кадров в секунду
//	int res_X = 1920, res_Y = 1080;
//	float a = 200, b = 200;
//	sf::RenderWindow window(sf::VideoMode(res_X, res_Y), "asd");
//	window.setFramerateLimit(60);
//
//	sf::RectangleShape shape(sf::Vector2f(a,b));
//	sf::RectangleShape shape1(sf::Vector2f(a,b));
//	shape.setFillColor(sf::Color::Red);
//	shape1.setFillColor(sf::Color::Green);
//
//	sf::Texture pict;
//	pict.loadFromFile("C:/Users/zavgm/Desktop/ehoJau95KJc.jpg");
////	pict.loadFromFile("C:/Users/zavgm/Desktop/asd.png");
//	sf::Sprite s(pict);
//
//	sf::Event event;
//	sf::Clock clock;
//
////	s.setScale(20,20);
//
//	const sf::Vector2f spriteSize(s.getTexture()->getSize().x * s.getScale().x,s.getTexture()->getSize().y * s.getScale().y);
//
//	a = spriteSize.x;
//	b = spriteSize.y;
//	int aa = a/2;
//	int bb = b/4;
//
//	sf::Sprite s1(s);
//	sf::Sprite s2(s);
//	sf::Sprite s3(s);
//	sf::Sprite s4(s);
//	sf::Sprite s5(s);
//	sf::Sprite s6(s);
//
//	sf::Sprite ss(s);
//	sf::Sprite ss1(s);
//	sf::Sprite ss2(s);
//	sf::Sprite ss3(s);
//	sf::Sprite ss4(s);
//	sf::Sprite ss5(s);
//	sf::Sprite ss6(s);
//
//	sf::Sprite sss(s);
//	sf::Sprite sss1(s);
//	sf::Sprite sss2(s);
//	sf::Sprite sss3(s);
//	sf::Sprite sss4(s);
//	sf::Sprite sss5(s);
//	sf::Sprite sss6(s);
//
//	while (window.isOpen())
//	{
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed) window.close();
//			if (event.type == sf::Event::KeyPressed)
//			{
//				// Получаем нажатую клавишу - выполняем соответствующее действие
//				if (event.key.code == sf::Keyboard::LControl) window.close();
//			}
//			if (event.type == sf::Event::MouseMoved)
//			{
//				sf::Vector2i pos = sf::Mouse::getPosition(window);
//
//				int ak = -100;
//
//				s.setPosition(pos.x - a/2 - ak, pos.y);
//				s1.setPosition(pos.x - 3*aa/2- a/4 - ak, pos.y);
//				s2.setPosition(pos.x - 5*aa/2- a/4 - ak, pos.y);
//				s3.setPosition(pos.x - 7*aa/2- a/4 - ak, pos.y);
//				s4.setPosition(pos.x + aa/2- a/4 - ak, pos.y);
//				s5.setPosition(pos.x + 3*aa/2- a/4 - ak, pos.y);
//				s6.setPosition(pos.x + 5*aa/2- a/4 - ak, pos.y);
//
//				ss.setPosition(pos.x - a/2, pos.y + bb);
//				ss1.setPosition(pos.x - 3*aa/2- a/4, pos.y + bb);
//				ss2.setPosition(pos.x - 5*aa/2- a/4, pos.y + bb);
//				ss3.setPosition(pos.x - 7*aa/2- a/4, pos.y + bb);
//				ss4.setPosition(pos.x + aa/2- a/4, pos.y + bb);
//				ss5.setPosition(pos.x + 3*aa/2- a/4, pos.y + bb);
//				ss6.setPosition(pos.x + 5*aa/2- a/4, pos.y + bb);
//
//				sss.setPosition(pos.x - a/2 + ak, pos.y + 2*bb);
//				sss1.setPosition(pos.x - 3*aa/2- a/4 + ak, pos.y + 2*bb);
//				sss2.setPosition(pos.x - 5*aa/2- a/4 + ak, pos.y + 2*bb);
//				sss3.setPosition(pos.x - 7*aa/2- a/4 + ak, pos.y + 2*bb);
//				sss4.setPosition(pos.x + aa/2- a/4 + ak, pos.y + 2*bb);
//				sss5.setPosition(pos.x + 3*aa/2- a/4 + ak, pos.y + 2*bb);
//				sss6.setPosition(pos.x + 5*aa/2- a/4 + ak, pos.y + 2*bb);
//
//				// Получаем нажатую клавишу - выполняем соответствующее действие
//			}
//		}
//
//		// Выполняем необходимые действия по отрисовке
//		window.clear();
////		window.draw(shape);
//		window.draw(s3);
//		window.draw(s2);
//		window.draw(s1);
//		window.draw(s);
//		window.draw(s4);
//		window.draw(s5);
//		window.draw(s6);
//
//		window.draw(ss3);
//		window.draw(ss2);
//		window.draw(ss1);
//		window.draw(ss);
//		window.draw(ss4);
//		window.draw(ss5);
//		window.draw(ss6);
//
//		window.draw(sss3);
//		window.draw(sss2);
//		window.draw(sss1);
//		window.draw(sss);
//		window.draw(sss4);
//		window.draw(sss5);
//		window.draw(sss6);
//
//		window.display();
//	}
	
//	std::string a = "testOper.cfg";
//	level.spawnOperative(a);
	std::string name = "2123";
	Point coord(1, 2);
	int force = 1, accuracy = 1;
	float reloadTime = 1;
	Operative *op = new Operative(name, coord, reloadTime, force, accuracy);
	std::string a;
	
	std::ifstream fs("test.txt");
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string type;
	int x, y;
	while(fs >> type){
		fs >> x;
		fs >> y;
		std::cout << "Type:" << type << " XY:" << x << ' ' << y << std::endl;
	}
	
	fs.close();

	return 0;
}