#include <SFML/Graphics.hpp>
#include "Keyboard.hpp"
#include "Colisions.hpp"
#include "LightEmitter.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "WallTurret.hpp"
#include "Cannon.hpp"
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prison Escaper");
    
    const float zoom = 0.7;
    
    window.setView(sf::View({0,0,800 * zoom, 600 * zoom}));
	
	Controls::addKeyMapping(Action::left, 	sf::Keyboard::A);
	Controls::addKeyMapping(Action::up, 	sf::Keyboard::W);
	Controls::addKeyMapping(Action::down, 	sf::Keyboard::S);
	Controls::addKeyMapping(Action::right, 	sf::Keyboard::D);
	Controls::addKeyMapping(Action::jump, 	sf::Keyboard::Space);
	Controls::addKeyMapping(Action::shoot, 	sf::Mouse::Left);
	
	Controls::bindWindow(window);
	
	LightEmitter::createDefaultLightmapTextures(window.getSize());
	
	std::cout << "Generating map..." << std::endl;
	
	Room::spawn(new Room(Rect<double>(50, 15, 390, 155), WallTypes::Rocks,  platforms));
	Room::spawn(new Room(Rect<double>(25, 170, 200, 30), WallTypes::Bricks, platforms));
	Room::spawn(new Room(Rect<double>(40, 200, 400, 20), WallTypes::Bricks, platforms));
	Room::spawn(new Room(Rect<double>(440, 15, 50, 300), WallTypes::Bricks, platforms));
	Room::spawn(new Room(Rect<double>(100, 100, 40, 40), WallTypes::Bricks));
	
	
	std::cout << "Adding collision platforms..." << std::endl;
    Platform::mergeAll(platforms);
    
	
	//platforms.push_back(Platform(Vector2d(100,100), 100, true));
	
	std::cout << "Spawning player..." << std::endl;
	Player player(Vector2d(70, 50));
	
	
	WallTurret::spawn(new WallTurret(WallTurret::Left, {50, 60}));
	WallTurret::spawn(new WallTurret(WallTurret::Up, {200, 15}));
    
    
	sf::Clock clock;
    double deltaTime   = 0;
    double currentTime = clock.getElapsedTime().asSeconds();
	double lastTime    = currentTime;
	//double subDeltaTime = 0;
	
	//double maxSubsteppingDeltaTime = 0.036;
	
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
		
		//std::cout << deltaTime << std::endl;
		
		// Update
		
		/*
		while(deltaTime > 0)
		{
			subDeltaTime = deltaTime > maxSubsteppingDeltaTime ? maxSubsteppingDeltaTime : deltaTime;
			
			player.update(subDeltaTime);
			Cannonball::updateAll(subDeltaTime);
			deltaTime -= maxSubsteppingDeltaTime;
		}
		*/
		
		player.update(deltaTime);
        Cannonball::updateAll(deltaTime);
        WallTurret::updateAll(deltaTime);
        
        
        // Draw
        window.clear(sf::Color::Black);
        
        Room::drawAll(window);
		/*
        for(auto& platform : platforms)
        {
            platform.draw(window);
        }
        */
        WallTurret::drawAll(window);
		Cannonball::drawAll(window);
        player.draw(window);
        
        LightEmitter::generateAndApplyLightMap(window);
        
        window.display();
    }

    return 0;
}
