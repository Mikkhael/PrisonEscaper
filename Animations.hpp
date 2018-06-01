#ifndef ANIMATIONS_HPP_INCLUDED
#define ANIMATIONS_HPP_INCLUDED
#include "TextureManager.hpp"
/*
enum AnimationState{Stop, Once, Loop};

class AnimationPreset
{
public:

    sf::IntRect rect;
    double length;
    unsigned int framesCount;
    unsigned int framesPerRow;

    AnimationPreset(sf::IntRect r, double l=0, unsigned int fc = 1, unsigned int fpr=0)
        : rect(r), length(l), framesCount(fc), framesPerRow(fpr)
    {
    }
};


class Animation
{

    double frameCounter;

public:
    sf::IntRect baseFrame;
    double length;
    unsigned int framesCount;
    unsigned int framesPerRow;

    AnimationState state = Stop;

    void update(double deltaTime)
    {
        if(state == Stop) return;
        frameCounter += deltaTime;
        if(frameCounter > length)
        {
            frameCounter = 0;
            if(state == Once)
                state = Stop;
        }
    }

    void reset()
    {
        frameCounter = 0;
    }

    sf::IntRect get() const
    {
        return get(framesCount * frameCounter/length);
    }

    sf::IntRect get(int f) const
    {
        f%=framesCount;
        if(framesPerRow == 0)
        {
            return sf::IntRect(baseFrame.left + f*baseFrame.width,
                               baseFrame.top,
                               baseFrame.width,
                               baseFrame.height);
        }
        else
        {
            int hf = f%framesPerRow;
            int vf = f/framesPerRow;
            return sf::IntRect(baseFrame.left + hf*baseFrame.width,
                               baseFrame.top  + vf*baseFrame.height,
                               baseFrame.width,
                               baseFrame.height);
        }
    }
    Animation(const AnimationPreset& a)
        : baseFrame(a.rect), length(a.length), framesCount(a.framesCount), framesPerRow(a.framesPerRow)
    {

    }
    void setPreset(const AnimationPreset& a)
    {
        reset();
        baseFrame = a.rect;
        length = a.length;
        framesCount = a.framesCount;
        framesPerRow = a.framesPerRow;
    }
    bool compareWithPreset(const AnimationPreset& ap)
    {
        return baseFrame    == ap.rect         &&
               length       == ap.length       &&
               framesCount  == ap.framesCount  &&
               framesPerRow == ap.framesPerRow;
    }
};


namespace AnimationPresets
{
    const AnimationPreset PlayerIdle{sf::IntRect(0,0,16,16), 1, 2};
    const AnimationPreset PlayerWalk{sf::IntRect(0,16,16,16), 0.5, 4};
    const AnimationPreset PlayerFly {sf::IntRect(16,16,16,16), 0, 1};
}
*/


enum AnimationState
{
	Stop,
	Once,
	Loop
};


struct AnimatedSpritePreset
{
	std::string 	textureName;
	sf::IntRect 	baseFrame;
	double 			length;
	unsigned int	framesCount;
	unsigned int	framesPerRow;
	
	bool compare(const AnimatedSpritePreset& a) const
	{
		return 	textureName == a.textureName &&
				length		== a.length		 &&
				framesCount	== a.framesCount &&
				baseFrame	== a.baseFrame	 &&
				framesPerRow== a.framesPerRow;
	}
	
	AnimatedSpritePreset(const std::string& textureName_, const sf::IntRect& baseFrame_, double length_=0, unsigned int framesCount_=1, unsigned int framesPerRow_=0)
		: textureName(textureName_), baseFrame(baseFrame_), length(length_), framesCount(framesCount_), framesPerRow(framesPerRow_)
	{}
};

namespace AnimatedSpritePresets
{
	const AnimatedSpritePreset	PlayerIdle		= AnimatedSpritePreset("assets/creatures/player.bmp", 	sf::IntRect(0,0,16,16), 	2, 		2);
	const AnimatedSpritePreset	PlayerWalk		= AnimatedSpritePreset("assets/creatures/player.bmp", 	sf::IntRect(0,16,16,16), 	0.5, 	4);
	const AnimatedSpritePreset	PlayerFall		= AnimatedSpritePreset("assets/creatures/player.bmp", 	sf::IntRect(16,16,16,16), 	-1,		1);
}

class AnimatedSprite : public sf::Sprite
{
	double 			frameCounter 	= 0;
	unsigned int 	frame			= 0;
	sf::IntRect		frameRect;
	
	AnimatedSpritePreset preset;
	
	inline double getFrameLength() const
	{
		return preset.length / preset.framesCount;
	}
	
	void updateTextureRect()
	{
		unsigned int yOffset = preset.framesPerRow != 0 ? frame/preset.framesPerRow : 0;
		unsigned int xOffset = preset.framesPerRow != 0 ? frame%preset.framesPerRow : frame;		
				
		frameRect = sf::IntRect( 	preset.baseFrame.left 	+ xOffset * preset.baseFrame.width 	+ (flipX ? preset.baseFrame.width : 0),
									preset.baseFrame.top 	+ yOffset * preset.baseFrame.height + (flipY ? preset.baseFrame.height: 0),
									preset.baseFrame.width	* (flipX ? -1 : 1),
									preset.baseFrame.height * (flipY ? -1 : 1));
									
		setTextureRect(frameRect);
	}
public:
	
	bool flipX = false;
	bool flipY = false;
	
	AnimationState state;
	
	void reset()
	{
		frame 			= 0;
		frameCounter 	= getFrameLength();
		updateTextureRect();
	}
	
	void reset(const AnimationState& newState)
	{
		frame 			= 0;
		frameCounter 	= getFrameLength();
		state			= newState;
		updateTextureRect();
	}
	
	void setFrame(unsigned int frame_)
	{
		frame 			= frame_;
		frameCounter 	= getFrameLength();
		updateTextureRect();
	}
	void setFrame(unsigned int frame_, const AnimationState& newState)
	{
		frame 			= frame_;
		frameCounter 	= getFrameLength();
		state			= newState;
		updateTextureRect();
	}
	
	void nextFrame()
	{
		frame++;
		frameCounter = getFrameLength();
		if(frame >= preset.framesCount)
		{
			frame = 0;
			
			if(state == Once)
				state = Stop;
		}
		updateTextureRect();
	}
	
	void updateFrame(double deltaTime)
	{
		if(state == Stop || preset.length < 0)
			return;
		
		frameCounter -= deltaTime;
		if(frameCounter < 0)
		{
			nextFrame();
		}
	}	
	
	AnimatedSprite(const AnimatedSpritePreset& preset_, AnimationState state_ = Loop)
		: sf::Sprite(textureManager.get(preset_.textureName)), preset(preset_), state(state_)
	{}
	
	void setPreset(const AnimatedSpritePreset& preset_, bool noReset = false)
	{
		if(!preset.compare(preset_))
		{
			preset = preset_;
			setTexture(textureManager.get(preset.textureName));
			if(!noReset)
				reset();
			updateTextureRect();
		}
	}
};



#endif // ANIMATIONS_HPP_INCLUDED
