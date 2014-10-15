#include "agent.h"


Agent::Agent(){
	this->x=0;
	this->y=0;
	this->sugar=0;
	this->vision= 1;
}

Agent::Agent(int x, int y){
	this->x=			x;
	this->y=			y;
	this->sugar=		(rand()% 50)+1; //random number chosen by fair dice roll
	this->vision=		(rand()% MAXVISION)+1;
	this->metabolism=	(rand()% MAXMETABOL)+1;
	this->setPosition(x*TILEW,y*TILEH);
	this->setFillColor(sf::Color::Red);
	this->setRadius(RADIUS);
	this->maxAge =		(rand() % 100) + 900;	// 900 - 1000
	this->age=0;
	this->gender =		(rand() % 2) ? M : F; //shortcut for M=0 F=1;
}

bool Agent::update(Tile grid[][GRIDH], std::list<Agent> &agent){
	move(grid);
	sugar -= metabolism;
	age++;
	sex(grid, agent);
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
	sugar += grid[x][y].eat();
	//else stay on the same tile cause you cant move
}

void Agent::sex(Tile grid[][GRIDH], std::list<Agent> &agent){
	int xT = x+1;
	int yT;
	xT = xT>=GRIDW ? x-GRIDW : xT;
	yT = y;
	if(grid[xT][yT].isTaken()){
		sf::Vector2i vecT(xT,yT);
		std::list<Agent>::iterator it;
		//find agent living on this tile
		for(it=agent.begin(); it != agent.end(); ++it){
			if((*it).getCoord() == vecT) break;
		}
		if(it == agent.end())
			return;

		if((*it).isFertile() && this->isFertile() && (*it).gender != this->gender ){
			//possible children locations
			std::vector<sf::Vector2i> fields;
			fields.push_back(sf::Vector2i((x+1)>=GRIDW?x+1-GRIDW:x+1, y));
			fields.push_back(sf::Vector2i((x-1)<0?x-1+GRIDW:x-1, y));
			fields.push_back(sf::Vector2i(x, (y+1)>=GRIDH?y+1-GRIDH:y+1));
			fields.push_back(sf::Vector2i(x, (y-1)<0?y-1+GRIDH:y-1));
			fields.push_back(sf::Vector2i((xT+1)>=GRIDW?xT+1-GRIDW:xT+1, yT));
			fields.push_back(sf::Vector2i((xT-1)<0?xT-1+GRIDW:xT-1, yT));
			fields.push_back(sf::Vector2i(xT, (yT+1)>=GRIDH?yT+1-GRIDH:yT+1));
			fields.push_back(sf::Vector2i(xT, (yT-1)<0?yT-1+GRIDH:yT-1));
			std::vector<sf::Vector2i>::iterator fieldsIt=  fields.begin();
			while(fieldsIt != fields.end()){
				if(!grid[(*fieldsIt).x][(*fieldsIt).y].isTaken()){
					break;
				}
				fieldsIt++;
			}
			if(fieldsIt == fields.end())
				return;

			Agent *child = new Agent((*fieldsIt).x, (*fieldsIt).y);
			grid[(*fieldsIt).x][(*fieldsIt).y].eat();
			agent.push_back(*child);
			this->sugar /= 2;
			(*it).sugar /= 2;
		}
	}
}

bool Agent::isFertile(){
	return (age>150 && age<=500 && sugar>50);
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
