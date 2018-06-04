#include <SFML/Graphics.hpp>
#include "Keyboard.hpp"
#include "Colisions.hpp"
#include "Room.hpp"
#include "Player.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prison Escaper");
	
	Controls::addKeyMapping(Action::left, 	sf::Keyboard::A);
	Controls::addKeyMapping(Action::up, 	sf::Keyboard::W);
	Controls::addKeyMapping(Action::down, 	sf::Keyboard::S);
	Controls::addKeyMapping(Action::right, 	sf::Keyboard::D);
	Controls::addKeyMapping(Action::jump, 	sf::Keyboard::Space);
	Controls::addKeyMapping(Action::jump, 	sf::Mouse::Left);
	
	rooms.emplace_back(Rect<double>(50, 15, 390, 155), WallTypes::Rocks,  platforms);
	rooms.emplace_back(Rect<double>(25, 170, 200, 30), WallTypes::Bricks, platforms);
	rooms.emplace_back(Rect<double>(40, 200, 400, 20), WallTypes::Bricks, platforms);
	rooms.emplace_back(Rect<double>(440, 15, 50, 300), WallTypes::Bricks, platforms);
	rooms.emplace_back(Rect<double>(100, 100, 40, 40), WallTypes::Bricks);
	
    Platform::mergeAll(platforms);
    
	
	//platforms.push_back(Platform(Vector2d(100,100), 100, true));
	
	Player player(Vector2d(70, 50));
		
    
		
	sf::Clock clock;
    double deltaTime   = 0;
    double currentTime = clock.getElapsedTime().asSeconds();
	double lastTime    = currentTime;
	double subDeltaTime = 0;
	
	double maxSubsteppingDeltaTime = 0.036;
	
    sf::Event event;
    while (window.isOpen())
    {
    	currentTime = clock.getElapsedTime().asSeconds();
    	deltaTime	= currentTime - lastTime;
    	lastTime	= currentTime;
    	
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		Controls::updateKeyStates();
		
		// Update
		while(deltaTime > 0)
		{
			subDeltaTime = deltaTime > maxSubsteppingDeltaTime ? maxSubsteppingDeltaTime : deltaTime;
			
			player.update(subDeltaTime);
			
			deltaTime -= maxSubsteppingDeltaTime;
		}
        
        
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
