#include <SFML/Graphics.hpp>
#include "Colisions.hpp"
#include "WallActor.hpp"

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
	wall.transform.position = Vector2d(100, 20);
	wall.setSize(Vector2i(200, 600));
	
	
	double offsetCounter = 0;
		
	sf::Clock clock;
    double deltaTime = 0;
		
    while (window.isOpen())
    {
    	deltaTime = clock.restart().asSeconds();
    	
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        
        offsetCounter += deltaTime*15;
        while(offsetCounter > 1.0)
		{
			wall.setOffset(Vector2i(wall.getOffset().x + 1, 0));
			offsetCounter -= 1.0;
		}
		
        wall.draw(window);
        
        
        
        window.display();
    }

    return 0;
}
