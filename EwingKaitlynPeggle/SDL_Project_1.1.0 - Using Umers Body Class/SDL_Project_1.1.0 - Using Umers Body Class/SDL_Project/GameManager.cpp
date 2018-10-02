#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "Assignment1.h"
#include "Assignment2.h"
#include "Assignment3.h"
#include <iostream>

GameManager::GameManager() 
{
	timer = nullptr;
	isRunning = true;
	isPaused = false;
	currentScene = nullptr;
}

/// In this OnCreate() method, fuction, subroutine, whatever the word, 
bool GameManager::OnCreate() 
{
	isPaused = false;
	const int SCREEN_WIDTH = 700;
	const int SCREEN_HEIGHT = 650;
	ptr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (ptr == nullptr) 
	{
		OnDestroy();
		return false;
	}

	if (ptr->OnCreate() == false) 
	{
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) 
	{
		OnDestroy();
		return false;
	}
	
	currentScene = new Assignment2(ptr->GetSDL_Window());
	if (currentScene == nullptr)
	{
		OnDestroy();
		return false;
	}
	if (currentScene->OnCreate() == false) 
	{
		OnDestroy();
		return false;
	}
	
	return true;
}

/// Here's the whole game
void GameManager::Run()
{
	timer->Start();
	SDL_Event event;
	
	while (isRunning)
	{
		if (SDL_PollEvent(&event)!=0) 
		{
			if (event.type == SDL_QUIT) { isRunning = false; }//makes close button work
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
			{
				//Use space bar as pause button
				if (isPaused) isPaused = false;
				else if (!isPaused) isPaused = true;
			}
			else
			{
				switch (event.type)
				{
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
					case SDLK_F1: //switch to assignment one when f1 key is pressed / or restart assignment 1
						currentScene->OnDestroy(); //delete current scene
						delete currentScene;
						currentScene = nullptr;
						currentScene = new Assignment1(ptr->GetSDL_Window()); //make new scene
						currentScene->OnCreate(); //build scene
						break;
					case SDLK_F2: //switch to assignment two when f2 key is pressed / or restart assignment 2
						currentScene->OnDestroy();//delete current scene
						delete currentScene;
						currentScene = nullptr;
						currentScene = new Assignment2(ptr->GetSDL_Window()); //make new scene
						currentScene->OnCreate();//build scene
						break;
					case SDLK_F3: //switch to assignment 3 when f13 key is pressed / or restart assignment 3
						currentScene->OnDestroy(); //delete current scene
						delete currentScene;
						currentScene = nullptr;
						currentScene = new Assignment3(ptr->GetSDL_Window()); //make new scene
						currentScene->OnCreate(); //build scene
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
				//handle any assignment specific events
				currentScene->HandleEvents(event); 
			}
		}
		timer->UpdateFrameTicks();
		if (!isPaused) //not paused
		{
			currentScene->Update(timer->GetDeltaTime());	//update
			currentScene->Render();							//render

			/// Keeeep the event loop running at a proper rate
			SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
		}
	}
}

GameManager::~GameManager() {}

void GameManager::OnDestroy()
{
	if (ptr) delete ptr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}