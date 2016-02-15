#if defined(_WIN32) || (_WIN64)
#include <SDL.h>
#include <SDL_image.h>
#endif

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#if defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <string>
#include <iostream>

using namespace std;

class Explode {

public:

	//bolllean for the state of the explosion
	bool active;

	//explosion texture
	SDL_Texture *texture;

	//explosion rectangle for position (X,Y) and size (W, H)
	SDL_Rect posRect;

	//explosion rectangle for position (X,Y) and size (W, H)
	SDL_Rect drawRect;

	//width and height of each frame plus width and height of entire image
	int frameWidth, frameHeight, textureWidth, textureHeight;

	//float values to track time until next frame of animation
	float frameCounter;

	//Explode creation method using passed in values for renderer, path to teh texture starting position X, starting position Y
	Explode(SDL_Renderer *renderer, string filePath, float x, float y);

	//update the plyaer using the passed in deltaTime
	void Update(float deltaTime);

	//draw the player main's passed in renderer
	void Draw(SDL_Renderer *renderer);

	//reset the player
	void Reset();

	//Explode destruction method
	~Explode();

};