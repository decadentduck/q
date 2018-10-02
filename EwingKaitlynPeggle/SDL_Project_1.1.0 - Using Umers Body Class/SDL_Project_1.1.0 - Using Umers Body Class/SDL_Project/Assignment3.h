#ifndef Assignment3_H
#define Assignment3_H
#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#define NUM_PEGS 16
class Body;
using namespace MATH;
class Assignment3 : public Scene
{
private:
	SDL_Window * window;
	Matrix4 projectionMatrix;
	Body* ball;
	Body* pegs[NUM_PEGS];
	float gravity;
	float elapsedTime;
	unsigned long frameCount;
	bool canShoot; 
	bool buttonDown;
	float ballInPlay;
	float chargeTime;
	Vec3 mousePos;
	bool gameOver;
	bool playerWon;
	SDL_Surface* winImage;
	SDL_Surface* loseImage;
public:
	Assignment3(SDL_Window* sdlWindow);
	~Assignment3();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	virtual void HandleEvents(const SDL_Event &event);
};

#endif

