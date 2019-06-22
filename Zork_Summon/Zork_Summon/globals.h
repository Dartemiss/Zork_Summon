#ifndef __Globals__
#define __Globals__

#include <string>
#include <vector>

using namespace std;

bool Same(const string& a, const string& b);
bool Same(const char* a, const string& b);
bool Same(const string& a, const char* b);
int Roll(int min, int max);
float elemental_multipliers(string typeAttacker, string typeDefender);
void Tokenize(const string& line, vector<string>& arguments);


#define PLAYING 0
#define GAME_VICTORY 1
#define GAME_DEFEAT 2
#endif //__Globals__
