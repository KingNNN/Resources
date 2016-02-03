#if defined(_WIN32) || (_WIN64)
	#include "SDL.h"
	#include "SDL_image.h"
#endif

#if defined(__APPLE__)
	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"
#endif

#if defined(__linux__)
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
#endif

//needed includes
#include <stdio.h>
#include <iostream>
using namespace std;

//Bullet includes
#include <vector>
#include "bullet.h"



class Player
{
public:

	//variable to hld the list of bullets
	vector<Bullet> bulletList;

	//string to hold the path to the player's texture
	string playerPath;

	//Integer for the player's number
	int playerNum;

	//player's surface to load png into
	SDL_Surface *surface;

	//player's texture to hold img
	SDL_Texture *texture;

	//player's SDL_Rect for the X, Y, W, and H of texture
	SDL_Rect posRect;

	//floats to hold the hoystick axis (-1 or 0 or 1)
	float xDir;
	float yDir;

	//float for player speed
	float speed;

	//float for the player's position to avoid precision loss
	float pos_X;
	float pos_Y;

	//Player's creation method using passed in values for renderer, player number, path to teh texture
	//starting position X, starting position Y
	Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y);

	//update the plyaer using the passed in deltaTime
	void Update(float deltaTime);

	//draw the player main's passed in renderer
	void Draw(SDL_Renderer *renderer);

	//method to allow the player the move via joystick axis
	void OnControllerAxis(const SDL_ControllerAxisEvent event);

	//method to allow the player interact with joystick buttons
	void OnControllerButton(const SDL_ControllerButtonEvent event);

	//player's destruction method
	//Player();

};
