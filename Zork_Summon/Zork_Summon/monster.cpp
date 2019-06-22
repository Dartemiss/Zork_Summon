#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"
#include "monster.h"
// ----------------------------------------------------
Monster::Monster(const char* title, const char* description, Room* room) :
	Creature(title, description, room)
{
	type = MONSTER;
	basicAttackBonus = ultimateAttackBonus =  0;
	maxHP = 0.0;
	master = NULL;
	focusPoints = 0;
}

// ----------------------------------------------------
Monster::~Monster()
{
}
// ----------------------------------------------------
bool Monster::IsAbsorbable()
{
	if(health_points > 0)
	{
		cout << "\nYou only can absorb defeated monsters." << endl;
		return false;
	}

	if(master->IsAlive())
	{
		cout << "\nYou must kill the former master in order to absorb a monster." << endl;
		return false;
	}
	cout << endl;
	return true;
}
