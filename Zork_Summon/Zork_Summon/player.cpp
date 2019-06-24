#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"
#include "monster.h"
// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
	Creature(title, description, room)
{
	type = PLAYER;
	actual_monster = NULL;
	numberOfMonsters = 1;
	magicEther = NULL;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const vector<string>& args) const
{
	if (args.size() > 1)
	{
		for (list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if (Same((*it)->name, args[1]) || ((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->GetNameFrom((Room*)parent))))
			{
				(*it)->Look();
				return;
			}
		}

		if (Same(args[1], "me"))
		{
			cout << "\n" << name << "\n";
			cout << description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const vector<string>& args)
{
	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked)
	{
		cout << "\nThat exit is locked.\n";
		return false;
	}

	cout << "\nYou take direction " << exit->GetNameFrom((Room*)parent) << "...\n";
	actual_monster->ChangeParentTo(exit->GetDestinationFrom((Room*)parent));
	ChangeParentTo(exit->GetDestinationFrom((Room*)parent));
	parent->Look();

	return true;
}


// ----------------------------------------------------
bool Player::Take(const vector<string>& args)
{
	if (args.size() == 3)
	{
		Item* item = (Item*)parent->Find(args[2], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == NULL)
			item = (Item*)Find(args[2], ITEM);

		if (item == NULL)
		{
			cout << "\nCannot find '" << args[2] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if (args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nThere is no item here with that name.\n";
			return false;
		}

		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		cout << "\nYou do not own any item.\n";
		return;
	}

	for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if (*it == weapon)
			cout << "\n" << (*it)->name << " (as weapon)";
		else if (*it == armour)
			cout << "\n" << (*it)->name << " (as armour)";
		else
			cout << "\n" << (*it)->name;
	}

	cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const vector<string>& args)
{
	if (args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if (container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);

		return true;
	}

	return false;
}


// ----------------------------------------------------
bool Player::Examine(const vector<string>& args) const
{
	Creature* target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}
	cout << endl;
	target->Inventory();
	target->Stats();
	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Attack(const vector<string>& args)
{
	Creature* target = (Creature*)parent->Find(args[1], CREATURE);
	if (target == NULL)
	{
		cout << "\n" << args[1] << " is not here.";
		return false;
	}
	if (target->health_points <= 0)
	{
		cout << "\nEnemy is already dead." << endl;
		cout << endl;
		return false;
	}
	if (Same(target->name, actual_monster->name))
	{
		cout << "\nYou cannot attack your own monsters dumbass." << endl;
		return false;
	}



	if (target->type == ENEMY && target->myMonster->health_points >= 0)
	{
		cout << "\nThere's a giant monster defending the master. You must defeat the monster first." << endl;
		cout << endl;
		return false;
	}
	//If monster have to attack
	if (args.size() == 3)
	{
		if (actual_monster->health_points <= 0)
		{
			cout << "\nYour actual monster is dead." << endl;
			return false;
		}
		if (!Same(args[2], actual_monster->name))
		{
			cout << "\nThis monster does not exist on the field." << endl;
			cout << endl;
			return false;
		}
		actual_monster->combat_target = target;
		cout << "\n" << actual_monster->name << " attacks " << target->name << endl;

		if (actual_monster->focusPoints < 3)
			actual_monster->focusPoints += 1;

		ComputeDamage(actual_monster, target);
		cout << endl;
		bool contra = EnemyContrattack(actual_monster, (Monster*)target);
	}
	else
	{
		if (target->type == MONSTER)
		{
			cout << "\nYou cannot attack a monster yourself." << endl;
			cout << endl;
			return false;
		}
		combat_target = target;
		cout << "\nYou jump to attack " << target->name << "!\n";
	}


	return true;
}
// ----------------------------------------------------
bool Player::EnemyContrattack(Monster* you, Monster* enemy)
{
	if (you == NULL || enemy == NULL)
	{
		return false;
	}

	if (enemy->health_points <= 0)
	{
		return false;
	}
	float multiplier;
	multiplier = elemental_multipliers(enemy->element_type, you->element_type);
	string mult = (multiplier == 2.0f) ? "It's very effective." : "It's not very effective.";
	if (multiplier == 1.0f)
		mult = "";
	double attack = (enemy->focusPoints == 3) ? (enemy->ultimateAttackBonus) : Roll(you->min_damage, you->max_damage);
	double defense = Roll(you->min_protection, you->max_protection);
	double total = abs(attack - defense) * multiplier;

	you->health_points -= total;
	cout << "\nEnemy " << enemy->name << " attacks " << you->name << " dealing " << total << " with " << ((enemy->focusPoints == 3) ? "his ultimate. " : "a basic attack.") << mult << endl;
	cout << "[Attack: " << total << " , Defense: " << defense << " , Multiplier: " << multiplier << ".]" << endl;
	enemy->focusPoints += ((enemy->focusPoints == 3) ? -3 : 1);
	cout << "My Health points: " << you->health_points << endl;
	cout << "Enemy focus points: " << enemy->focusPoints << endl;


	if (you->health_points <= 0)
	{
		cout << you->name << " died." << endl;
		numberOfMonsters -= 1;
	}

	cout << endl;
	return true;
}
// ----------------------------------------------------
bool Player::UltimateAttack(const vector<string>& args)
{
	if (!Same(args[3], "ult") && !Same(args[3], "ultimate"))
	{
		cout << "\nIncorrect command, did you wanted to use 'ult'/'ultimate'." << endl;
		return false;
	}
	if (actual_monster->focusPoints != 3)
	{
		cout << "\nYou need 3 focus points in order to use the Ultimate." << endl;
		cout << endl;
		return false;
	}
	Creature* target = (Creature*)parent->Find(args[1], CREATURE);
	if (target == NULL)
	{
		cout << "\n" << args[1] << " is not here.";
		return false;
	}
	if (target->health_points <= 0)
	{
		cout << "\nEnemy is already dead." << endl;
		cout << endl;
		return false;
	}
	if (Same(target->name, actual_monster->name))
	{
		cout << "\nYou cannot attack your own monsters dumbass." << endl;
		return false;
	}



	if (target->type == ENEMY && target->myMonster->health_points >= 0)
	{
		cout << "\nThere's a giant monster defending the master. You must defeat the monster first." << endl;
		cout << endl;
		return false;
	}

	if (!Same(args[2], actual_monster->name))
	{
		cout << "\nThis monster does not exist on the field." << endl;
		cout << endl;
		return false;
	}

	//Use ultimate

	double multiplier;

	if (target->type == ENEMY)
	{
		multiplier = elemental_multipliers(actual_monster->element_type, "normal");
	}
	else
	{
		Monster* monster = (Monster*)target;
		multiplier = elemental_multipliers(actual_monster->element_type, monster->element_type);
	}

	double defense = Roll(target->min_protection, target->max_protection);

	double total = abs(actual_monster->ultimateAttackBonus - defense) * multiplier;

	string mult = (multiplier == 2.0f) ? "It's very effective." : "It's not very effective.";
	if (multiplier == 1.0f)
		mult = "";
	cout << "\n" << actual_monster->name << " uses his ultimate delivering an enormous amount of power to " << target->name << " dealing: " << total << " health points. " << mult << endl;
	cout << "[Ultimate attack: " << actual_monster->ultimateAttackBonus << " , Defense: " << defense << " , Multiplier: " << multiplier << ".]" << endl;
	cout << "Enemy Health points: " << target->health_points << endl;
	actual_monster->focusPoints = 0;
	cout << "Focus points: " << actual_monster->focusPoints << endl;
	cout << endl;

	if (target->health_points <= 0)
	{
		cout << target->name << " died." << endl;
	}
	bool contra = EnemyContrattack(actual_monster, (Monster*)target);
	cout << endl;
	return true;
}


// ----------------------------------------------------
void Player::ComputeDamage(Monster* attacker, Creature* deffender)
{
	double multiplier;

	if (deffender->type == ENEMY)
	{
		multiplier = elemental_multipliers(attacker->element_type, "normal");
	}
	else
	{
		Monster* monster = (Monster*)deffender;
		multiplier = elemental_multipliers(attacker->element_type, monster->element_type);
	}

	double damage = Roll(attacker->min_damage, attacker->max_damage);
	double defense = Roll(deffender->min_protection, deffender->max_protection);

	double total = abs(damage - defense) * multiplier;
	string mult = (multiplier == 2.0f) ? "It's very effective." : "It's not very effective.";
	if (multiplier == 1.0f)
		mult = "";
	deffender->health_points -= total;
	cout << attacker->name << " attacks " << deffender->name << " dealing: " << total << " health points. " << mult << endl;
	cout << "[Attack: " << damage << " , Defense: " << defense << " , Multiplier: " << multiplier << ".]" << endl;
	cout << "Enemy Health points: " << deffender->health_points << endl;
	cout << "Focus points: " << actual_monster->focusPoints << endl;
	if (deffender->health_points <= 0)
	{
		cout << deffender->name << " died." << endl;
	}
}

// ----------------------------------------------------
bool Player::Loot(const vector<string>& args)
{
	Creature* target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if (target->IsAlive() == true)
	{
		cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	list<Entity*> items;
	target->FindAll(ITEM, items);

	if (items.size() > 0)
	{
		cout << "\nYou loot " << target->name << "'s corpse:\n";

		for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
		{
			Item* i = (Item*)(*it);
			cout << "You find: " << i->name << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";

	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Lock(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == true)
	{
		cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[2], ITEM);

	if (item == NULL)
	{
		cout << "\nItem '" << args[2] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;
	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == false)
	{
		cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[2], ITEM);

	if (item == NULL)
	{
		cout << "\nKey '" << args[2] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item)
	{
		cout << "\nKey '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;
	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Summon(const vector<string>& args)
{
	if (!IsAlive())
	{
		return false;
	}

	Monster* monster = (Monster*)magicEther->Find(args[1], MONSTER);

	if (monster == NULL)
	{
		cout << "\nMonster: '" << args[1] << "' is not in under your control.\n";
		return false;
	}
	cout << "\n" << actual_monster->name << " is transported to your magic container." << endl;
	actual_monster->ChangeParentTo(magicEther);
	monster->ChangeParentTo(parent);
	actual_monster = monster;
	cout << "\nYou have summoned: " << actual_monster->name << " to help you.";
	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Absorb(const vector<string>& args)
{
	if (!IsAlive())
	{
		return false;
	}

	Monster* m = (Monster*)parent->Find(args[1], MONSTER);

	if (m == NULL)
	{
		cout << "This monster doesn't exist." << endl;
		return false;
	}

	if (!m->IsAbsorbable())
	{
		return false;
	}

	m->ChangeParentTo(magicEther);
	m->master = this;
	m->health_points = m->maxHP;
	cout << "\nYou raise your hand with your mark glowing. A lightning falls from the sky making dissapear the monster." << endl;
	cout << "You have absorved : " << m->name;
	cout << endl;

	numberOfMonsters += 1;

	return true;
}

// ----------------------------------------------------
bool Player::Use(const vector<string>& args)
{
	if (!IsAlive())
	{
		return false;
	}

	Item* item = (Item*)Find(args[1], ITEM);
	if (item == NULL)
	{
		cout << "\nYou don't have this item." << endl;
		return false;
	}

	if (Same(args[1], "steroids"))
	{
		Monster* monster = (Monster*)parent->Find(args[2], CREATURE);
		if (monster == NULL)
		{
			cout << "\nThis monster doesn't exist or isn't in the field." << endl;
			return false;
		}
		if (monster->health_points <= 0)
		{
			numberOfMonsters += 1;
		}
		monster->health_points = monster->maxHP;
		monster->focusPoints = 3;
		item->ChangeParentTo(magicEther);
	}
	else
	{
		cout << "\nYou can't use this item." << endl;
		return false;
	}

	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Talk(const vector<string>& args)
{
	Creature* creature = (Creature*)parent->Find(args[1], ENEMY);
	if (creature == NULL)
	{
		cout << "\nThe creature does not exist." << endl;
		return false;
	}

	cout << "\n" << creature->dialog << endl;
	if (creature->type == ENEMY && !creature->haveTalked)
	{
		Monster* monster = (Monster*)creature->myMonster;
		monster->ChangeParentTo(parent);
		cout << creature->name << " have summoned: " << monster->name << endl;
		creature->haveTalked = true;
	}
	cout << endl;
	return true;
}


// ----------------------------------------------------
bool Player::StatsMonster(const vector<string>& args)
{
	Monster* m = (Monster*)parent->Find(args[1], MONSTER);

	if (m == NULL)
	{
		cout << "\nThe monster does not exist." << endl;
		return false;
	}

	cout << "\nMonster: " << m->name << endl;
	cout << "Health points: " << m->health_points << endl;
	cout << "Type: " << m->element_type << endl;
	cout << "Ultimate: " << m->basicSkill << " -- bonus = " << m->basicAttackBonus << endl;
	cout << "Ultimate: " << m->ultimateSkill << " -- bonus = " << m->ultimateAttackBonus << endl;
	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Combine(const vector<string>& args)
{
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;

	ok1 = (Same(args[1], "firekey") || Same(args[2], "firekey") || Same(args[3], "firekey"));
	ok2 = (Same(args[1], "waterkey") || Same(args[2], "waterkey") || Same(args[3], "waterkey"));
	ok3 = (Same(args[1], "grasskey") || Same(args[2], "grasskey") || Same(args[3], "grasskey"));

	if (!ok1 || !ok2 || !ok3)
	{
		cout << "\nYou can't combine these ingredients." << endl;
		return false;
	}

	Item* fire = (Item*)Find("firekey", ITEM);
	Item* water = (Item*)Find("waterkey", ITEM);
	Item* grass = (Item*)Find("grasskey", ITEM);

	fire->ChangeParentTo(magicEther);
	water->ChangeParentTo(magicEther);
	grass->ChangeParentTo(magicEther);

	Item* golden = (Item*)magicEther->Find("masterkey", ITEM);
	golden->ChangeParentTo(this);

	cout << "\nYou focus on the energy of the keys." << endl;
	cout << "\nCongratulations! You obtained: MasterKey" << endl;
	cout << endl;
	return true;

}

// ----------------------------------------------------
bool Player::ListMyMonsters()
{
	cout << "\nList of monsters under your control: " << endl;
	list<Entity*> monsters;
	magicEther->FindAll(MONSTER, monsters);
	if (monsters.size() == 0)
	{
		cout << "You have no monsters." << endl;
		return false;
	}
	for (list<Entity*>::const_iterator it = monsters.begin(); it != monsters.cend(); ++it)
	{
		Monster* aux = (Monster*)(*it);
		if (Same(aux->master->name, name))
		{
			cout << aux->name;
			string alive = (aux->health_points <= 0) ? ("(dead)") : ("(alive)");
			cout << alive << endl;
		}
	}

	if (actual_monster != NULL)
	{
		cout << actual_monster->name;
		string alive = (actual_monster->health_points <= 0) ? ("(dead)") : ("(alive)");
		cout << alive << endl;
	}
	cout << endl;
	return true;
}

// ----------------------------------------------------
bool Player::Help()
{
	cout << "\n-----------------------------------------------------" << endl;
	cout << "The objective of the game is to defeat the high priest in the last room." << endl;
	cout << "If all your monsters or you die the game ends." << endl;
	cout << "There are 5 elemental types: fire, water, grass, darkness and light." << endl;
	cout << "(Fire > Grass > Water > Fire) -- darkness and light are supereffective always." << endl;
	cout << "All commands aren't case sensitive, capital letters don't matter" << endl;
	cout << "These are all the available commands: " << endl;
	cout << endl;
	cout << "look/lk (X): You look at your room by default. Or with one argument look at something." << endl;
	cout << "take/pick (X) (Y): Take an object X, or take an object Y that is contained in X." << endl;
	cout << "drop/put (X) (Y): Drop an object X, or drop an object Y that is contained in X." << endl;
	cout << "attack/at (X) (Y) (Z): Attack enemy X with monster Y, (Z) = (ult/ultimate) for using ultimate." << endl;
	cout << "******* IMPORTANT TIP: if you press '-' you can use the last command used, it is very useful for combat" << endl;
	cout << "go (X): Move to X direction." << endl;
	cout << "north/south/east/west: Move to the desired direction." << endl;
	cout << "loot/lt (X): Take all objects from dead body X." << endl;
	cout << "absorb/abs (X): Absorb dead monster X and add it to your collection." << endl;
	cout << "summon/sum (X): Summon monster X from your collection to the field." << endl;
	cout << "lock/lk (X) (Y): Lock direction X using object Y." << endl;
	cout << "unlock/unlk (X) (Y): Unlock direction X using object Y." << endl;
	cout << "stats/st (X): Look at your stats without arguments or at monster X stats." << endl;
	cout << "inventory/i: Look the items that you own." << endl;
	cout << "use/u (X) (Y): Use steroids = X in monster Y." << endl;
	cout << "talk/tlk (X): Talk to NPC X." << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << endl;
	return true;
}
