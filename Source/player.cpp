#include "player.h"

//analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//player creation method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	//activate the player
	active = true;

	//set the player number 0 or 1
	playerNum = pNum;

	//set float for player speed
	speed = 500.0f;

	laser = Mix_LoadWAV((audioPath + "laser.wav").c_str());

	//int score and lives variables
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	//init the font system
	TTF_Init();

	//load the font
	font = TTF_OpenFont((audioPath + "zombie.ttf").c_str(),40);

	//see if this is player 1, or player 2, and create the correct x an y locations
	if(playerNum == 0)
	{
		//create the score texture x and y positions
		scorePos.x = 10;
		scorePos.y = 10;
		livesPos.x = 10;
		livesPos.y = 50;
	}else{
		scorePos.x = 650;
		scorePos.y = 10;
		livesPos.x = 650;
		livesPos.y = 50;
	}

	//update score method
	UpdateScore(renderer);

	//update score method
	UpdateLives(renderer);

	//see if this is player 1, or player 2, and create the correct file path
	if(playerNum == 0)
	{
		//create the player 1 texture
		playerPath = filePath + "player.png";
	}else{
		//create the player 2 texture
		playerPath = filePath + "player2.png";
	}

	//loat the surface
	surface = IMG_Load(playerPath.c_str());

	//loat the surface into the texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface for later use
	SDL_FreeSurface(surface);

	//set the SDL_Rect X and Y for the player
	posRect.x = x;
	posRect.y = y;

	//use SDL_QueryTexture to get the W and H of the player's texture
	int w,h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	//Set the movement floats to the players original X and Y
	pos_X = x;
	pos_Y = y;

	//set the xDir and yDir for the joysticks
	xDir = 0;
	yDir = 0;

	//String to create the path to the player's bullet image
	string bulletPath;

	//see if this is plyer 1, or player 2, and create the correct file path
	if(playerNum == 0)
	{
		//create the bullet 1 texture
		bulletPath = filePath + "bullet.png";
	}else{
		//create the bullet 2 texture
		bulletPath = filePath + "bullet2.png";
	}

	//create the player's bullet pool
	for(int i = 0; i < 10; i++)
	{
		//creat teh bullet and move offscree, out of the gameplay area
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000);

		//add to bulletlist
		bulletList.push_back(tmpBullet);
	}
}

void Player::UpdateScore(SDL_Renderer *renderer)
{
	//fix for to_string problems on linux
	string Result;		//string which will contain the result
	ostringstream convert;		//stream used for the conversion
	convert << playerScore;		//inser thte textual representation of "number' in the characters in teh stream
	Result = convert.str();		//set 'Result' to teh contents of the stream

	//create the text for the font texture
	tempScore = "PLAYER SCORE: " + Result;

	//check to see what player this is and color the font as needed
	if(playerNum == 0)
	{
		//place the player 1 score info into a surface
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(),colorP1);

	}else{
		//place the player 1 score info into a surface
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(),colorP2);
	}

	//create the player score texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer,scoreSurface);

	//get the Width and Height of the texture
	SDL_QueryTexture(scoreTexture,NULL,NULL,&scorePos.w,&scorePos.h);

	//free surface
	SDL_FreeSurface(scoreSurface);

	//set the old score
	oldScore = playerScore;

}

//reset the player method
void Player::Reset()
{
	//place the player based on player number
	if (playerNum == 0)
	{
		if (playerNum == 0)
		{
			//set x and y for playier 1
			posRect.x = 250.00;
			posRect.y = 500.00;
		}
		else {
			//set x and y for playier 1
			posRect.x = 550.00;
			posRect.y = 500.00;
		}

		pos_X = posRect.x;
		pos_Y = posRect.y;
		playerLives = 3;
		playerScore = 0;
		xDir = 0;
		yDir = 0;
		active = true;
	}
}

