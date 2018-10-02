#include "Assignment2.h"
#include "Body.h"
#include <SDL.h>
#include "GameManager.h"
#include <SDL_image.h>
#include <math.h>

Assignment2::Assignment2(SDL_Window* sdlWindow_)
{
	//constructor method initialize values
	window = sdlWindow_;
	elapsedTime = 0.0f;
	gravity = 0.0f;
	frameCount = 0L;
	for (int i = 0; i < NUM_BODIES; i++) { bodies[i] = nullptr; }
}

Assignment2::~Assignment2() { }

bool Assignment2::OnCreate()
{
	//window height and width
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	//matrix to change orientation of grid (0, 0) is at bottom left
	projectionMatrix = MMath::viewportNDC(w, h) * MMath::orthographic(0.0f, 40.0f, 0.0f, 40.0f, 0.0f, 1.0f);
	//image initialize
	IMG_Init(IMG_INIT_PNG);

	//create a planet to orbit the stars
	pluto = new Body("hadesSmall.png", 1.0f, 0.0f, Vec3(10.0f, 20.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));

	//add 2 bodies to array 
	//Array will be of stars
	bodies[0] = new Body("legoSmall.png", 100.0f, 0.0f, Vec3(5.0f, 10.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	bodies[1] = new Body("girSmall.png", 150.0f, 0.0f, Vec3(15.0f, 25.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < NUM_BODIES; i++) { if (bodies[i] == nullptr) return false; }
	return true;
}

void Assignment2::OnDestroy()
{
	//delete each object
	for (int i = 0; i < NUM_BODIES; i++)
	{
		if (bodies[i]) {
			delete bodies[i];
			bodies[i] = nullptr;
		}
	}
	pluto = nullptr;
	delete pluto;
	elapsedTime = 0.0f;
	frameCount = 0L;
	IMG_Quit();
}

void Assignment2::Update(const float time)
{
	Vec3 force; 
	if (frameCount == 0) { printf("----------START----------\n"); }
	else if(frameCount == 1)
	{
		//determine initial force
		force = Vec3( -130.0f, 300.0f, 0.0f);
		//Apply force on planet
		pluto->ApplyForce(force);
		//update the planet
		pluto->Update(time, gravity);
	}
	else //Math stuff
	{
		//find distance between planet and stars
		float r1 = sqrt((((bodies[0]->pos.x - pluto->pos.x) * (bodies[0]->pos.x - pluto->pos.x)))
			+ ((bodies[0]->pos.y - pluto->pos.y) * (bodies[0]->pos.y - pluto->pos.y)));
		float r2 = sqrt((((bodies[1]->pos.x - pluto->pos.x) * (bodies[1]->pos.x - pluto->pos.x)))
			+ ((bodies[1]->pos.y - pluto->pos.y) * (bodies[1]->pos.y - pluto->pos.y)));
		//find force magnitude
		float magnitude1 = (pluto->mass * bodies[0]->mass) / (r1 * r1);
		float magnitude2 = (pluto->mass * bodies[1]->mass) / (r2 * r2);
		//find force direction
		Vec3 force1 = bodies[0]->pos - pluto->pos;
		Vec3 force2 = bodies[1]->pos - pluto->pos;
		//normalize vector
		force1 /= r1;
		force2 /= r2;
		//multiply vector by magnitude
		force1 *= magnitude1;
		force2 *= magnitude2;
		//combine forces
		force = force1 + force2;
		//Apply force on planet
		pluto->ApplyForce(force);
		//update the planet
		pluto->Update(time, gravity);
	}
	frameCount++;

	//print planet coordinates to the screen 
	printf("%f", pluto->pos.x);
	printf(", ");
	printf("%f", pluto->pos.y);
	printf("    ");
	printf("%f\n", time);
}

void Assignment2::Render()
{
	//I've moved the origin to the right a bit so pluto stays on screen
	SDL_Rect imageRectangle;
	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
	//clear the screen
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
	//draw each body on the screen
	for (int i = 0; i < NUM_BODIES; i++) {
		Vec3 screenCoords = projectionMatrix * bodies[i]->pos;

		imageRectangle.h = bodies[i]->getImage()->h;
		imageRectangle.w = bodies[i]->getImage()->w;
		imageRectangle.x = screenCoords.x + 50; /// implicit type conversions BAD - probably causes a compiler warning
		imageRectangle.y = screenCoords.y - bodies[i]->getImage()->h; /// implicit type conversions BAD - probably causes a compiler warning

		SDL_BlitSurface(bodies[i]->getImage(), nullptr, screenSurface, &imageRectangle);
	}
	Vec3 screenCoords = projectionMatrix * pluto->pos;

	imageRectangle.h = pluto->getImage()->h;
	imageRectangle.w = pluto->getImage()->w;
	imageRectangle.x = screenCoords.x + 50; /// implicit type conversions BAD - probably causes a compiler warning
	imageRectangle.y = screenCoords.y - pluto->getImage()->h; /// implicit type conversions BAD - probably causes a compiler warning

	SDL_BlitSurface(pluto->getImage(), nullptr, screenSurface, &imageRectangle);
	SDL_UpdateWindowSurface(window);
}

void Assignment2::HandleEvents(const SDL_Event &event){}