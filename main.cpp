#include <SFML/Graphics.hpp>
#include "Colisions.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	
	Collider* c1 = new RectCollider(Rect<double>(40, -100, 30, 200));
	Collider* c2 = new CircleCollider( Circle<double>(Vector2<double>(0,10), 50.0));
	
	auto r = c2->test(*c1);
	
	std::cout << bool(r) << std::endl;
	std::cout << r.xPenetration << std::endl;
	std::cout << r.yPenetration << std::endl;
		
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
