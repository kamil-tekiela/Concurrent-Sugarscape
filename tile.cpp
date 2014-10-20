#include "tile.h"


Tile::Tile(){
	this->x=0;
	this->y=0;
	this->taken=false;
	//this->whoIs=Agent();
}

Tile::Tile(int x, int y){
	this->x=x;
	this->y=y;
	this->setPosition(x*TILEW,y*TILEH);
	this->setFillColor(sf::Color::Green);
	//4 is max level of sugar (floored)
	this->capacity = std::max<double>(0.0,(MAXLEVEL)- std::min( sqrt(std::pow((double)x-35,2)+pow((double)y-15,2)), sqrt(pow((double)x-15,2)+pow((double)y-35,2)) )/5 );
	//level = 0;
	this->level = capacity;
	this->setRadius(level);
	this->taken=false;
	//this->whoIs=Agent();
	//this->whoIs= NULL;

	int offset = MAXLEVEL-level;
	this->setPosition(x*TILEW+offset,y*TILEH+offset);
}

void Tile::grow(){
	if(level==capacity) return;
	level++;
	setRadius(level);
	int offset = MAXLEVEL-level;
	setPosition(x*TILEW+offset,y*TILEH+offset);
}

int Tile::eat(){
	taken=true;
	int temp = level;
	level=0;
	//whoIs = who;
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
	//whoIs=Agent();
}
//Agent& Tile::getAgent(){
//	return whoIs;
//}
