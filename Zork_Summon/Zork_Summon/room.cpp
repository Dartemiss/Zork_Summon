#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "monster.h"
#include "room.h"

// ----------------------------------------------------
Room::Room(const char* title, const char* description) :
	Entity(title, description, NULL)
{
	type = ROOM;
}

// ----------------------------------------------------
Room::~Room()
{
}

// ----------------------------------------------------
void Room::Look() const
{
	cout << "\n" << name << "\n";
	cout << description;

	// List exits --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)* it;
			cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see: " << ex->description;
		}
	}

	// List items --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == ITEM)
		{
			Item* item = (Item*)* it;
			cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == ENEMY)
		{
			Creature* cr = (Creature*)* it;
			cout << "\nThere is someone else here: " << cr->name;
			if (cr->IsAlive() == false)
				cout << " (dead)";
		}
	}

	// List monsters --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == MONSTER)
		{
			Monster* cr = (Monster*)* it;
			if(Same(cr->master->name,"hero"))
			{
				cout << "\nYour monster is here: " << cr->name;
			}
			else 
			{
				cout << "\nThere is a monster here: " << cr->name;
			}
			
			if (cr->IsAlive() == false)
				cout << " (dead)";
		}
	}

	cout << "\n";
	cout << endl;
	return;
}

// ----------------------------------------------------
Exit* Room::GetExit(const string& direction) const
{
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)* it;
			if (Same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}


