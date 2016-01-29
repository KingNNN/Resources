
// Example program:
// Using SDL2 to create an application window


#if defined(_WIN32) || (_WIN64)

#include "SDL.h"

#endif

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#endif

#if defined(__linux__)

#include "SDL2/SDL.h"

#endif

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

int main(int argc, char* argv[]) {

//Cout to show we are running on Windows
#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows" << endl;

#endif

//Cout to show that we are running on Linux
#if defined(__linux__)

	cout << "Running on Linux" << endl;
	cout << "Added on Linux" << endl;

#endif

//Cout to show that we are running on Apple
#if defined(__APPLE__)

	cout << "Running on Apple" << endl;

	//string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL,0));

	//create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory +"/Resources/Images/";

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
    SDL_Rect cursorPos, activePos;

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

    int cursorSpeed = 400;

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
    titlePos.x = 330;
    titlePos.y = 120;
    titlePos.w = 390;
    titlePos.h = 45;


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
    players1NPos.x = 400;
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
    players2NPos.x = 390;
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
    instructNPos.x = 385;
    instructNPos.y = 390;
    instructNPos.w = 262;
    instructNPos.h = 33;

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
    quitNPos.x = 460;
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


    //***************** CREATE MAIN END ***********************
    /////////////////////////////////////////////////////////////


	//***** Set up a Game Controller variable *****
	SDL_GameController* gGameController = NULL;

	//***** Open Game Controller *****
	gGameController = SDL_GameControllerOpen(0);

	//*****Turn on Game Controller Events *****
	SDL_GameControllerEventState(SDL_ENABLE);

	//***** SDL Event to handle input *****
	SDL_Event event;

	// Set up variables for the game states
	enum GameState {MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

	// Set up initial state
	GameState gameState = MENU;

	// Boolean values to control movement through the states
	bool menu, instructions, players1, players2, win, lose, quit;

    // The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)
	{
		switch(gameState)
		{
			case MENU:

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
										menu = false;
										gameState = INSTRUCTIONS;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										menu = false;
										gameState = PLAYERS1;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
									{
										menu = false;
										gameState = PLAYERS2;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
									{
										menu = false;
										quit = true;
									}
								}
							break;
						}
					}

					//Update Section
					UpdateBackground();


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
					SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);

					//Draw player 2 button
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);

					//Draw Instructions button
					SDL_RenderCopy(renderer, instructN, NULL, &instructNPos);

					//Draw Quit button
					SDL_RenderCopy(renderer, quitN, NULL, &quitNPos);

					//Draw cursor
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					//SDL Render present
					SDL_RenderPresent(renderer);


				}
			break; // end of the main menu case

			case INSTRUCTIONS:
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
							instructions = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										instructions = false;
										gameState = MENU;
									}

								}
							break;
						}
					}


					//Update Section
					UpdateBackground();

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

					//Draw instructions Graphic
					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);

					//Draw instructions Graphic
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					//SDL Render present
					SDL_RenderPresent(renderer);

				}
			break; // end of the instructions case


			case PLAYERS1:
				players1 = true;
				cout << "The Game State is 1 Player Game" << endl;
				cout << "Press the A Button for Win Screen" << endl;
				cout << "Press the B Button for Lose Screen" << endl;
				cout << endl;

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
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										players1 = false;
										gameState = WIN;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										players1 = false;
										gameState = LOSE;
									}
								}
							break;
						}
					}

					//Update Section
					UpdateBackground();

					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title
					SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);

					//SDL Render present
					SDL_RenderPresent(renderer);

				}
			break; // end of the player1 case

			case PLAYERS2:
				players2 = true;
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

								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										players2 = false;
										gameState = WIN;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										players2 = false;
										gameState = LOSE;
									}
								}
							break;
						}
					}
					//Update Section
					UpdateBackground();

					//Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);

					//SDL Render present
					SDL_RenderPresent(renderer);
				}
			break; // end of the player2 case

			case WIN:
				win = true;
				cout << "The Game State is WIN" << endl;
				cout << "Press the A Button for Main Menu Screen" << endl;
				cout << "Press the B Button to Quit" << endl;
				cout << endl;

				while(win)
				{
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
										win = false;
										gameState = MENU;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										win = false;
										quit = true;
									}
								}
							break;
						}
					}
				}
			break; // end of the win case

			case LOSE:
				lose = true;
				cout << "The Game State is LOSE" << endl;
				cout << "Press the A Button for Main Menu Screen" << endl;
				cout << "Press the B Button to Quit" << endl;
				cout << endl;

				while(lose)
				{
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
										lose = false;
										gameState = MENU;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										lose = false;
										quit = true;
									}
								}
							break;
						}
					}
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




