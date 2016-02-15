#include "explode.h"

//Explode creation method
Explode::Explode(SDL_Renderer *renderer, string filePath, float x, float y)
{
	//set the explosion initial state
	active = false;

	//get the directory to the explosion graphic
	string explodePath = filePath + "explosion.png";

	//init frame counter
	frameCounter = 0.0f;

	//Load the texture into a surface
	SDL_Surface *surface = IMG_Load(explodePath.c_str());

	//Create the texture from teh passed renderer and create surface
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surafce for later use
	SDL_FreeSurface(surface);

	//set the width adn height of the bullet's recrtangel by querying the texture itself
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	//getting tht ewidth adn height of the entire image
	textureWidth = w;
	textureHeight = h;

	//setting the individual image size
	posRect.w = 80;
	posRect.h = 128;

	//set the x and y positions of the exposion rectangle
	posRect.x = x;
	posRect.y = y;

	//frame width and height of each aniamtion image
	frameWidth = 128;
	frameHeight = 128;

	//original draw rectangle used to draw on eframe of hte animation strip
	drawRect.x = 128;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;

}

//reset the bullet method
void Explode::Reset()
{
	//deactivate the bullet
	active = false;

}

//bullet update method
void Explode::Update(float deltaTime)
{
	//check to see if the bulet is active
	if (active)
	{
		//add the frame counter
		frameCounter += deltaTime;

		//check to see if enough time has passed
		if (frameCounter >= .035f)
		{
			//reset frame counter
			frameCounter = 0.0f;

			//add to the drawRect teh width of an individual frame to move the drawing area over one
			drawRect.x += frameWidth;

			//see if we have gone pas the edge of the image - width
			if (drawRect.x >= textureWidth)
			{
				//if so, go back to the first - 0
				drawRect.x = 0;

				//set active set to false
				active = false;

				//move off screen until needed again
				posRect.x = -1000;
				posRect.y = -1000;
			}

		}
	}
}
//bullet draw method
void Explode::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
}

//bulet desctruction
Explode::~Explode()
{
	//SDL_DestroyTexture(texture);
}