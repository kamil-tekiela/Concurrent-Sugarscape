#include "tile.h"


Tile::Tile(){
	this->x=0;
	this->y=0;
	this->taken=false;
}

Tile::Tile(int x, int y){
	this->x=x;
	this->y=y;
	this->setPosition(x*TILEW,y*TILEH);
	this->setFillColor(sf::Color::Green);
	//4 is max level of sugar (floored)
	//capacity = rand()% (MAXLEVEL);
	capacity = std::max(0.0,(MAXLEVEL)- std::min( sqrt(pow(x-35,2)+pow(y-15,2)), sqrt(pow(x-15,2)+pow(y-35,2)) )/5.0 );
	//level = 0;
	level = capacity;
	this->setRadius(level);
	this->taken=false;

	int offset = MAXLEVEL-level;
	this->setPosition(x*TILEW+offset,y*TILEH+offset);
}

void Tile::grow(){
	if(level==capacity) return;
	level++;
	this->setRadius(level);
	int offset = MAXLEVEL-level;
	this->setPosition(x*TILEW+offset,y*TILEH+offset);
}

int Tile::eat(){
	taken=true;
	int temp = level;
	level=0;
	return temp;
}

sf::Vector2f Tile::getCoord(){
	return sf::Vector2f(float(x), float(y));
}
int Tile::getSugarLvl(){
	return level;
}
bool Tile::isTaken(){
	return taken;
}
void Tile::freeUp(){
	taken = false;
}
