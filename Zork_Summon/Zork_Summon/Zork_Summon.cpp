#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"
#include "player.h"

using namespace std;

#define BACKSPACE "\033[D\033[K"

#define WHITE_ "\033[1;36m"
#define _WHITE "\033[0m"

// -------------------

int main()
{
	//*********INTRO*********
	cout << "Welcome to Zork Summoners!" << endl << endl;
	cout << "You just woke up in the middle of a forest." << endl;
	cout << "The first thing that you remember is the ritual." << endl;
	cout << "You feel pain in your right hand and open it." << endl;
	cout << "You look at the mark with spiral form on the palm of your hand." << endl;
	cout << "Then you feel a overwhelming power." << endl;
	cout << "It is time to summon you little one." << endl;
	//********INTRO**********

	World myWorld = World();
	
	string playerInput;
	char key;
	bool looping = true;
	vector<string> args;
	int gameState = PLAYING;
	string lastInput = "";

	while (looping)
	{
		gameState = myWorld.checkState();
		switch (gameState)
		{
		case PLAYING:
			if (_kbhit())
			{
				key = _getch();
				if (key == '\b') // backspace
				{
					if (playerInput.length() > 0)
					{
						playerInput.pop_back();
						cout << '\b';
						cout << " ";
						cout << '\b';
					}
				}
				else if (key == '-') {
					args.clear();
					playerInput = lastInput;
					cout << playerInput;
				}
				else if (key != '\r') // Every key except return or backspace
				{
					playerInput += key;
					cout << key;
				}
				else // return creates args
					Tokenize(playerInput, args);
			}
			//Quit
			if (args.size() > 0 && Same(args[0], "quit"))
			{
				looping = false;
			}
			// Evaluate commands
			if (myWorld.Tick(args) == false)
			{
				cout << "\nSorry, I do not understand your command.\n";
			}
			if (args.size() > 0)
			{
				lastInput = playerInput;
				args.clear();
				playerInput = "";
					cout << ">";
			}
			break;
		case GAME_VICTORY:
			looping = false;
			break;
		case GAME_DEFEAT:
			looping = false;
			break;
		}
	}
	cout << "Thanks for playing!" << endl;
	system("pause");
	return 0;
	
}