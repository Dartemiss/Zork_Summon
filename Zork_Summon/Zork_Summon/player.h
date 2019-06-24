#ifndef __Player__
#define __Player__

#include "creature.h"
#include "monster.h"
using namespace std;

class Player : public Creature
{
public:
	Player(const char* name, const char* description, Room* room);
	~Player();

	bool Go(const vector<string>& args);
	void Look(const vector<string>& args) const;
	bool Take(const vector<string>& args);
	bool Drop(const vector<string>& args);
	void Inventory() const;
	bool Examine(const vector<string>& args) const;
	bool Attack(const vector<string>& args);
	bool Loot(const vector<string>& args);
	bool Lock(const vector<string>& args);
	bool UnLock(const vector<string>& args);
	bool Summon(const vector<string>& args);
	bool Absorb(const vector<string>& args);
	bool Use(const vector<string>& args);
	bool Talk(const vector<string>& args);
	bool StatsMonster(const vector<string>& args);
	bool Combine(const vector<string>& args);
	bool UltimateAttack(const vector<string>& args);
	bool EnemyContrattack(Monster* you, Monster* enemy);
	bool ListMyMonsters();
	bool Help();
	void ComputeDamage(Monster* attacker, Creature* deffender);

public:
	Monster* actual_monster;
	Room* magicEther;
	int numberOfMonsters;
};

#endif //__Player__

