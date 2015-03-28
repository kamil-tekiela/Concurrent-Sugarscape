#include "tile.h"


Tile::Tile(){
	this->x=0;
	this->y=0;
	this->taken=false;
}

Tile::Tile(int x, int y){
	this->x=			x;
	this->y=			y;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
	this->setFillColor(sf::Color::Green);
	//4 is max level of sugar (floored)
	this->capacity =	(int)std::max<double>(0.0,(MAXLEVEL)- std::min( sqrt(std::pow((double)x-35,2)+pow((double)y-15,2)), sqrt(pow((double)x-15,2)+pow((double)y-35,2)) )/5 );
	//this->capacity =	MAXLEVEL;
	this->level =		capacity;
	this->capacitySpice =	(int)std::max<double>(0.0,(MAXLEVEL)- std::min( sqrt(std::pow((double)x-12,2)+pow((double)y-12,2)), sqrt(pow((double)x-38,2)+pow((double)y-38,2)) )/4 );
	this->levelSpice =		capacitySpice;
	this->setRadius(std::max(static_cast<float>(level), static_cast<float>(levelSpice)));
	this->taken=		false;
	this->pollution=	0;

	float offset =		static_cast<float>(MAXLEVEL-level);
	this->setPosition(static_cast<float> (x*TILEW+offset), static_cast<float> (y*TILEH+offset));
	if(capacitySpice > capacity)
		setFillColor(sf::Color::Yellow);
	else
		setFillColor(sf::Color::Green);
}

void Tile::grow(Tile grid[][GRIDH]){
	if(MOVEMENT==WithTrade){
		level = std::min(capacity, static_cast<short>(level+1));
		levelSpice = std::min(capacitySpice, static_cast<short>(levelSpice+1));
		float r = std::max(static_cast<float>(level), static_cast<float>(levelSpice));
		this->setRadius(r);
		float offset = MAXLEVEL-r;
		setPosition(static_cast<float> (x*TILEW+offset), static_cast<float> (y*TILEH+offset));

		if(MOVEMENT==WithPollution)
			pollutionDiffusion(grid);
	}
	else{
		if(level>=capacity) return;
		level++;
		setRadius(level);
		float offset =		static_cast<float>(MAXLEVEL-level);
		setPosition(static_cast<float> (x*TILEW+offset), static_cast<float> (y*TILEH+offset));

		if(MOVEMENT==WithPollution)
			pollutionDiffusion(grid);
	}
}

void Tile::seasonalGrow(Tile grid[][GRIDH], int time, int seasonLen){
	if(level>=capacity) {
		level = floor(level);
		return;
	}
	float gamma = 8.f;
	if((time%(2*seasonLen))/seasonLen != (int)(y< GRIDH/2))
		level++;
	else
		level = level + 1.f/gamma;
	setRadius(std::max(0.f, std::floor(level)));
	int offset = MAXLEVEL-std::max(0, (int)level);
	setPosition((float) x*TILEW+offset, (float) y*TILEH+offset);

	if(MOVEMENT==WithPollution)
		pollutionDiffusion(grid);
}

void Tile::pollutionDiffusion(Tile grid[][GRIDH]){
	int xl = x==0 ? GRIDW-1 : x-1;
	int xh = x==GRIDW-1 ? 0 : x+1;
	int yl = y==0 ? GRIDH-1 : y-1;
	int yh = y==GRIDH-1 ? 0 : y+1;
	float flux = 0;
	flux += grid[xl][y].pollution;
	flux += grid[xh][y].pollution;
	flux += grid[x][yl].pollution;
	flux += grid[x][yh].pollution;
	pollution = flux/4.f;
	if(pollution<0) pollution=0;
}

int Tile::eat(){
	taken=true;
	int temp = std::max(0, (int)level);
	if(MOVEMENT==WithPollution)
		genPollutionG(temp);
	level=-1;	//either 0 or -1; -1 yields better results but 0 is more correct
	return temp;
}

int Tile::eatSpice(){
	int temp = std::max(0, (int)levelSpice);
	levelSpice=-1;	//either 0 or -1; -1 yields better results but 0 is more correct
	return temp;
}

sf::Vector2f Tile::getCoord(){
	return sf::Vector2f(float(x), float(y));
}
int Tile::getSugarLvl(){
	return std::max(0, (int)level);
}
int Tile::getSpiceLvl(){
	return std::max(0, (int)levelSpice);
}
float Tile::getS_Pratio(){
	return level/(1+pollution);
}
bool Tile::isTaken(){
	return taken;
}
void Tile::freeUp(){
	taken = false;
}

void Tile::genPollutionG(float amount){
	this->pollution += POLLUTIONALPHA * amount;
}

void Tile::genPollutionM(float amount){
	this->pollution += POLLUTIONBETA * amount;
}
