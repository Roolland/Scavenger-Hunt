#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "TransformComponent.h"
#include <cstring>

//10:02
Map* map;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,1600,900 };



bool Game::isRunning = false; 
bool arr[3] = { 0,0,0 };
bool ok = 0;
bool okq = 0;
auto& Player(manager.addEntity());
bool answer;
bool startq = 0;



Game::Game()
{}
Game :: ~Game()
{}

void Game :: init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!.." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, heigh,flags);
		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
	
	map = new Map("Assets/terrain_ss3.png",3,32);
	
	//char path[200];
	//strcpy_s(path, sizeof("Assets/p16x16.map"), "Assets/p16x16.map");
	map->LoadMap("Assets/map.map", 25, 20);
	//strcpy_s(path, sizeof("Assets/p16x16.map"), "Assets/p16x16.map")

	Player.addComponent<TransformComponent>(2);
	Player.addComponent<SpriteComponent>("Assets/finalAnimation.png", true);
	//Player.addComponent<SpriteComponent>("Assets/playerAnimation2.png", true);
	Player.addComponent<KeyboardController>();
	Player.addComponent<ColliderComponent>("Player");
	Player.addGroup(groupPlayers);


}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& Colliders(manager.getGroup(Game::groupColliders));

void Game  :: handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	}

}

void Game :: update()
{ 
	SDL_Rect playerCol = Player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = Player.getComponent<TransformComponent>().position;
	Vector2D Qpos;
	manager.refresh();
	manager.update();

	for (auto& c : Colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			Player.getComponent<TransformComponent>().position = playerPos;
			Qpos = playerPos;
		}

		if (Qpos.x >= 1570 && Qpos.x <= 1726 && Qpos.y >= 706 && Qpos.y <= 862 && okq == 0)
		{
			std::cout << "To get to the final quiz, you need to collect the three hidden books on the map!" << std::endl;
			ok = 1;
			okq = 1;
		}

		if (Qpos.x >= 1570 && Qpos.x <= 1681 && Qpos.y >= 1282 && Qpos.y <= 1372 && arr[0] == 0 && ok == 1)
		{
			std::cout << "PMI (Project Management Institute) has defined these five process groups,or phases, which come together to form the project life cycle. Project initiation, project planning, project execution ,project monitoring and control Closing the Project (Book 1 found)" << std::endl;
			arr[0] = 1;
		}

		if (Qpos.x >= 2050 && Qpos.x <= 2206 && Qpos.y >= 1090 && Qpos.y <= 1246 && arr[1] == 0 && ok == 1)
		{
			std::cout << "The success of a project can be highlighted using the constraint between 5 dimensions (purpose, time, quality, cost, risk). Changing any one of them causes the others to change. (Book 2 found)" << std::endl;
			arr[1] = 1;
		}
		if (Qpos.x >= 1954 && Qpos.x <= 2110 && Qpos.y >= 322 && Qpos.y <= 478 && arr[2] == 0 && ok == 1)
		{
			std::cout << " Regardless of why a project is created, it is important that the purpose of the project to be definedand established correctly from the beginning.  The creation of the activity plan should be one of the first planning steps of process, once they were written the specifications required for the project. (Book 3 found)" << std::endl;
			arr[2] = 1;
		}

		if (arr[0] == 1 && arr[1] == 1 && arr[2] == 1 && startq == 0)
		{
			std::cout <<std::endl<< "You can take the quiz!! Go back to the quiz zone!" << std::endl<<std::endl;
			arr[0] == 0;
			startq = 1;
		}

		if (startq == 1 && Qpos.x >= 1570 && Qpos.x <= 1726 && Qpos.y >= 706 && Qpos.y <= 862)
		{
			int answ2;
			char answ1[25], answ3[25];
			std::cout << "Is the Project execution one of the five process groups?" << std::endl;
			std::cin >> answ1;
			std::cout << "How many dimensions are there which can affect the succes of a project? " << std::endl;
			std::cin >> answ2;
			std::cout << "Is abstractization one of the four pillars of Object Oriented Programming?" << std::endl;
			std::cin >> answ3;
			if (strcmp(answ1, "Yes") == NULL && answ2 == 5 && strcmp(answ3, "Yes") == NULL)
			{

				std::cout << "Congratulations! You answered correctly !" << std::endl;

				
				Game::isRunning = false;
			}
			else
			{
				std::cout << "That's a shame! You answered wrong to one of the questions :(" << std::endl;
				Game::isRunning = false;
			}
		}

	}

	camera.x = Player.getComponent<TransformComponent>().position.x - 400;
	camera.y = Player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}



void Game::render()
{
	SDL_RenderClear(renderer);
	//manager.draw();
	for (auto& t : tiles)
	{
		t->draw();
	}


	for (auto& p : players)
	{
		p->draw();
	}


	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned" << std::endl;
}
