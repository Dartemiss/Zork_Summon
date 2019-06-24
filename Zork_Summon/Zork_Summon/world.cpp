#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"

World::World()
{
	tick_timer = clock();

	//Rooms ---
	Room* glade = new Room("Glade", " You are on a luminous glade in the middle of a wild forest. There are three stones overflowing with magic.");
	Room* lake = new Room("Lake", "You walk into the shore of a huge lake with a small island in the middle. There's someone there.");
	Room* vulcan = new Room("Vulcan", "You are teleported to the top of a vulcan. There's someone waiting above the crater.");
	Room* forest = new Room("Forest", "You are in the middle of a wild forest, where you are surrounded by high trees. You hear someone falling from a tree behind you.");
	Room* town = new Room("Town", "You walk into a ghost town. You see an old man sitting on a rusty chair in the middle of nowhere.");
	Room* abandonedHouse = new Room("Abandoned House", "An old house that contains an overwelming amount of magic.");
	Room* graveyard = new Room("Graveyard", "You feel surrounded by darkness. A hooded person is sitting on a grave with your name.");
	Room* cathedral = new Room("Cathedral", "You enter into an silent cathedral where everything is in calm. There's someone on the altar.");
	//Special room
	Room* ether = new Room("Eter", "The dimension where all creatures that can be summoned live.");

	entities.push_back(glade);
	entities.push_back(lake);
	entities.push_back(vulcan);
	entities.push_back(forest);
	entities.push_back(town);
	entities.push_back(abandonedHouse);
	entities.push_back(graveyard);
	entities.push_back(cathedral);
	entities.push_back(ether);

	//Player ---
	player = new Player("Hero", "A new summoner with mysterious powers.", glade);
	player->health_points = 20;
	player->magicEther = ether;
	entities.push_back(player);

	//Exits ---
	Exit* gladeLake = new Exit("east", "west", "Right magic stone teleport", glade, lake);
	Exit* gladeForest = new Exit("south", "north", "Middle magic stone teleport", glade, forest);
	Exit* gladeVulcan = new Exit("west", "east", "Left magic stone teleport", glade, vulcan);
	Exit* gladeTown = new Exit("north", "south", "Surprisingly convenient royal road", glade, town);
	Exit* townHouse = new Exit("east", "west", "Robust door", town, abandonedHouse);
	townHouse->locked = true;

	Exit* houseGrave = new Exit("down", "up", "An evil portal.", abandonedHouse, graveyard, true);
	houseGrave->locked = true;

	Exit* graveCathedral = new Exit("down", "up", "A celestial portal.", graveyard, cathedral, true);
	graveCathedral->locked = true;

	entities.push_back(gladeLake);
	entities.push_back(gladeForest);
	entities.push_back(gladeVulcan);
	entities.push_back(gladeTown);
	entities.push_back(townHouse);
	entities.push_back(houseGrave);
	entities.push_back(graveCathedral);


	//Creatures ---
	Creature* oldMan = new Creature("Ryan", "It's Ryan the strange old man in town.", town);
	//Enemies ---
	Creature* waterSummoner = new Creature("Rik", "It's Rik, a tall and fit summoner who loves swimming.", lake);
	Creature* fireSummoner = new Creature("Serg", "It's Serg, a crazy and entuthiast summoner.", vulcan);
	Creature* grassSummoner = new Creature("Pabl", "It's Pabl, the explorer who loves nature.", forest);
	Creature* darkSummoner = new Creature("Dart", "It's Dart, the hodded man who sits on your grave.", graveyard);
	lauren = new Creature("Lauren", "It's Lauren the high priest.,", cathedral);
	//Monsters ---
	Monster* redDragon = new Monster("Dragon", "The grown version of the baby dragon, his scales are red and bright.", ether);
	Monster* whiteShark = new Monster("Shark", "The grown version of the baby white shark, his teeth are scary.", ether);
	Monster* kingSnake = new Monster("Snake", "The grown version of the baby snake, his bite is lethal and is known as King of snakes.", ether);
	Monster* elementalMonkey = new Monster("KingKong", "A giant gorila who have been blessed with the power of one element.", ether);
	Monster* blackDragon = new Monster("BlackDragon", "An ancient creature who breaths darkness. His scales are pure black.", ether);

	angelKnight = new Monster("Galiel", "It's angel Galiel, last son of god.", ether);

	oldMan->type = ENEMY;
	waterSummoner->type = ENEMY;
	fireSummoner->type = ENEMY;
	grassSummoner->type = ENEMY;
	darkSummoner->type = ENEMY;
	lauren->type = ENEMY;

	oldMan->dialog = "Combining the three elements is key. All the answers you seek are in my house, but you really think that you are a good summoner? Come here and test your limits. Kong, kill him!";
	waterSummoner->dialog = "Water is balance. Everything you believe in is a lie. Come, swim with me. Shark, devour him!";
	fireSummoner->dialog = "You seemed lost. The answers you seek are in the north. But, are you brave enough? Dragon, burn him!";
	grassSummoner->dialog = "In order to become a master you have to be one with nature! King Snake bite him!";
	darkSummoner->dialog = "You must accept darkness if you want to defeat the real enemy. Accept your death. Black Dragon, teach him!";
	lauren->dialog = "Hi, hero! This is your final test. Are you capable of defeating an angel? Come Galiel";

	redDragon->master = fireSummoner;
	whiteShark->master = waterSummoner;
	kingSnake->master = grassSummoner;
	elementalMonkey->master = oldMan;
	blackDragon->master = darkSummoner;
	angelKnight->master = lauren;

	fireSummoner->myMonster = redDragon;
	waterSummoner->myMonster = whiteShark;
	grassSummoner->myMonster = kingSnake;
	oldMan->myMonster = elementalMonkey;
	darkSummoner->myMonster = blackDragon;
	lauren->myMonster = angelKnight;

	//Initials
	Monster* babyDragon = new Monster("BabyDragon", "A baby dragon.", ether);
	Monster* babyShark = new Monster("BabyShark", "A baby shark.", ether);
	Monster* babySnake = new Monster("BabySnake", "A baby snake.", ether);

	oldMan->health_points = 25;
	waterSummoner->health_points = 25;
	fireSummoner->health_points = 25;
	grassSummoner->health_points = 25;
	darkSummoner->health_points = 25;
	lauren->health_points = 35;

	redDragon->health_points = redDragon->maxHP = 150;
	whiteShark->health_points = whiteShark->maxHP = 150;
	kingSnake->health_points = kingSnake->maxHP = 150;
	elementalMonkey->health_points = elementalMonkey->maxHP = 250;
	blackDragon->health_points = blackDragon->maxHP = 325;
	angelKnight->health_points = angelKnight->maxHP = 400;

	redDragon->ultimateAttackBonus = 80;
	whiteShark->ultimateAttackBonus = 75;
	kingSnake->ultimateAttackBonus = 70;
	elementalMonkey->ultimateAttackBonus = 100;
	blackDragon->ultimateAttackBonus = 110;
	angelKnight->ultimateAttackBonus = 120;


	redDragon->min_damage = 45;
	whiteShark->min_damage = 55;
	kingSnake->min_damage = 50;
	elementalMonkey->min_damage = 50;
	blackDragon->min_damage = 60;
	angelKnight->min_damage = 65;

	redDragon->max_damage = 80;
	whiteShark->max_damage = 90;
	kingSnake->max_damage = 85;
	elementalMonkey->max_damage = 100;
	blackDragon->max_damage = 105;
	angelKnight->max_damage = 110;

	redDragon->min_protection = 0;
	whiteShark->min_protection = 3;
	kingSnake->min_protection = 5;
	elementalMonkey->min_protection = 10;
	blackDragon->min_protection = 15;
	angelKnight->min_protection = 18;

	redDragon->max_protection = 5;
	whiteShark->max_protection = 8;
	kingSnake->max_protection = 10;
	elementalMonkey->max_protection = 15;
	blackDragon->max_protection = 18;
	angelKnight->max_protection = 22;

	redDragon->element_type = "fire";
	whiteShark->element_type = "water";
	kingSnake->element_type = "grass";
	blackDragon->element_type = "darkness";
	angelKnight->element_type = "light";

	entities.push_back(oldMan);
	entities.push_back(waterSummoner);
	entities.push_back(fireSummoner);
	entities.push_back(grassSummoner);
	entities.push_back(darkSummoner);
	entities.push_back(lauren);
	entities.push_back(redDragon);
	entities.push_back(whiteShark);
	entities.push_back(kingSnake);
	entities.push_back(blackDragon);
	entities.push_back(angelKnight);

	//Items ---

	Item* steroids = new Item("Steroids", "A strong drug that can cure a monster.", oldMan, COMMON);

	Item* fireKey = new Item("FireKey", "A red key.", fireSummoner, COMMON);
	Item* waterKey = new Item("WaterKey", "A blue key.", waterSummoner, COMMON);
	Item* grassKey = new Item("GrassKey", "A green key.", grassSummoner, COMMON);
	Item* masterKey = new Item("MasterKey", "A bright golden key.", ether, COMMON);
	Item* darkOrb = new Item("DarkOrb", "An orb that can open a portal.", darkSummoner, COMMON);

	Item* rubbish = new Item("Rubbish", "Just rubbish.", town, COMMON);
	Item* oldKey = new Item("OldKey", "An old key.", rubbish, COMMON);
	townHouse->key = oldKey;
	houseGrave->key = masterKey;
	graveCathedral->key = darkOrb;

	entities.push_back(fireKey);
	entities.push_back(waterKey);
	entities.push_back(grassKey);
	entities.push_back(darkOrb);
	entities.push_back(rubbish);
	entities.push_back(oldKey);
	entities.push_back(steroids);


	//Choose initial monster
	cout << "When you think of your ideal environment behind your partner. You think on: " << endl;
	cout << "A dangerous vulcan. You like danger. [1- To choose Fire]" << endl;
	cout << "A relaxing island near the ocean. You like chilling. [2- To choose Water]" << endl;
	cout << "An enormous forest where someone could get lost easily. You like to explore the unknown. [3- To choose Grass]" << endl;
	string element = "";
	cout << ">";
	while (element != "1" && element != "2" && element != "3")
	{
		cin >> element;
		if (element == "1")
		{
			//Assign initial fire monster to player (Baby dragon)
			babyDragon->ChangeParentTo(glade);
			babyDragon->element_type = "fire";
			babyDragon->health_points = 100;
			babyDragon->maxHP = 100;
			babyDragon->ultimateAttackBonus = 75;
			babyDragon->min_damage = 35;
			babyDragon->max_damage = 55;
			babyDragon->min_protection = 0;
			babyDragon->max_protection = 5;
			babyDragon->master = player;
			entities.push_back(babyDragon);
			player->actual_monster = babyDragon;
			babyShark->master = oldMan;
			babySnake->master = oldMan;
			cout << "You have summoned your first partner: Baby Dragon" << endl;
		}
		else if (element == "2")
		{
			//Assign initial water monster to player (Baby shark)
			babyShark->ChangeParentTo(glade);
			babyShark->element_type = "water";
			babyShark->maxHP = 100;
			babyShark->health_points = 100;
			babyShark->ultimateAttackBonus = 70;
			babyShark->min_damage = 40;
			babyShark->max_damage = 60;
			babyShark->min_protection = 0;
			babyShark->max_protection = 5;
			babyShark->master = player;
			entities.push_back(babyShark);
			player->actual_monster = babyShark;
			babySnake->master = oldMan;
			babyDragon->master = oldMan;
			cout << "You have summoned your first partner: Baby Shark" << endl;
		}
		else if (element == "3")
		{
			//Assign initial grass monster to player (Baby snake)
			babySnake->ChangeParentTo(glade);
			babySnake->element_type = "grass";
			babySnake->health_points = 100;
			babySnake->maxHP = 100;
			babySnake->ultimateAttackBonus = 70;
			babySnake->min_damage = 45;
			babySnake->max_damage = 65;
			babySnake->min_protection = 5;
			babySnake->max_protection = 10;
			babySnake->master = player;
			entities.push_back(babySnake);
			player->actual_monster = babySnake;
			babyShark->master = oldMan;
			babyDragon->master = oldMan;
			cout << "You have summoned your first partner: Baby Snake" << endl;
		}
		else
		{
			cout << "Your command is incorrect. Please choose [1,2,3]" << endl;
			cout << ">";
		}

	}
	elementalMonkey->element_type = player->actual_monster->element_type;
	entities.push_back(elementalMonkey);

	glade->Look();
	cout << ">";
}
// ----------------------------------------------------
World::~World()
{
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete* it;

	entities.clear();
}
// ----------------------------------------------------
bool World::Tick(vector<string>& args)
{
	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if ((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
int World::checkState()
{

	if (lauren->health_points <= 0 && angelKnight->health_points <= 0)
	{
		cout << "########################################################################################" << endl;
		cout << "Congratulations! You have defeated the high priest and stopped the monster annihilation." << endl;
		cout << "You have becomed an actual high summoner." << endl;
		cout << "You completed the game." << endl;

		return 1;
	}
	else if (player->health_points <= 0)
	{
		cout << "########################################################################################" << endl;
		cout << "You died. You and your partners weren't strongest enough." << endl;
		cout << "Try again. Best luck next time." << endl;
		return 2;
	}
	else if (player->numberOfMonsters <= 0)
	{
		cout << "########################################################################################" << endl;
		cout << "All your monsters have been defeated. You are horrible as a summoner." << endl;
		cout << "Try again. Best luck next time." << endl;
		return 2;
	}

	return 0;
}


// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch (args.size())
	{
	case 1: // commands with no arguments ------------------------------
	{
		if (Same(args[0], "look") || Same(args[0], "l"))
		{
			player->Look(args);
		}
		else if (Same(args[0], "north") || Same(args[0], "n"))
		{
			args.push_back("north");
			player->Go(args);
		}
		else if (Same(args[0], "south") || Same(args[0], "s"))
		{
			args.push_back("south");
			player->Go(args);
		}
		else if (Same(args[0], "east") || Same(args[0], "e"))
		{
			args.push_back("east");
			player->Go(args);
		}
		else if (Same(args[0], "west") || Same(args[0], "w"))
		{
			args.push_back("west");
			player->Go(args);
		}
		else if (Same(args[0], "up") || Same(args[0], "u"))
		{
			args.push_back("up");
			player->Go(args);
		}
		else if (Same(args[0], "down") || Same(args[0], "d"))
		{
			args.push_back("down");
			player->Go(args);
		}
		else if (Same(args[0], "stats") || Same(args[0], "st"))
		{
			player->Stats();
		}
		else if (Same(args[0], "inventory") || Same(args[0], "i"))
		{
			player->Inventory();
		}
		else if (Same(args[0], "monsters") || Same(args[0], "m"))
		{
			player->ListMyMonsters();
		}
		else if (Same(args[0], "help") || Same(args[0], "h"))
		{
			player->Help();
		}
		else
			ret = false;
		break;
	}
	case 2: // commands with one argument ------------------------------
	{
		if (Same(args[0], "look") || Same(args[0], "l"))
		{
			player->Look(args);
		}
		else if (Same(args[0], "go"))
		{
			player->Go(args);
		}
		else if (Same(args[0], "take") || Same(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (Same(args[0], "drop") || Same(args[0], "put"))
		{
			player->Drop(args);
		}
		else if (Same(args[0], "examine") || Same(args[0], "ex"))
		{
			player->Examine(args);
		}
		else if (Same(args[0], "attack") || Same(args[0], "at"))
		{
			player->Attack(args);
		}
		else if (Same(args[0], "loot") || Same(args[0], "lt"))
		{
			player->Loot(args);
		}
		else if (Same(args[0], "absorb") || Same(args[0], "abs"))
		{
			player->Absorb(args);
		}
		else if (Same(args[0], "summon") || Same(args[0], "sum"))
		{
			player->Summon(args);
		}
		else if (Same(args[0], "stats") || Same(args[0], "st"))
		{
			player->StatsMonster(args);
		}
		else if (Same(args[0], "talk") || Same(args[0], "tlk"))
		{
			player->Talk(args);
		}
		else
			ret = false;
		break;
	}
	case 3: // commands with two arguments ------------------------------
	{
		if (Same(args[0], "attack") || Same(args[0], "at"))
		{
			player->Attack(args);
		}
		else if (Same(args[0], "take") || Same(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (Same(args[0], "unlock") || Same(args[0], "unlk"))
		{
			player->UnLock(args);
		}
		else if (Same(args[0], "lock") || Same(args[0], "lk"))
		{
			player->Lock(args);
		}
		else if (Same(args[0], "drop") || Same(args[0], "put"))
		{
			player->Drop(args);
		}
		else if (Same(args[0], "use") || Same(args[0], "u"))
		{
			player->Use(args);
		}
		break;
	}
	case 4: // commands with three arguments ------------------------------
	{
		if (Same(args[0], "combine") || Same(args[0], "mix"))
		{
			player->Combine(args);
		}
		else if (Same(args[0], "attack") || Same(args[0], "at"))
		{
			player->UltimateAttack(args);
		}
		else
			ret = false;
		break;
	}
	default:
		ret = false;
	}

	return ret;

}