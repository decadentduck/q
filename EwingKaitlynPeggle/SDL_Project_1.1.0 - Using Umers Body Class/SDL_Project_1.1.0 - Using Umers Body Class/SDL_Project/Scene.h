#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>

class Scene {
public:
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float time) = 0;
	virtual void Render() = 0;
	virtual void HandleEvents(const SDL_Event &_event) = 0;
};
//dfkjgkdj
#endif
