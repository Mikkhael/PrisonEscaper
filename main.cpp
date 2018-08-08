#include <SFML/Graphics.hpp>
bool TAK= false;
#include "Keyboard.hpp"
#include "Colisions.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Cannon.hpp"
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prison Escaper");
    
    window.setView(sf::View({0,0,500,500}));
	
	Controls::addKeyMapping(Action::left, 	sf::Keyboard::A);
	Controls::addKeyMapping(Action::up, 	sf::Keyboard::W);
	Controls::addKeyMapping(Action::down, 	sf::Keyboard::S);
	Controls::addKeyMapping(Action::right, 	sf::Keyboard::D);
	Controls::addKeyMapping(Action::jump, 	sf::Keyboard::Space);
	Controls::addKeyMapping(Action::shoot, 	sf::Mouse::Left);
	
	Controls::bindWindow(window);
	
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
		
    
    sf::CircleShape circle;
    circle.setFillColor(sf::Color::Red);
    circle.setRadius(3);
    circle.setOrigin(3, 3);
    
		
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
			
			TAK = true;
			player.update(subDeltaTime);
			TAK = false;
			Cannonball::updateAll(subDeltaTime);
			deltaTime -= maxSubsteppingDeltaTime;
		}
		
		circle.setPosition(Controls::getMouseView());
        
        // Draw
        window.clear();
        
        Room::drawAll(window);
		/*
        for(auto& platform : platforms)
        {
            platform.draw(window);
        }
        */
		Cannonball::drawAll(window);
        player.draw(window);
        
        window.draw(circle);
        
        window.display();
    }

    return 0;
}
