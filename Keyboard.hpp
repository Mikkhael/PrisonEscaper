#ifndef KEYBOARD_HPP_INCLUDED
#define KEYBOARD_HPP_INCLUDED

#include <map>
#include <vector>
#include "Vectors.hpp"

enum Action
{
	left,
	right,
	up,
	down,
	jump,
	shoot,
};

class Controls
{
	
	enum KeyState
	{
		released,
		tapped,
		pressed,
		untapped
	};
	
	class InputButton
	{
	public:
		virtual bool isPressed() = 0;
		
		virtual ~InputButton(){};
	};
	
	class InputButtonKeyboard : public InputButton
	{
		sf::Keyboard::Key key;
	public:
		
		InputButtonKeyboard(const sf::Keyboard::Key& key_)
			: key(key_)
		{}
		virtual ~InputButtonKeyboard(){};
		
		virtual bool isPressed()
		{
			return sf::Keyboard::isKeyPressed(key);
		}
	};
	
	class InputButtonMouse: public InputButton
	{
		sf::Mouse::Button button;
	public:
		
		InputButtonMouse(const sf::Mouse::Button& button_)
			: button(button_)
		{}
		virtual ~InputButtonMouse(){};
		
		virtual bool isPressed()
		{
			return sf::Mouse::isButtonPressed(button);
		}
	};
	
	static std::map<Action, std::vector<InputButton*> > inputMap;
	static std::map<Action, KeyState>					actionStates;
	
	static sf::RenderWindow* bindedWindow;
	
public:
	
	static void clearKeyMapping(Action action)
	{
		for(auto& mapping : inputMap[action])
		{
			delete mapping;
		}
		inputMap[action].clear();
		actionStates[action] = released;
	}
	
	static void addKeyMapping(Action action, const sf::Keyboard::Key& inputButton)
	{
		inputMap[action].push_back(new InputButtonKeyboard(inputButton));
		actionStates[action] = released;
	}
	static void addKeyMapping(Action action, const sf::Mouse::Button& inputButton)
	{
		inputMap[action].push_back(new InputButtonMouse(inputButton));
		actionStates[action] = released;
	}
		
	static void updateKeyStates()
	{
		for(auto& keyMap : inputMap)
		{
			bool isPressed = false;
			for(auto& key : keyMap.second)
			{
				if(key->isPressed())
				{
					isPressed = true;
					if(actionStates[keyMap.first] 		== tapped)
					{
						actionStates[keyMap.first] = pressed;
					}
					else if(actionStates[keyMap.first] 	!= pressed)
					{
						actionStates[keyMap.first] = tapped;
					}
					break;
				}
			}
			if(!isPressed)
			{
				if(actionStates[keyMap.first] 		== untapped)
				{
					actionStates[keyMap.first] = released;
				}
				else if(actionStates[keyMap.first] 	!= released)
				{
					actionStates[keyMap.first] = untapped;
				}
			}
		}
	}
	
	static bool isPressed(Action action)
	{
		return actionStates[action] == pressed || actionStates[action] == tapped;
	}
	
	static bool isTapped(Action action)
	{
		return actionStates[action] == tapped;
	}
	
	static bool isUntapped(Action action)
	{
		return actionStates[action] == untapped;
	}
	
	static KeyState getState(Action action)
	{
	    return actionStates[action];
	}
	
	
	static void bindWindow(sf::RenderWindow& rw)
	{
		bindedWindow = &rw;
	}
	
	static Vector2f getMouseScreen()
	{
		return sf::Mouse::getPosition();
	}
	static Vector2f getMouseWindow(const sf::RenderWindow& rw)
	{
		return sf::Mouse::getPosition(rw);
	}
	static Vector2f getMouseWindow()
	{
		if(!bindedWindow)
			return Vector2f(0,0);
		return sf::Mouse::getPosition(*bindedWindow);
	}
	static Vector2f getMouseView(const sf::RenderWindow& rw)
	{
		return rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
	}
	static Vector2f getMouseView()
	{
		if(!bindedWindow)
			return Vector2f(0,0);
		return bindedWindow->mapPixelToCoords(sf::Mouse::getPosition(*bindedWindow));
	}
	
	
};

std::map<Action, std::vector<Controls::InputButton*> > 		Controls::inputMap;
std::map<Action, Controls::KeyState>						Controls::actionStates;

sf::RenderWindow* Controls::bindedWindow;

#endif // KEYBOARD_HPP_INCLUDED