//update lives
void Player::UpdateLives(SDL_Renderer *renderer)
{
	//fix for to_string problems on linux
	string Result;		//string which will contain the result
	ostringstream convert;		//stream used for the conversion
	convert << playerLives;		//inser thte textual representation of "number' in the characters in teh stream
	Result = convert.str();		//set 'Result' to teh contents of the stream

	//create the text for the font texture
	tempLives = "PLAYER LIVES: " + Result;

	//check to see what player this is and color the font as needed
	if(playerNum == 0)
	{
		//place the player 1 score info into a surface
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(),colorP1);

	}else{
		//place the player 1 score info into a surface
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(),colorP2);
	}

	//create the player score texture
	livesTexture = SDL_CreateTextureFromSurface(renderer,livesSurface);

	//get the Width and Height of the texture
	SDL_QueryTexture(livesTexture,NULL,NULL,&livesPos.w,&livesPos.h);

	//free surface
	SDL_FreeSurface(livesSurface);

	//set the old score
	oldLives = playerLives;

	//if player has no mor elives
	if (playerLives <= 0)
	{
		//deactive the player
		active = false;

		//move the player
		posRect.x = posRect.y = -2000;

		//set float values to location values
		pos_X = pos_Y = -2000;

	}

}
//player update method
void Player::Update(float deltaTime, SDL_Renderer *renderer)
{
	//adjust position floats based on speed, direction of joystick axis and deltaTime
	pos_X += (speed *xDir) *deltaTime;
	pos_Y += (speed *yDir) *deltaTime;

	//update player position with code to account for precision loss
	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if(posRect.x < 0)
	{
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w)
	{
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0)
	{
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h)
	{
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	//update the player's bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active)
		{
			//update bullet
			bulletList[i].Update(deltaTime);
		}
	}

	//should score be updated?
	if(playerScore != oldScore)
	{
		UpdateScore(renderer);
	}

	//should lives be updated?
	if(playerLives != oldLives)
	{
		UpdateLives(renderer);
	}

}

//player draw method
void Player::Draw(SDL_Renderer *renderer)
{
	//draw the player texture using the vars textyure and posRect
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	//draw the player's bullet
	for(int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active)
		{
			//draw bullet
			bulletList[i].Draw(renderer);
		}
	}

	//draw the player score
	SDL_RenderCopy(renderer,scoreTexture,NULL,&scorePos);

	//draw the player lives
	SDL_RenderCopy(renderer,livesTexture,NULL,&livesPos);
}


//player destruction method
Player::~Player()
{
	SDL_DestroyTexture(texture);
}

//player joystick button method
void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	//if the player snumber is 0 and the joystick button is from joystick O
	if(event.which == 0 && playerNum == 0)
	{
		//if A button
		if(event.button == 0)
		{
			//create the bullet
			CreateBullet();
		}
	}
	//if the player snumber is 1 and the joystick button is from joystick 1
	if(event.which == 1 && playerNum == 1)
	{
		//if A button
		if(event.button == 0)
		{
			//create the bullet
			CreateBullet();
		}
	}
}

//create the bullet
void Player::CreateBullet()
{
	//see if there is a bullet active to fire
	for(int i = 0; i < bulletList.size(); i++)
	{
		//see if the bullet is not active
		if(bulletList[i].active == false)
		{
			//play the over sound - plays fine through levels, must pause for QUIT
			Mix_PlayChannel(-1, laser, 0);

			//set bulle to active
			bulletList[i].active = true;

			//use some math in the x position to get the bullet close to the center of the plalyer using player width
			bulletList[i].posRect.x = (pos_X + (posRect.w/2));

			//finisighing aligning to the player center using the texture width
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w/2));
			bulletList[i].posRect.y = posRect.y;

			//set the x and y positions of teh bullet's float positions
			bulletList[i].pos_X = pos_X;
			bulletList[i].pos_Y = pos_Y;

			//once bullet is found, break out of the loop
			break;
		}
	}
}

//player joystick axis method
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	//if the player snumber is 0 and the joystick button is from joystick O
	if(event.which == 0 && playerNum == 0)
	{
		//X axis
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; //left
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; //right
			}else{
				xDir = 0.0f; //none
			}
		}

		//Y axis
		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; //down
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; //up
			}else{
				yDir = 0.0f; //none
			}
		}
	}

	//if the player snumber is 0 and the joystick button is from joystick O
	if(event.which == 1 && playerNum == 1)
	{
		//X axis
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; //left
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; //right
			}else{
				xDir = 0.0f; //none
			}
		}

		//Y axis
		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; //down
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; //up
			}else{
				yDir = 0.0f; //none
			}
		}
	}
}


