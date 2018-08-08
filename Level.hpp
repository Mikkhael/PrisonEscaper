#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED
#include <vector>
#include "Object.hpp"

class Level
{
public:
	
	std::vector<Actor*> 		actors;
	
	void update(double deltaTime)
	{
		for(auto& actor : actors)
		{
			actor->update(deltaTime);
		}
	}
	void draw(sf::RenderTarget& rt, const sf::RenderStates& states = sf::RenderStates::Default)
	{
		for(auto& actor : actors)
		{
			actor->draw(rt, states);
		}
	}
	
	std::vector<Actor*>::const_iterator getActor(Actor* ptr)
	{
		if(!ptr)
		{
			return actors.end();
		}
		for(auto it = actors.begin(); it < actors.end(); it++)
		{
			if(*it == ptr)
			{
				return it;
			}
		}
		return actors.end();
	}
	bool contains(Actor* ptr)
	{
		if(!ptr)
		{
			return false;
		}
		return getActor(ptr) == actors.end();
	}
	
	bool spawn(Actor* ptr)
	{
		if(contains(ptr))
		{
			return false;
		}
		actors.push_back(ptr);
		return true;
	}
	
	bool despawn(Actor* ptr)
	{
		auto it = getActor(ptr);
		if(it == actors.end())
		{
			return false;
		}
		delete *it;
		actors.erase(it);
		return true;
	}	
};

#endif // LEVEL_HPP_INCLUDED
