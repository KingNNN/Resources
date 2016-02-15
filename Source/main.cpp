
// Example program:
// Using SDL2 to create an application window

#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#endif

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

#endif

#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

#endif

#if defined(__linux__)
#include <unistd.h>
#endif

//needed includes
#include <stdio.h>
#include <iostream>
using namespace std;

//code for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//set speed for background
int bkgdSpeed = 100;

//create the SDL_Rectangle for the texture'es position and size - x,y,w,h
SDL_Rect bkgd1Pos, bkgd2Pos;

//set temp variable to hold movement
float BG1pos_X = 0.0, BG1pos_Y = 0.0;
float BG2pos_X = 0.0, BG2pos_Y = -768;

//move the background
void UpdateBackground()
{
	//update bkg1's float position values
	BG1pos_Y += (bkgdSpeed * 1) * deltaTime;
	bkgd1Pos.y = (int)(BG1pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y =-768;
		BG1pos_Y= bkgd1Pos.y;
	}

	//Update bkgd2's float position values
	BG2pos_Y += (bkgdSpeed * 1) * deltaTime;
	bkgd2Pos.y = (int)(BG2pos_Y + 0.5f);

	//reset the float position values of bkgd2 when off the bottom fo screen
	if(bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y =-768;
		BG2pos_Y= bkgd2Pos.y;
	}
}

//new joystick vars
//Analog joystick deadzone
const int JOYSTICK_DEAD_ZONE = 8000;

//joystick direction vars
float xDir, yDir;

//cursor float vars for moement
float pos_X, pos_Y;

//create SDl Rectangel for the 2 palyer graphic
SDL_Rect cursorPos, activePos;

//var from cursor speed
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event)
{
	//check joystick 0 - first player
	if (event.which == 0)
	{
		//check X axis
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;

			}else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}else{
				xDir = 0.0f;
			}
		}

		//check Y axis
		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;

			}else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}else{
				yDir = 0.0f;
			}
		}
	}
}

