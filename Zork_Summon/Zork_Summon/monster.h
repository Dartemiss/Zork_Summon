#ifndef __Monster__
#define __Monster__

#include "creature.h"

using namespace std;

class Monster : public Creature
{
public:
	Monster(const char* name, const char* description, Room* room);
	~Monster();

	bool IsAbsorbable();


public:
		string basicSkill;
		string ultimateSkill;
		int basicAttackBonus;
		int ultimateAttackBonus;
		int focusPoints;
		double maxHP;
		string element_type;
		Creature* master;

};

#endif //__Player__