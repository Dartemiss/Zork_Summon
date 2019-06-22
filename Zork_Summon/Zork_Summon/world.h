#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <time.h>

#define TICK_FREQUENCY 0.5f

using namespace std;

class Entity;
class Player;
class Monster;
class Creature;

class World
{
public:

	World();
	~World();

	bool Tick(vector<string>& args);
	bool ParseCommand(vector<string>& args);
	int checkState();
	void GameLoop();

private:

	clock_t tick_timer;
	list<Entity*> entities;
	Player* player;

	Monster* angelKnight;
	Creature* lauren;
};

#endif //__World__
