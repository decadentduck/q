#include "Assignment1.h"
#include "Body.h"
#include <SDL.h>
#include "GameManager.h"
#include <SDL_image.h>

Assignment1::Assignment1(SDL_Window* sdlWindow_)
{
	window = sdlWindow_;
	elapsedTime = 0.0f;
	gravity = -9.8f;
	frameCount = 0L;
	for (int i =0; i < NUM_BODIES; i++){ bodies[i] = nullptr; }
}

Assignment1::~Assignment1(){ }

bool Assignment1::OnCreate() 
{
	//window height and width
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	//matrix to change orientation of grid (0, 0) is at bottom left
	projectionMatrix = MMath::viewportNDC(w,h) * MMath::orthographic(0.0f, 100.0f, 0.0f, 250.0f, 0.0f, 1.0f);
	//image initialize
	IMG_Init(IMG_INIT_PNG);

	//add 2 bodies to array
	/*bodies[0] = new Body("jetskiSmall.bmp", 2.0f, Vec3(0.0f, 200.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));*/
	bodies[0] = new Body("LasagnaSmall.png", 2.0f, 0.0f, Vec3(0.0f, 200.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	bodies[1] = new Body("GarfieldSmall.bmp", 4.0f, 0.0f, Vec3(0.0f, 200.0f, 0.0f),
		Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < NUM_BODIES; i++) 
	{
		if (bodies[i] == nullptr) { return false; }
	}
	return true;
}

void Assignment1::OnDestroy() 
{
	//delete each object
	for (int i = 0; i < NUM_BODIES; i++) 
	{
		if (bodies[i]) {
			delete bodies[i];
			bodies[i] = nullptr;
		}
	}
	IMG_Quit();
}

void Assignment1::Update(const float time) 
{
	
	elapsedTime += time;
	//add initial force
	if(frameCount == 1) { for(Body* b : bodies) { b->ApplyForce(Vec3(1000.0f, 0.0f, 0.0f)); }}
	//else { for (Body* b : bodies) { b->ApplyForce(Vec3(0.0f, 0.0f, 0.0f)); }}
	//stop moving at bottom
	for (Body* b : bodies) { if (b->pos.y <= 30) b->vel.y = 0.0f; }
	//stop at end of screen
	for (Body* b : bodies) {  if (b->pos.x >= 90) b->vel.x = 0.0f; }
	
	//update each object
	for (int i = 0; i < NUM_BODIES; i++) {
		if (bodies[i]) bodies[i]->Update(time, gravity);
	}
	frameCount++;
}

void Assignment1::Render() 
{

	SDL_Rect imageRectangle;
	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
	//clear the screen
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
	//draw each body on the screen
	for (int i = 0; i < NUM_BODIES; i++) {
		Vec3 screenCoords = projectionMatrix * bodies[i]->pos;

		imageRectangle.h = bodies[i]->getImage()->h;
		imageRectangle.w = bodies[i]->getImage()->w;
		imageRectangle.x = screenCoords.x; /// implicit type conversions BAD - probably causes a compiler warning
		imageRectangle.y = screenCoords.y; /// implicit type conversions BAD - probably causes a compiler warning

		SDL_BlitSurface(bodies[i]->getImage(), nullptr, screenSurface, &imageRectangle);
	}
	SDL_UpdateWindowSurface(window);
}

void Assignment1::HandleEvents(const SDL_Event &event) 
{
	 //Look for a keypress 
	if (event.type == SDL_KEYUP)
	{
		//bodies[0]->ApplyForce(Vec3(-5000.0f, -500.0f, 0.0f));
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			for (Body* b : bodies) { b->ApplyForce(Vec3(-1000.0f, 0.0f, 0.0f)); }
			printf("LEFT\n");
			break;
		case SDLK_RIGHT:
			for (Body* b : bodies){ b->ApplyForce(Vec3(1000.0f, 0.0f, 0.0f)); }
			printf("RIGHT\n");
			break;
		case SDLK_DOWN:
			for (Body* b : bodies) { b->ApplyForce(Vec3(0.0f, -10000.0f, 0.0f)); }
			printf("DOWN\n"); 
			break;
		case SDLK_UP:
			for (Body* b : bodies) { b->ApplyForce(Vec3(0.0f, 10000.0f, 0.0f)); }
			printf("UP\n"); 
			break;
		default:
			printf("Key up\n");
			break;
		}
		
	}
	else
	{
		printf("Unhandled Event!\n");
	}
	
}