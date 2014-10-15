#include "agent.h"


Agent::Agent(){
	this->x=0;
	this->y=0;
	this->sugar=0;
	this->vision= 1;
}

Agent::Agent(int x, int y){
	this->x=x;
	this->y=y;
	this->sugar= (rand()% 50)+1; //random number chosen by fair dice roll
	this->vision= (rand()% MAXVISION)+1;
	this->metabolism= (rand()% MAXMETABOL)+1;
	this->setPosition(x*TILEW,y*TILEH);
	this->setFillColor(sf::Color::Red);
	this->setRadius(RADIUS);
	this->maxAge = (rand() % 100) + 900;	// 900 - 1000
	this->age=0;
	this->gender = (rand() % 2) ? M : F; //shortcut for M=0 F=1;
}

bool Agent::update(Tile grid[][GRIDH]){
	move(grid);
	sugar -= metabolism;
	age++;
	sex(grid);
	if(sugar <=0 || age>maxAge){
		return false;
	}
	return true;
}

void Agent::move(Tile grid[][GRIDH]){
	std::vector<point> points;
	int high = 0;
	for(int a=x-vision; a<=x+vision; a++){
		int aT = a < 0 ? GRIDW+a : a >= GRIDW ? a-GRIDW : a;
		if(grid[aT][y].isTaken()) continue;
		int lvl = grid[aT][y].getSugarLvl();
		if(lvl == high){
			points.push_back(point(aT,y,abs(x-a)));
		}
		else if(lvl > high){
			points.clear();
			points.push_back(point(aT,y,abs(x-a)));
			high = lvl;
		}
	}
	for(int a=y-vision; a<=y+vision; a++){
		int aT = a < 0 ? GRIDH+a : a >= GRIDH ? a-GRIDH : a;
		if(grid[x][aT].isTaken()) continue;
		int lvl = grid[x][aT].getSugarLvl();
		if(lvl == high){
			points.push_back(point(x,aT,abs(y-a)));
		}
		else if(lvl > high){
			points.clear();
			points.push_back(point(x,aT,abs(y-a)));
			high = lvl;
		}
	}

	int random;
	//add while loop for concurrency
	if(points.size()){
		//find the largest CLOSEST sugar tile
		//cumbersome but works
		std::sort( points.begin(), points.end() );
		int min = points.at(0).dist;
		for(int i=1;i<points.size();i++){
			if(points.at(i).dist>min){
				points.erase(points.begin()+i);
				i--;
			}
		}

		// if we have more then random
		random = rand() % points.size();
		int oldx = x; int oldy = y;
		x= points.at(random).x;
		y= points.at(random).y;
		grid[oldx][oldy].freeUp();
		setPosition(x*TILEW,y*TILEH);
	}
	sugar += grid[x][y].eat(*this);
	//else stay on the same tile cause you cant move
}

void Agent::sex(Tile grid[][GRIDH]){
	int xT = x+1;
	xT = xT>=GRIDW ? x-GRIDW : xT;
	if(grid[xT][y].isTaken()){
		Agent& mate = grid[xT][y].getAgent();
		//Sex g = mate->getGender();
		//std::cout << g << std::endl;
		//if(mate->gender != this->gender){
		//	// we have a mate
		//	std::cout << "WE HAVE A MATE" << std::endl;
		//}
	}
}

int Agent::getWealth(){
	return sugar;
}

int Agent::getMetabolRate(){
	return metabolism;
}

sf::Vector2i Agent::getCoord(){
	return sf::Vector2i((x), (y));
}

Sex Agent::getGender(){
	return gender;
}
