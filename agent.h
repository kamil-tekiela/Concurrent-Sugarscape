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
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>


#include "tile.h"

#define GRIDW 50
#define GRIDH 50
#define RADIUS 5
#define TILEW 10
#define TILEH 10

class Agent : public sf::CircleShape 
{
private:
	int x;
	int y;
	int sugar;
	short int vision;

public:
	Agent();
	Agent(int x, int y);
	
	void update(Tile[][GRIDH]);
	void move(Tile[][GRIDH]);

	sf::Vector2f getCoord();
};