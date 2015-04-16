#pragma once

#include "settings.h"

class Agent;

/**
* Sugarscape tile class.
* Sugarascape lattice is divided into tiles which are displayed as circles of varying sizes. Each tile is an object of this class.
**/
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
	float pollution;
	
	short int getCapacity();
	void pollutionDiffusion(Tile grid[][GRIDH]);

public:
	
	/**
	* Defaul constructor.
	* @see Tile(int x, int y)
	**/
	Tile();
	/**
	* Tile constructor.
	* This constructor takes in as a parameter two integers for tile's x and y position on the grid.
	**/
	Tile(int x, int y);
	
	/**
	* Growback rule.
	* @param grid A pointer to an array of all tiles
	* @param time Amount of iterations passed since simulation's start.
	* This method should be called every iteration for a tile to grow back.
	* @see seasonalGrow
	**/
	void grow(Tile grid[][GRIDH], int time);
	/**
	* Seasonal Growback rule.
	* @param grid A pointer to an array of all tiles
	* @param time Amount of iterations passed since simulation's start.
	* @param seasonLen Amount of iterations each season should last for.
	* This method should be called every iteration for a tile to grow back.
	* @see grow
	**/
	void seasonalGrow(Tile grid[][GRIDH], int time, int seasonLen=50);
	/**
	* Clears level of sugar and returns it; sets flag taken to true.
	* This method is called when an agent moves to a tile. It sets the taken flag to true so that no other agent will come to this tile. Has no effect on secondary resource.
	* @see eatSpice
	* @see isTaken
	**/
	int eat();
	/**
	* Clears level of spices and returns it.
	* This method is called when an agent moves to a tile. It does not set taken flag. Has no effect on primary resource.
	* @see eat
	**/
	int eatSpice();
	
	int getSugarLvl();
	int getSpiceLvl();
	/**
	* Pollution rule; return sugar to pollution ratio.
	* Returns ratio according to the formulae: sugar/(1+pollution)
	**/
	float getS_Pratio();
	/**
	* Returns tiles x/y coordinates on the lattice as a sf::Vector2i.
	**/
	sf::Vector2f getCoord();
	/**
	* Returns true if an agent has just visited this tile and taken flag is set to true.
	**/
	bool isTaken();
	/**
	* Sets taken flag to false.
	**/
	void freeUp();
	/**
	* Pollution rule; generates pollution from metabolism.
	* @param amount Amount of sugar consumed on this tile this turn.
	* @see genPollutionG
	**/
	void genPollutionM(float amount);
	/**
	* Pollution rule; generates pollution from gathering.
	* @param amount Amount of sugar gathered on this tile this turn.
	* @see genPollutionM
	**/
	void genPollutionG(float amount);
};


#include "agent.h"