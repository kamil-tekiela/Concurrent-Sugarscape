#include "agent.h"


Agent::Agent(){
	this->x=0;
	this->y=0;
	this->sugar=0;
	this->vision=5;
}

Agent::Agent(int x, int y){
	this->x=x;
	this->y=y;
	this->sugar= rand()% 15; //random number chosen by fair dice roll
	this->vision=3;
	this->setPosition(x*TILEW,y*TILEH);
	this->setFillColor(sf::Color::Red);
	this->setRadius(RADIUS);
}

void Agent::update(Tile grid[][GRIDH]){
	move(grid);
	//this->setPosition(x*TILEW,y*TILEH);
}

void Agent::move(Tile grid[][GRIDH]){
	std::vector<sf::Vector2i> points;
	int high = 0;
	for(int a=std::max(0,x-vision); a<=std::min(GRIDW-1,x+vision); a++){
		int lvl = grid[a][y].getSugarLvl();
		if(grid[a][y].getSugarLvl() == high){
			points.push_back(sf::Vector2i(a,y));
		}
		else if(grid[a][y].getSugarLvl() > high){
			points.clear();
			points.push_back(sf::Vector2i(a,y));
			high = lvl;
		}
	}
	for(int a=std::max(0,y-vision); a<=std::min(GRIDW-1,y+vision); a++){
		int lvl = grid[x][a].getSugarLvl();
		if(lvl == high){
			points.push_back(sf::Vector2i(x,a));
		}
		else if(lvl > high){
			points.clear();
			points.push_back(sf::Vector2i(x,a));
			high = lvl;
		}
	}

	int random = rand() % points.size();
	x= points.at(random).x;
	y= points.at(random).y;
	grid[x][y].eat();
	setPosition(x*TILEW,y*TILEH);
}

sf::Vector2f Agent::getCoord(){
	return sf::Vector2f(float(x), float(y));
}
