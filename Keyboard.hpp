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
	jump
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
					if(actionStates[keyMap.first] 		== released)
					{
						actionStates[keyMap.first] = tapped;
					}
					else if(actionStates[keyMap.first] 	== tapped)
					{
						actionStates[keyMap.first] = pressed;
					}
					break;
				}
			}
			if(!isPressed)
			{
				if(actionStates[keyMap.first] 		== pressed)
				{
					actionStates[keyMap.first] = untapped;
				}
				else if(actionStates[keyMap.first] 	== untapped)
				{
					actionStates[keyMap.first] = released;
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
	
	
	
	static Vector2f getMouseScreen()
	{
		return sf::Mouse::getPosition();
	}
	static Vector2f getMouseWindow(const sf::RenderWindow& rw)
	{
		return sf::Mouse::getPosition(rw);
	}
	static Vector2f getMouseView(const sf::RenderWindow& rw)
	{
		return rw.getView().getInverseTransform().transformPoint(sf::Vector2f(sf::Mouse::getPosition(rw)));
	}
	
	
	
};

std::map<Action, std::vector<Controls::InputButton*> > 		Controls::inputMap;
std::map<Action, Controls::KeyState>						Controls::actionStates;

#endif // KEYBOARD_HPP_INCLUDED
