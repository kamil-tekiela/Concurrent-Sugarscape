#pragma once

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define GRIDW 50
#define GRIDH 50
#define MAXLEVEL 5
#define TILEW 10
#define TILEH 10

class Agent;

class Tile : public sf::CircleShape 
{
private:
	int x;
	int y;
	float level;
	short int capacity;
	bool taken;

public:
	Tile();
	Tile(int x, int y);
	
	void grow();
	void seasonalGrow(int time, int seasonLen=50);
	int eat();
	
	int getSugarLvl();
	sf::Vector2f getCoord();
	bool isTaken();
	void freeUp();
};


#include "agent.h"