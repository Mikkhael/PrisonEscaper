#include <SFML/Graphics.hpp>
#include "Keyboard.hpp"
#include "Colisions.hpp"
#include "Room.hpp"
#include "Player.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prison Escaper");
	
	Controls::setKeyMapping(sf::Keyboard::A, 		Action::left);
	Controls::setKeyMapping(sf::Keyboard::W, 		Action::up);
	Controls::setKeyMapping(sf::Keyboard::S, 		Action::down);
	Controls::setKeyMapping(sf::Keyboard::D, 		Action::right);
	Controls::setKeyMapping(sf::Keyboard::Space, 	Action::jump);
	
	
	
	
	Collider* c1 = new RectCollider(Rect<double>(40, -100, 30, 200));
	Collider* c2 = new CircleCollider( Circle<double>(Vector2<double>(0,10), 50.0));
	
	auto r = c2->test(*c1);
	
	std::cout << bool(r) << std::endl;
	std::cout << r.xPenetration << std::endl;
	std::cout << r.yPenetration << std::endl;
	
	
	
	WallActor wall(WallTypes::Bricks);
	wall.setPosition(Vector2d(100, 20));
	wall.setSize(Vector2i(200, 600));
	
	
	std::vector<Room> rooms;
	std::vector<Platform> platforms;
	
	rooms.emplace_back(Rect<double>(50, 15, 390, 155), WallTypes::Rocks,  platforms);
	rooms.emplace_back(Rect<double>(25, 170, 200, 30), WallTypes::Bricks, platforms);
	rooms.emplace_back(Rect<double>(40, 200, 400, 20), WallTypes::Bricks, platforms);
	rooms.emplace_back(Rect<double>(440, 15, 50, 300), WallTypes::Bricks, platforms);
	rooms.emplace_back(Rect<double>(100, 100, 40, 40), WallTypes::Bricks);
	
	
	Player player(Vector2d(20, 50));
		
    Platform::mergeAll(platforms);
    
		
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
		
		Controls::updateKeyStates();
		
		if(Controls::isPressed(Action::jump))
		{
			std::cout << "SPACE";
			if(Controls::isTapped((Action::jump)))
			{
				std::cout << "  TAPPED";
			}
			std::cout << std::endl;
		}
		
		
        player.update(deltaTime);
        
        
        window.clear();
        
        for(auto& room : rooms)
        {
            room.draw(window);
        }
        for(auto& platform : platforms)
        {
            platform.draw(window);
        }
        player.draw(window);
        
        window.display();
    }

    return 0;
}
