#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED
#include <vector>
#include "Player.hpp"
#include "Cannon.hpp"
#include "Room.hpp"

class Level
{
public:
	
	std::vector<Actor*> 		actors;
	
	std::vector<Room*> 			rooms;
	std::vector<Cannonball*> 	cannonballs;
	
	Player* 					player = nullptr;
	
	std::vector<Platform> 		platforms;
	
	
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
	
	bool addActor(Actor* ptr)
	{
		if(contains(ptr))
		{
			return false;
		}
		actors.push_back(ptr);
		return true;
	}
	
	bool removeActor(Actor* ptr)
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
	
	template<class T>
	bool removeFromGroup(Actor* ptr, std::vector<T>& list)
	{
		if(!ptr)
		{
			return false;
		}
		for(auto it = list.begin(); it < list.end(); it++)
		{
			if(*it == ptr)
			{
				removeActor(ptr);
				list.erase(it);
				return true;
			}
		}
		return false;
	}
	
	void spawn(Player* player_)
	{
		if(addActor(player_))
		{
			player = player_;
		}
	}
	
	void spawn(Room* room)
	{
		if(addActor(room))
		{
			rooms.push_back(room);
		}
	}
	void despawn(Room* room)
	{
		removeFromGroup(room, rooms);
	}
	void spawn(Cannonball* cannonball)
	{
		if(addActor(cannonball))
		{
			cannonball.push_back(cannonball);
		}
	}
	void despawn(Cannonball* cannonball)
	{
		removeFromGroup(cannonball, cannonballs);
	}
	
	
};

#endif // LEVEL_HPP_INCLUDED
