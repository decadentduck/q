#include "Assignment3.h"
#include "Body.h"
#include <SDL.h>
#include "GameManager.h"
#include <SDL_image.h>
#include <math.h>
#include "Overlord.h"
#include "VMath.h"

Assignment3::Assignment3(SDL_Window* sdlWindow_)
{
	//constructor method initialize values
	window = sdlWindow_;
	elapsedTime = 0.0f;
	gravity = -9.8f;
	frameCount = 0L;
	canShoot = true;
	ballInPlay = 0;
	playerWon = false;
	gameOver = false;
	//set bodies to be nullptr
	for (int i = 0; i < NUM_PEGS; i++) { pegs[i] = nullptr;	}
}

Assignment3::~Assignment3() { }

bool Assignment3::OnCreate()
{
	//window height and width
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	//matrix to change orientation of grid (0, 0) is at bottom left
	projectionMatrix = MMath::viewportNDC(w, h) * MMath::orthographic(0.0f, 40.0f, 0.0f, 40.0f, 0.0f, 1.0f);
	//image initialize
	IMG_Init(IMG_INIT_PNG);

	char *imageName = "victory.png";
	winImage = IMG_Load(imageName);
	imageName = "defeat.png";
	loseImage = IMG_Load(imageName);

	//create ball
	ball = new Body("gir_round_small.png", 1.5f, 0.8f, Vec3(-10.0f, -10.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	//create array of pegs
	pegs[0] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(15.0, 25.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[1] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(20.0, 25.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[2] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(25.0, 25.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[3] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(12.5, 20.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[4] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(17.5, 20.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[5] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(22.5, 20.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[6] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(27.5, 20.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[7] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(15.0, 15.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[8] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(20.0, 15.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[9] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(25.0, 15.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[10] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(17.5, 10.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[11] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(22.5, 10.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[12] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(5.0, 15.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));

	pegs[13] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(7.0, 10.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[14] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(35.0, 15.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	pegs[15] = new Body("piggysmall.png", 3.0f, 1.5f, Vec3(32.0, 10.0, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
			
	for (int i = 0; i < NUM_PEGS; i++)
	{
		if (pegs[i] == nullptr) { return false; }
	}
	return true;
}

void Assignment3::OnDestroy()
{
	//delete each object
	for (int i = 0; i < NUM_PEGS; i++)
	{
		if (pegs[i]) 
		{
			delete pegs[i];
			pegs[i] = nullptr;
		}
	}
	ball = nullptr;
	delete ball;
	IMG_Quit();
}

void Assignment3::Update(const float time)
{
	//if the game is not over then update
	if (!gameOver)
	{
		/*
		1.	Player starts with 10 balls and can see all the pegs.
		2.	Player takes a shot and watches the ball hit pegs until the ball eventually falls offscreen. During this time the player can't shoot again.
		3.	If all pegs are gone when the ball hits bottom, player wins.
		4.	If no balls remain when the ball hits bottom, player loses.
		5.	Repeat from 2.
		*/

		//if button is down add to the charge
		if (buttonDown) chargeTime += (4 * time);

		//if ball is at the bottom allow the player to fire a new ball
		if (ball->pos.y < 0)
		{
			canShoot = true;
			if (ballInPlay > 9) {  /*Game Over*/ gameOver = true; }
		}
		else { canShoot = false; }

		//peg collision and updates
		float pegCount = 0.0f;
		for (Body *p : pegs)
		{
			if (p->mass != 0.0f)
			{
				pegCount++;
				//see if ball collided with peg				
				if (VMath::distance(ball->pos, p->pos) < (ball->radius + p->radius))
				{
					//calculate collsion
					Overlord::Collision(ball, p);
				}

				//update the peg
				if (p->vel.x != 0.0f || p->vel.y != 0.0f)
				{
					if (p->pos.y < -5.0f)
					{
						p->mass = 0.0f;
					}
					else p->Update(time, gravity);
				}
			}
		}
		//if no more pegs are left the player wins
		if (pegCount == 0.0f) 
		{
			/*Player wins, GAME OVER */ 
			gameOver = true;
			playerWon = true;
		}

		//check if ball collided with a wall
		if (ball->pos.x < 0) //left wall
		{
			ball->pos.x = 0.1;
			ball->vel = Vec3(-(ball->vel.x), ball->vel.y, 0.0f);
		}
		else if (ball->pos.x > 39) //right wall
		{
			ball->pos.x = 38.9;
			ball->vel = Vec3(-(ball->vel.x), ball->vel.y, 0.0f);
		}
		if (ball->pos.y > 39) //top wall
		{
			ball->pos.y = 38.9;
			ball->vel = Vec3(ball->vel.x, -(ball->vel.y), 0.0f);
		}
		//update ball
		if (ball->pos.y > -15) ball->Update(time, gravity);

		//print ball coordinates to the screen 
		printf("%f", ball->pos.x);
		printf(", ");
		printf("%f", ball->pos.y);
		printf("    ");
		printf("%f\n", time);
	}
}

void Assignment3::Render()
{
	SDL_Rect imageRectangle;
	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
	//clear the screen
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 180, 88, 216));
	
	if (!gameOver) //draw objects if the game is not over
	{
		//draw each peg on the screen
		for (int i = 0; i < NUM_PEGS; i++)
		{
			Vec3 screenCoords = projectionMatrix * pegs[i]->pos;

			imageRectangle.h = pegs[i]->getImage()->h;
			imageRectangle.w = pegs[i]->getImage()->w;
			imageRectangle.x = screenCoords.x - pegs[i]->radius; /// implicit type conversions BAD - probably causes a compiler warning
			imageRectangle.y = screenCoords.y - pegs[i]->radius; /// implicit type conversions BAD - probably causes a compiler warning

			SDL_BlitSurface(pegs[i]->getImage(), nullptr, screenSurface, &imageRectangle);
		}
		
		//draw the ball on the screen
		Vec3 screenCoords = projectionMatrix * ball->pos;

		imageRectangle.h = ball->getImage()->h;
		imageRectangle.w = ball->getImage()->w;
		imageRectangle.x = screenCoords.x - ball->radius; /// implicit type conversions BAD - probably causes a compiler warning
		imageRectangle.y = screenCoords.y - ball->radius; /// implicit type conversions BAD - probably causes a compiler warning

		SDL_BlitSurface(ball->getImage(), nullptr, screenSurface, &imageRectangle);

		//TODO draw how many balls are left onto the screen
		//for (int i = 0; i < 3, i++;)
		//{
		//	//draw the ball on the screen
		//	Vec3 screenCoords = projectionMatrix * Vec3(0.0f, 0.0f, 0.0f);

		//	imageRectangle.h = ball->getImage()->h;
		//	imageRectangle.w = ball->getImage()->w;
		//	imageRectangle.x = 0; /// implicit type conversions BAD - probably causes a compiler warning
		//	imageRectangle.y = 0; /// implicit type conversions BAD - probably causes a compiler warning

		//	SDL_BlitSurface(ball->getImage(), nullptr, screenSurface, &imageRectangle);
		//}
	}
	else //game is over 
	{
		//show game over image
		Vec3 screenCoords = projectionMatrix * Vec3(0.0f, 0.0f, 0.0f);
		
		//determine victory or defeat and show apropriate image
		if (playerWon)
		{
			imageRectangle.h = winImage->h;
			imageRectangle.w = winImage->w;
			imageRectangle.x = 0; /// implicit type conversions BAD - probably causes a compiler warning
			imageRectangle.y = 0; /// implicit type conversions BAD - probably causes a compiler warning

			SDL_BlitSurface(winImage, nullptr, screenSurface, &imageRectangle);
		}
		else
		{
			imageRectangle.h = loseImage->h;
			imageRectangle.w = loseImage->w;
			imageRectangle.x = 0; /// implicit type conversions BAD - probably causes a compiler warning
			imageRectangle.y = 0; /// implicit type conversions BAD - probably causes a compiler warning

			SDL_BlitSurface(loseImage, nullptr, screenSurface, &imageRectangle);
		}
	}

	//update the screen
	SDL_UpdateWindowSurface(window);
}

void Assignment3::HandleEvents(const SDL_Event &event) 
{ 
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		printf("Button Down\n");
		buttonDown = true;
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		buttonDown = false;
		if (canShoot)
		{
			printf("Shoot Ball\n");
			//get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//convert mouse position to screen coordinates
			x /= 17.5;		//	screen width (700) / how big I told the screen to be (40)
			y /= 16.25;		//	screen height (650) / how big I told the screen to be (40)

			//put mouse position into a vector
			Vec3 mousePos(x, y, 0);

			//put origin in bottem left corner
			mousePos = projectionMatrix * mousePos;

			//find the ball spawn point / launch point
			Vec3 ballSpawn(20.0f, 40.0f, 0.0f);
			//change relative to origin point
			ballSpawn = projectionMatrix * ballSpawn;

			//set force to be the distance between to spawn point and mouse point
			Vec3 force(mousePos.x - ballSpawn.x, ballSpawn.y - mousePos.y, 0.0f );
			
			//normalize the force vector
			VMath::normalize(force);
			//multiply the force by the chargeTime
			force *= chargeTime;

			//recreate the ball
			ball = new Body("gir_round_small.png", 1.0f, 0.8f, Vec3(20.0f, 35.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
			//apply the force to the ball
			ball->ApplyForce(force);
			//update the ball in play number
			ballInPlay++;
		}
		printf("Button up\n");
		chargeTime = 0.0f;
	}
}