//update cursor on screen
void UpdateCursor(float deltaTime)
{
//update cursor - float values
	pos_X += (cursorSpeed *xDir) * deltaTime;
	pos_Y += (cursorSpeed *yDir) * deltaTime;

	//assign to SDL_Rect ints X and Y
	cursorPos.x = (int) (pos_X + 0.5f);
	cursorPos.y = (int) (pos_Y + 0.5f);

	//update active position of cursor - collision box
	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	//off the screen in X
	if(cursorPos.x < 0)
	{
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w)
	{
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	//off the screen in Y
	if(cursorPos.y < 0)
	{
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h)
	{
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}

//cariables for all Menus button over
bool players1Over = false;
bool players2Over = false;
bool instructionsOver = false;
bool quitOver = false;
bool menuOver = false;
bool playOver = false;


#include "player.h"
#include "enemy.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "explode.h"

//variable to hold the list of enemies: for 1 player game - 6 total for 2 player games - 12 total
vector<Enemy> enemyList;

vector<Explode> explodeList;

void MakeExplosion(int x, int y)
{
	//see if there is a explision not active to use
	for (int i = 0; i < explodeList.size(); i++)
	{
		//see if the bullet is not actgive
		if (explodeList[i].active == false)
		{
			//set explosions to active
			explodeList[i].active = true;

			//use some math in the x position to get the bullet close to the center of the player using player width
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			//once explosion is found, break out of loop
			break;
		}
	}
}


int main(int argc, char* argv[]) {


	/*initialize random seed: */
	time_t srand (time(NULL));

//Cout to show we are running on Windows
#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows" << endl;
	//string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL, 0));

	//create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory + "\\Resources\\Images\\";
	string audio_dir = currentWorkingDirectory + "\\Resources\\Audio\\";

#endif

//Cout to show that we are running on Linux
#if defined(__linux__)

	cout << "Running on Linux" << endl;
	cout << "Added on Linux" << endl;
	//string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL, 0));

	//create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory + "/Resources/Images/";

	//create a string to link the adio folder on __LINUX__
	string audio_dir = currentWorkingDirectory + "/Resources/Audio/";

#endif

//Cout to show that we are running on Apple
#if defined(__APPLE__)

	cout << "Running on Apple" << endl;

	//string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL,0));

	//create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory +"/Resources/Images/";

	//create a string to link the adio folder on __APPLE__
	string audio_dir = currentWorkingDirectory + "/Resources/Audio/";

#endif


    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    //Create a Renderer variable - pointer
    SDL_Renderer *renderer = NULL;

    //create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    /////////////////////////////////////////////////////////////
    //**************Main Menu - START *****************

    //////////////////////////////////////////////////////////////////////
    //***************** Create Background  - START ***********************

    //string BKGDpath = s_cwd_images + "/bkgd.png";

    //cout << BKGDpath << endl;

    //create a SDL surface to hold the background image
    SDL_Surface *surface = IMG_Load((images_dir + "bkgd.png").c_str());

    //create a SDL texture
    SDL_Texture *bkgd1;

    //place surface info into the texture bkdg1
    bkgd1 = SDL_CreateTextureFromSurface(renderer,surface);

    //create a SDL texture
    SDL_Texture *bkgd2;

    //place surface info into the texture bkdg1
    bkgd2 = SDL_CreateTextureFromSurface(renderer,surface);

    ////free the SDL surface
    SDL_FreeSurface(surface);

    //set the X,Y,W, and H for the Rectangle
    bkgd1Pos.x = 0;
    bkgd1Pos.y = 0;
    bkgd1Pos.w = 1024;
    bkgd1Pos.h = 768;

    //set the X,Y,W, and H for the Rectangle
    bkgd2Pos.x = 0;
    bkgd2Pos.y = -768;
    bkgd2Pos.w = 1024;
    bkgd2Pos.h = 768;


    //***************** Create Background  - END ***********************
    //////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    //************** Create Cursor - START *****************

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "cursor.png").c_str());

    //create a SDL texture - 2 Player button
    SDL_Texture *cursor;

    //place surface into the texture
    cursor = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for player 2
    //SDL_Rect cursorPos, activePos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - cursor graphic
    cursorPos.x = 0;
    cursorPos.y = 0;
    cursorPos.w = 50;
    cursorPos.h = 50;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - cursor graphic
    activePos.x = 10;
    activePos.y = 10;
    activePos.w = 10;
    activePos.h = 10;

    //int cursorSpeed = 400;

    /////////////////////////////////////////////////////////////
    //**************Create Cursor - END *****************

    /////////////////////////////////////////////////////////////
    //**************Create Title - START *****************

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "title.png").c_str());

    //create a SDL texture
    SDL_Texture *title;

    //place surface into the texture
    title = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for the title graphic
    SDL_Rect titlePos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    titlePos.x = 275;
    titlePos.y = 110;
    titlePos.w = 500;
    titlePos.h = 75;


    //**************Create Title - END *****************
    /////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////
    //**************Create Player 1 - START *****************

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "one_n.png").c_str());

    //create a SDL texture
    SDL_Texture *players1N;

    //place surface into the texture
   	players1N = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "one_o.png").c_str());

    //create a SDL texture
    SDL_Texture *players1O;

    //place surface into the texture
   	players1O = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for the title graphic
    SDL_Rect players1NPos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    players1NPos.x = 410;
    players1NPos.y = 220;
    players1NPos.w = 390;
    players1NPos.h = 45;

    //**************Create Player1 - END *****************
    /////////////////////////////////////////////////////////////



    /////////////////////////////////////////////////////////////
    //**************Create Player 2 - START *****************

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "two_n.png").c_str());

    //create a SDL texture - 2 Player button
    SDL_Texture *players2N;

    //place surface into the texture
   	players2N = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "two_o.png").c_str());

    //create a SDL texture
    SDL_Texture *players2O;

    //place surface into the texture
   	players2O = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for player 2
    SDL_Rect players2NPos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    players2NPos.x = 400;
    players2NPos.y = 300;
    players2NPos.w = 390;
    players2NPos.h = 45;

    //**************Create Player2 - END *****************
    /////////////////////////////////////////////////////////////



    /////////////////////////////////////////////////////////////
    //**************Instructions - START *****************

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "instruct_n.png").c_str());

    //create a SDL texture - 2 Player button
    SDL_Texture *instructN;

    //place surface into the texture
   	instructN = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "instruct_o.png").c_str());

    //create a SDL texture
    SDL_Texture *instructO;

    //place surface into the texture
   	instructO = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for player 2
    SDL_Rect instructNPos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    instructNPos.x = 390;
    instructNPos.y = 385;
    instructNPos.w = 262;
    instructNPos.h = 32;

    //**************Instructions - END *****************
    /////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    //**************Quit Game - START *****************

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "quit_n.png").c_str());

    //create a SDL texture - 2 Player button
    SDL_Texture *quitN;

    //place surface into the texture
    quitN = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "quit_o.png").c_str());

    //create a SDL texture
    SDL_Texture *quitO;

    //place surface into the texture
    quitO = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for player 2
    SDL_Rect quitNPos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    quitNPos.x = 465;
    quitNPos.y = 452;
    quitNPos.w = 380;
    quitNPos.h = 48;

    //**************Quit Game - END *****************
    /////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////
    //**************Create Instructions Menu - START *****************

    //********Instructions graphics - START **************
    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "instruct_text.png").c_str());

    //create a SDL texture
    SDL_Texture *instructionsText;

    //place surface into the texture
    instructionsText = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for the title graphic
    SDL_Rect instructionsTextPos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    instructionsTextPos.x = 0;
    instructionsTextPos.y = 0;
    instructionsTextPos.w = 1024;
    instructionsTextPos.h = 768;

    //********Instructions graphics - END **************

    //********Small Menu Button - START **************
    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "menu_n.png").c_str());

    //create a SDL texture
    SDL_Texture *menuN;

    //place surface into the texture
    menuN = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create a SDL surface to hold the background image
    surface = IMG_Load((images_dir + "menu_o.png").c_str());

    //create a SDL texture
    SDL_Texture *menuO;

    //place surface into the texture
    menuO = SDL_CreateTextureFromSurface(renderer,surface);

    //Release the SDL surface for later use
    SDL_FreeSurface(surface);

    //create SDL Rectangle for the title graphic
    SDL_Rect menuNPos;

    //the rectangle which has the X pos, Ypos, texture Width and texture Height - title
    menuNPos.x = 100;
    menuNPos.y = 600;
    menuNPos.w = 200;
    menuNPos.h = 50;

    //********Small Menu Button - START **************

    //**************Create Instructions Menu- END *****************
    /////////////////////////////////////////////////////////////

	//********Lose graphics - START **************
	//create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "lose.png").c_str());

	//create a SDL texture
	SDL_Texture *loseText;

	//place surface into the texture
	loseText = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	//create SDL Rectangle for the title graphic
	SDL_Rect loseTextPos;

	//the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	loseTextPos.x = 240;
	loseTextPos.y = 288;
	loseTextPos.w = 596;
	loseTextPos.h = 114;

	//********Lose graphics - END **************


    //***************** CREATE WIN END ***********************
    /////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//**************Create Win Graphics - START *****************

	//********Instructions graphics - START **************
	//create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "win.png").c_str());

	//create a SDL texture
	SDL_Texture *winText;

	//place surface into the texture
	winText = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	//create SDL Rectangle for the title graphic
	SDL_Rect winTextPos;

	//the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	winTextPos.x = 240;
	winTextPos.y = 288;
	winTextPos.w = 596;
	winTextPos.h = 114;

	//********Win graphics - END **************

	//********Small Play Again Button - START **************
	//create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "play_n.png").c_str());

	//create a SDL texture
	SDL_Texture *playN;

	//place surface into the texture
	playN = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	//create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "play_o.png").c_str());

	//create a SDL texture
	SDL_Texture *playO;

	//place surface into the texture
	playO = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	//create SDL Rectangle for the title graphic
	SDL_Rect playNPos;

	//the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	playNPos.x = 875;
	playNPos.y = 702;
	playNPos.w = 125;
	playNPos.h = 40;

	//********Small Menu Button - START **************

	//**************Create Instructions Menu- END *****************
	/////////////////////////////////////////////////////////////

	//*****Turn on Game Controller Events *****
	SDL_GameControllerEventState(SDL_ENABLE);



	//***** Set up a Game Controller variable *****
	SDL_GameController* gGameController0 = NULL;

	//***** Open Game Controller *****
	gGameController0 = SDL_GameControllerOpen(0);

	//***** Set up a Game Controller variable *****
	SDL_GameController* gGameController1 = NULL;

	//***** Open Game Controller *****
	gGameController1 = SDL_GameControllerOpen(1);



	//***** SDL Event to handle input *****
	SDL_Event event;

	// Set up variables for the game states
	enum GameState {MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

	// Set up initial state
	GameState gameState = MENU;

	// Boolean values to control movement through the states
	bool menu, instructions, players1, players2, win, lose, quit;
	quit = false;

	//Open Audio Channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load a MUSIC file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

	//If the MUSIC file is not playig - play it.
	if(!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgm, -1);
	}

	//Set up a Sound Effects CHUNK for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	//Set up a Sound Effects CHUNK for the button over state
	Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	//Set up a Sound Effects CHUNK for the button over state
	Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "enemy.wav").c_str());

	//bool value to control the over sound effect and the buttons
	bool alreadyOver = false;

    //create renderer
    Player player1 = Player(renderer, 0, images_dir.c_str(), audio_dir.c_str(),250.0,500.0);
    Player player2 = Player(renderer, 1, images_dir.c_str(), audio_dir.c_str(),750.0,500.0);

	//create a pool of explosions - 20
	for (int i = 0; i < 20; i++)
	{
		//create the enemy
		Explode tmpExplode(renderer, images_dir, -1000, -1000);

		//add to enemyList
		explodeList.push_back(tmpExplode);
	}
    // The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)
	{
		switch(gameState)
		{
			case MENU:
				alreadyOver = false;
				menu = true;
				cout << "The Game State is Menu" << endl;
				cout << "Press the A Button for Instructions" << endl;
				cout << "Press the B Button for 1 Player Game" << endl;
				cout << "Press the X Button for 2 Player Game" << endl;
				cout << "Press the Y Button for Quit Game" << endl;
				cout << endl;
				while(menu)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					//check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							menu = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										//if player chooses 1 player game
										if(players1Over)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											menu = false;
											gameState = PLAYERS1;
											players1Over = false;
										}

										//if player chooses 2 player game
										if(players2Over)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											menu = false;
											gameState = PLAYERS2;
											players2Over = false;
										}
										//if player chooses instructions
										if(instructionsOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											menu = false;
											gameState = INSTRUCTIONS;
											instructionsOver = false;
										}
										//if player chooses 1 player game
										if(quitOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);

											//add a slight delay
											SDL_Delay(200);

											menu = false;
											quit = true;
											quitOver = false;
										}
									}
								}

							break;
							case SDL_CONTROLLERAXISMOTION:
							moveCursor(event.caxis);
							break;
						}
					}

					//Update Section
					UpdateBackground();

					//update corsor
					UpdateCursor(deltaTime);

					//check for collision between cursor active state and buttons
					players1Over = SDL_HasIntersection(&activePos, &players1NPos);
					players2Over = SDL_HasIntersection(&activePos, &players2NPos);
					instructionsOver = SDL_HasIntersection(&activePos, &instructNPos);
					quitOver = SDL_HasIntersection(&activePos, &quitNPos);

					//if the curso ris over a button, play the over sound
					if(players1Over || players2Over || instructionsOver || quitOver)
					{
						if(alreadyOver == false)
						{
							Mix_PlayChannel(-1,overSound,0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over any button, reset the already over yar
					if(!players1Over && !players2Over && !instructionsOver && !quitOver)
					{
						alreadyOver = false;
					}
					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw player 1 button
					if(players1Over)
					{
						SDL_RenderCopy(renderer, players1O, NULL, &players1NPos);
					}else{
						SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);
					}


					//Draw player 2 button
					if(players2Over)
					{
						SDL_RenderCopy(renderer, players2O, NULL, &players2NPos);
					}else{
						SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);
					}

					//Draw Instructions button
					if(instructionsOver)
					{
						SDL_RenderCopy(renderer, instructO, NULL, &instructNPos);
					}else{
						SDL_RenderCopy(renderer, instructN, NULL, &instructNPos);
					}

					//Draw Quit button
					if(quitOver)
					{
						SDL_RenderCopy(renderer, quitO, NULL, &quitNPos);
					}else{
						SDL_RenderCopy(renderer, quitN, NULL, &quitNPos);
					}

					//Draw cursor
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					//SDL Render present
					SDL_RenderPresent(renderer);


				}
			break; // end of the main menu case

			case INSTRUCTIONS:
				alreadyOver = false;
				instructions = true;
				cout << "The Game State is Instructions" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << endl;

				while(instructions)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					//check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							menu = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										if(menuOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											instructions = false;
											gameState = MENU;
											menuOver = false;
										}
									}

								}
							break;
							case SDL_CONTROLLERAXISMOTION:
								moveCursor(event.caxis);
								break;
						}
					}


					//Update Section
					UpdateBackground();

					//cursor
					UpdateCursor(deltaTime);

					//check for cursor over menu botton
					menuOver = SDL_HasIntersection(&activePos, &menuNPos);

					//if the curso ris over a button, play the over sound
					if(menuOver)
					{
						if(alreadyOver == false)
						{
							Mix_PlayChannel(-1,overSound,0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over any button, reset the already over yar
					if(!menuOver)
					{
						alreadyOver = false;
					}

					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw instructions Graphic
					SDL_RenderCopy(renderer, instructionsText, NULL, &instructionsTextPos);

					//Draw Main Menu botton
					if(menuOver)
					{
						SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
					}else{
						SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
					}

					//Draw cursor Graphic
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					//SDL Render present
					SDL_RenderPresent(renderer);

				}
			break; // end of the instructions case


			case PLAYERS1:

				enemyList.clear();
				
				//reset the player
				player1.Reset();

				players1 = true;
				alreadyOver = false;

				//Create the enemy pool - 6
				for (int i = 0; i < 6; i++)
				{
					//create the enemy
					Enemy tmpEnemy(renderer, images_dir);

					//add to enemylist
					enemyList.push_back(tmpEnemy);

				}
				while(players1)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					//check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							players1 = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
									{
										players1 = false;
										gameState = WIN;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
									{
										players1 = false;
										gameState = LOSE;
									}

									//send button press info to player1
									if(player1.active)
									{
										player1.OnControllerButton(event.cbutton);
									}
								}
							break;
							case SDL_CONTROLLERAXISMOTION:
								if (player1.active)
								{
									player1.OnControllerAxis(event.caxis);
								}break;
								
						}
					}

					//Update Section
					UpdateBackground();

					//Update Player
					if (player1.active)
					{
						player1.Update(deltaTime, renderer);
					}
					//update enemys
					for(int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Update(deltaTime);
					}

					//only check if the player is active
					if(player1.active == true)
					{

						//use the player 1 bullet list to see if the active bullets hit any of the enemies
						for (int i = 0; i < player1.bulletList.size(); i++)
						{
							//check to see if this bulet is actgive(onscreen)
							if (player1.bulletList[i].active == true)
							{
								for (int j = 0; j < enemyList.size(); j++)
								{
									//check all enemies against between the two objects
									if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
									{
										//play explosion sound
										Mix_PlayChannel(-1, explosionSound, 0);

										MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

										//reset the enemy
										enemyList[j].Reset();

										//reset the enemy
										player1.bulletList[i].Reset();

										////give the player some points
										player1.playerScore += 50;

										//check to see if there is a winning condition
										if (player1.playerScore >= 1000)
										{
											//go to win
											players1 = false;
											gameState = WIN;
										}
									}
								}
							}
						}

						//check to see if the enemy hit the player
						for (int i = 0; i < enemyList.size(); i++)
						{
							//if there is a collision between teh two objects
							if (SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect))
							{
								//play explosion sound
								Mix_PlayChannel(-1, explosionSound, 0);

								MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

								//reset the enemy
								enemyList[i].Reset();

								//give the player some points
								player1.playerLives -= 1;

								//if game over - player lives <= 0
								if (player1.playerLives <= 0)
								{
									players1 = false;
									gameState = LOSE;
									break;
								}
							}
						}
					}

					// player 1 active check end

					//update the pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Update(deltaTime);
						}
					}
					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the enemies
					for(int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Draw(renderer);
					}

					//Draw the title
					//SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);

					//Draw Player 1
					player1.Draw(renderer);

					//update the pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Draw(renderer);
						}
					}

					//SDL Render present
					SDL_RenderPresent(renderer);

				}
			break; // end of the player1 case

			case PLAYERS2:

				//clear out any old enemies
				enemyList.clear();

				//reset player 1 and 2
				player1.Reset();
				player2.Reset();

				alreadyOver = false;
				players2 = true;

				//Create the enemy pool - 6
				for (int i = 0; i < 12; i++)
				{
					//create the enemy
					Enemy tmpEnemy(renderer, images_dir);

					//add to enemylist
					enemyList.push_back(tmpEnemy);

				}

				cout << "The Game State is 2 Player Game" << endl;
				cout << "Press the A Button for Win Screen" << endl;
				cout << "Press the B Button for Lose Screen" << endl;
				cout << endl;

				while(players2)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					//check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							players2 = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0 || event.cdevice.which == 1)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
									{
										players2 = false;
										gameState = WIN;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
									{
										players2 = false;
										gameState = LOSE;
									}
								}

								//send button press info to player1
								if(player1.active)
								{ 
									player1.OnControllerButton(event.cbutton);
								}
								//send button press info to player1
								if(player2.active)
								{ 
									player2.OnControllerButton(event.cbutton);
								}
							break;

							case SDL_CONTROLLERAXISMOTION:
								if(player1.active)
								{
									player1.OnControllerAxis(event.caxis);
								}

								if(player2.active)
								{
									player2.OnControllerAxis(event.caxis);
								}
								break;
						}
					}
					//Update Section
					UpdateBackground();

					//Update Player 1
					if (player1.active)
					{
						player1.Update(deltaTime, renderer);
					}
					//Update Player 2
					if (player2.active)
					{
						player2.Update(deltaTime, renderer);
					}

					//update enemys
					for (int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Update(deltaTime);
					}

					//only check if the player is active
					if (player1.active == true)
					{

						//use the player 1 bullet list to see if the active bullets hit any of the enemies
						for (int i = 0; i < player1.bulletList.size(); i++)
						{
							//check to see if this bulet is actgive(onscreen)
							if (player1.bulletList[i].active == true)
							{
								for (int j = 0; j < enemyList.size(); j++)
								{
									//check all enemies against between the two objects
									if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
									{
										//play explosion sound
										Mix_PlayChannel(-1, explosionSound, 0);

										MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

										//reset the enemy
										enemyList[j].Reset();

										//reset the enemy
										player1.bulletList[i].Reset();

										////give the player some points
										player1.playerScore += 50;

										//check to see if there is a winning condition
										if (player1.playerScore >= 1000)
										{
											//go to win
											players2 = false;
											gameState = WIN;
										}
									}
								}
							}
						}

						//check to see if the enemy hit the player
						for (int i = 0; i < enemyList.size(); i++)
						{
							//if there is a collision between teh two objects
							if (SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect))
							{
								//play explosion sound
								Mix_PlayChannel(-1, explosionSound, 0);

								MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

								//reset the enemy
								enemyList[i].Reset();

								//give the player some points
								player1.playerLives -= 1;

								//if game over - player lives <= 0
								if (player1.playerLives <= 0 && player2.playerLives <= 0)
								{
									players2 = false;
									gameState = LOSE;
									break;
								}
							}
						}
					}

					// player 1 active check end

					// player 2 active check start
					//only check if the player is active
					if (player2.active == true)
					{

						//use the player 1 bullet list to see if the active bullets hit any of the enemies
						for (int i = 0; i < player2.bulletList.size(); i++)
						{
							//check to see if this bulet is actgive(onscreen)
							if (player2.bulletList[i].active == true)
							{
								for (int j = 0; j < enemyList.size(); j++)
								{
									//check all enemies against between the two objects
									if (SDL_HasIntersection(&player2.bulletList[i].posRect, &enemyList[j].posRect))
									{
										//play explosion sound
										Mix_PlayChannel(-1, explosionSound, 0);

										MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

										//reset the enemy
										enemyList[j].Reset();

										//reset the enemy
										player2.bulletList[i].Reset();

										////give the player some points
										player2.playerScore += 50;

										//check to see if there is a winning condition
										if (player2.playerScore >= 1000)
										{
											//go to win
											players2 = false;
											gameState = WIN;
										}
									}
								}
							}
						}

						//check to see if the enemy hit the player
						for (int i = 0; i < enemyList.size(); i++)
						{
							//if there is a collision between teh two objects
							if (SDL_HasIntersection(&player2.posRect, &enemyList[i].posRect))
							{
								//play explosion sound
								Mix_PlayChannel(-1, explosionSound, 0);

								MakeExplosion(player2.posRect.x - 32, player1.posRect.y - 32);

								//reset the enemy
								enemyList[i].Reset();

								//give the player some points
								player2.playerLives -= 1;

								//if game over - player lives <= 0
								if (player1.playerLives <= 0 && player2.playerLives <= 0)
								{
									players2 = false;
									gameState = LOSE;
									break;
								}
							}
						}
					}

					// player 2 active check end

					//update the pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Update(deltaTime);
						}
					}

					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the enemies
					for (int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Draw(renderer);
					}

					//Draw Player 1
					player1.Draw(renderer);

					//Draw Player 2
					player2.Draw(renderer);

					//update the pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Draw(renderer);
						}
					}

					//Draw the title
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);

					//SDL Render present
					SDL_RenderPresent(renderer);
				}
			break; // end of the player2 case

			case WIN:
				alreadyOver = false;
				win = true;
				cout << "The Game State is WIN" << endl;
				cout << "Press the A Button for Main Menu Screen" << endl;
				cout << "Press the B Button to Replay Game" << endl;
				cout << endl;

				while(win)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime) / 1000;
					lastTime = thisTime;

					//check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							win = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										if(menuOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											win = false;
											gameState = MENU;
											menuOver = false;
										}

										if(playOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											win = false;
											gameState = PLAYERS1;
											playOver = false;
										}
									}
								}
							break;
							case SDL_CONTROLLERAXISMOTION:
								moveCursor(event.caxis);
								break;
						}
					}

					//Update Section
					UpdateBackground();

					//Update Section
					UpdateCursor(deltaTime);



					//check for cursor collision
					menuOver = SDL_HasIntersection(&activePos, &menuNPos);
					playOver = SDL_HasIntersection(&activePos, &playNPos);

					//if the curso ris over a button, play the over sound
					if(menuOver || playOver)
					{
						if(alreadyOver == false)
						{
							Mix_PlayChannel(-1,overSound,0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over any button, reset the already over yar
					if(!menuOver && !playOver)
					{
						alreadyOver = false;
					}

					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw win Text
					SDL_RenderCopy(renderer,winText, NULL, &winTextPos);

					//Draw Main Menu botton
					if(menuOver)
					{
						SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
					}else{
						SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
					}

					//Draw Play Again botton
					if(playOver)
					{
						SDL_RenderCopy(renderer, playO, NULL, &playNPos);
					}else{
						SDL_RenderCopy(renderer, playN, NULL, &playNPos);
					}

					//Draw cursor Graphic
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					//SDL Render present
					SDL_RenderPresent(renderer);

				}
			break; // end of the win case

			case LOSE:
				alreadyOver = false;
				lose = true;
				cout << "The Game State is LOSE" << endl;
				cout << "Press the A Button for Main Menu Screen" << endl;
				cout << "Press the B Button to Replay Game" << endl;
				cout << endl;

				while(lose)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime) / 1000;
					lastTime = thisTime;

					//check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							lose = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										if(menuOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											lose = false;
											gameState = MENU;
											menuOver = false;
										}

										if(playOver)
										{
											//play the over sound - plays fine through levels, must pause for QUIT
											Mix_PlayChannel(-1, pressedSound, 0);
											lose = false;
											gameState = PLAYERS1;
											playOver = false;
										}
									}
								}
							break;
							case SDL_CONTROLLERAXISMOTION:
								moveCursor(event.caxis);
								break;
						}
					}

					//Update Section
					UpdateBackground();

					//Update Section
					UpdateCursor(deltaTime);

					//check for cursor collision
					menuOver = SDL_HasIntersection(&activePos, &menuNPos);
					playOver = SDL_HasIntersection(&activePos, &playNPos);

					//if the curso ris over a button, play the over sound
					if(menuOver || playOver)
					{
						if(alreadyOver == false)
						{
							Mix_PlayChannel(-1,overSound,0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over any button, reset the already over yar
					if(!menuOver && !playOver)
					{
						alreadyOver = false;
					}

					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw win Text
					SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);

					//Draw Main Menu botton
					if(menuOver)
					{
						SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
					}else{
						SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
					}

					//Draw Play Again botton
					if(playOver)
					{
						SDL_RenderCopy(renderer, playO, NULL, &playNPos);
					}else{
						SDL_RenderCopy(renderer, playN, NULL, &playNPos);
					}

					//Draw cursor Graphic
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					//SDL Render present
					SDL_RenderPresent(renderer);
				}

			break; // end of the lose case
		}
	}
    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
