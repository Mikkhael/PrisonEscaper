#include <SFML/Graphics.hpp>
#include "Colisions.hpp"
#include "Room.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prison Escaper");
	
	Collider* c1 = new RectCollider(Rect<double>(40, -100, 30, 200));
	Collider* c2 = new CircleCollider( Circle<double>(Vector2<double>(0,10), 50.0));
	
	auto r = c2->test(*c1);
	
	std::cout << bool(r) << std::endl;
	std::cout << r.xPenetration << std::endl;
	std::cout << r.yPenetration << std::endl;
	
	
	
	
	WallActor wall(WallTypes::Bricks);
	wall.setPosition(Vector2d(100, 20));
	wall.setSize(Vector2i(200, 600));
	
	
	Room room1(Rect<double>(10, 15, 500, 155), WallTypes::Rocks);
	Room room2(Rect<double>(100, 100, 40, 40), WallTypes::Bricks, false);
		
	sf::Clock clock;
    double deltaTime   = 0;
    double currentTime = clock.getElapsedTime().asSeconds();
	double lastTime    = currentTime;
		
    while (window.isOpen())
    {
    	currentTime = clock.getElapsedTime().asSeconds();
    	deltaTime	= currentTime - lastTime;
    	lastTime	= currentTime;
    	
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        room1.draw(window);
        room2.draw(window);
        
        window.display();
    }

    return 0;
}
