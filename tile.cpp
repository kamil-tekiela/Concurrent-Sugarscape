#include "tile.h"


Tile::Tile(){
	this->x=0;
	this->y=0;
	this->taken=false;
}

Tile::Tile(int x, int y){
	this->x=x;
	this->y=y;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
	this->setFillColor(sf::Color::Green);
	//4 is max level of sugar (floored)
	this->capacity = (int)std::max<double>(0.0,(MAXLEVEL)- std::min( sqrt(std::pow((double)x-35,2)+pow((double)y-15,2)), sqrt(pow((double)x-15,2)+pow((double)y-35,2)) )/5 );
	//level = 0;
	this->level = capacity;
	this->setRadius(level);
	this->taken=false;

	int offset = MAXLEVEL-level;
	this->setPosition((float) x*TILEW+offset, (float) y*TILEH+offset);
}

void Tile::grow(){
	/*if(taken)
		setFillColor(sf::Color::Black);
	else
		setFillColor(sf::Color::Green);*/

	if(level>=capacity) return;
	level++;
	setRadius(level);
	int offset = MAXLEVEL-level;
	setPosition((float) x*TILEW+offset, (float) y*TILEH+offset);
}

void Tile::seasonalGrow(int time, int seasonLen){
	if(level>=capacity) {
		level = floor(level);
		return;
	}
	float gamma = 8.f;
	if((time%(2*seasonLen))/seasonLen != (int)(y< GRIDH/2))
		level++;
	else
		level = level + 1.f/gamma;
	setRadius(std::max(0, (int)level));
	int offset = MAXLEVEL-std::max(0, (int)level);
	setPosition((float) x*TILEW+offset, (float) y*TILEH+offset);
}

int Tile::eat(){
	taken=true;
	int temp = std::max(0, (int)level);
	level=-1;	//either 0 or -1; -1 yields better results but 0 is more correct
	return temp;
}

sf::Vector2f Tile::getCoord(){
	return sf::Vector2f(float(x), float(y));
}
int Tile::getSugarLvl(){
	return std::max(0, (int)level);
}
bool Tile::isTaken(){
	return taken;
}
void Tile::freeUp(){
	taken = false;
}
