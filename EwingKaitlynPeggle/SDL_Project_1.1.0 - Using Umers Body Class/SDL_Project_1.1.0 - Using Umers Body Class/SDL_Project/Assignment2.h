#ifndef Assignment2_H
#define Assignment2_H
#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#define NUM_BODIES 2

using namespace MATH;
class Assignment2 : public Scene 
{
private:
	SDL_Window * window;
	Matrix4 projectionMatrix;
	class Body* bodies[NUM_BODIES];
	Body* pluto;
	float gravity;
	float elapsedTime;
	unsigned long frameCount;
public:
	Assignment2(SDL_Window* sdlWindow);
	~Assignment2();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	virtual void HandleEvents(const SDL_Event &event);
};

#endif

