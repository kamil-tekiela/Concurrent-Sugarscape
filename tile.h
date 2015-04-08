#pragma once

#include "settings.h"

class Agent;

class Tile : public sf::CircleShape 
{
private:
	int x;
	int y;
	float level;
	short int capacity;
	float levelSpice;
	short int capacitySpice;
	bool taken;
	
	short int getCapacity();
	void pollutionDiffusion(Tile grid[][GRIDH]);

public:
	float pollution;

	Tile();
	Tile(int x, int y);
	
	void grow(Tile grid[][GRIDH]);
	void seasonalGrow(Tile grid[][GRIDH], int time, int seasonLen=50);
	int eat();
	int eatSpice();
	
	int getSugarLvl();
	int getSpiceLvl();
	float getS_Pratio();
	sf::Vector2f getCoord();
	bool isTaken();
	void freeUp();
	void genPollutionM(float amount);
	void genPollutionG(float amount);
};


#include "agent.h"