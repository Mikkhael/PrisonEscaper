#ifndef KEYBOARD_HPP_INCLUDED
#define KEYBOARD_HPP_INCLUDED

#include <map>

enum Action
{
	left,
	right,
	up,
	down,
	jump
};

class Controls
{
	
	enum KeyState
	{
		released,
		tapped,
		pressed
	};
	
	static std::map<sf::Keyboard::Key, Action> 	keyMap;
	static std::map<Action, KeyState>		keyStates;
	
public:
	
	static void setKeyMapping(sf::Keyboard::Key keyCode, const Action& action)
	{
		keyMap[keyCode] 	= action;
		keyStates[action] 	= released;
	}
	
	static void updateKeyStates()
	{
		for(auto& key : keyMap)
		{
			if(sf::Keyboard::isKeyPressed(key.first))
			{
				if(keyStates[key.second] >= tapped)
				{
					keyStates[key.second] = pressed;
				}
				else
				{
					keyStates[key.second] = tapped;
				}
			}
			else
			{
				keyStates[key.second] = released;
			}
		}
	}
	
	static bool isPressed(const Action& action)
	{
		if(keyStates[action] >= tapped)
		{
			return true;
		}
		return false;
	}
	
	static bool isTapped(const Action& action)
	{
		if(keyStates[action] == tapped)
		{
			return true;
		}
		return false;
	}
};

std::map<sf::Keyboard::Key, Action> 	Controls::keyMap;
std::map<Action, Controls::KeyState>	Controls::keyStates;

#endif // KEYBOARD_HPP_INCLUDED
