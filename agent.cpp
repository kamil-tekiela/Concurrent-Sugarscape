#include "agent.h"

int Agent::idCounter;

Agent::Agent(){
	this->setVariables();
}

Agent::Agent(int x, int y){
	this->setVariables();
	this->x=			x;
	this->y=			y;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
}

Agent::Agent(int x, int y, double wealth, double met, int vis){
	this->setVariables();
	this->x=			x;
	this->y=			y;
	this->sugar=		wealth;
	this->sugarStart=	(int)wealth;
	this->vision=		vis;
	this->metabolism=	met;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
}

void Agent::setVariables(){
	this->id =			++idCounter;
	this->setFillColor(sf::Color::Red);
	this->x =			0;
	this->y =			0;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
	this->setRadius(RADIUS);
	this->maxAge =		((rand()% (40 * AGEM)) + 60 * AGEM);	// 600 - 1000
	this->age=			((rand()% (50 * AGEM))+ 0 * AGEM); //0 - 500
	this->gender =		(rand()% 2) ? M : F; //shortcut for M=0 F=1;
	this->sugar=		(rand()% 50)+50; 
	this->sugarStart=	(int)sugar;
	this->vision=		(rand()% MAXVISION)+1;
	this->metabolism=	(rand()% MAXMETABOL)+1;
	this->puberty =		((rand()% (4 * AGEM))+ 12 * AGEM); //12 - 15
	this->endFertility =( (gender==F) ? ((rand()% (11 * AGEM))+ 40 * AGEM) : ((rand()% (11 * AGEM))+ 50 * AGEM) );
}

bool Agent::update(Tile grid[][GRIDH], std::vector<Agent*> &agent, double s){
	//movement rule
	move(grid);

	//coloring
	if(vision>= s)
	//if(this->isFertile())
		setFillColor(sf::Color(255, 0, 0));
	else
		setFillColor(sf::Color(0, 0, 255));
	sugar -= metabolism;
	age++;
	
	//death rule
	if(sugar <=0 || age>maxAge){
		return false;
	}

	//mating rule
	int xT = x+1;
	int yT = y;
	xT = xT>=GRIDW ? xT-GRIDW : xT;
	sex(xT, yT, grid, agent);
	xT = x;
	yT = y+1;
	yT = yT>=GRIDH ? yT-GRIDH : yT;
	sex(xT, yT, grid, agent);

	return true;
}

void Agent::move(Tile grid[][GRIDH]){
	std::vector<point> points;
	int high = 0;
	
	//inlcude self? agents dont move!
	//points.push_back(point(x,y,0));
	//high = grid[x][y].getSugarLvl();

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
		for(unsigned int i=1;i<points.size();i++){
			if(points.at(i).dist>min){
				points.erase(points.begin()+i);
				i--;
			}
		}

		// if we have more then random
		random = rand() % points.size();
		int oldx = x; int oldy = y;
		this->x= points.at(random).x;
		this->y= points.at(random).y;
		grid[oldx][oldy].freeUp();
		setPosition((float) x*TILEW, (float) y*TILEH);
	}
	sugar += grid[x][y].eat();
	//else stay on the same tile cause you cant move
}

void Agent::sex(int xT, int yT, Tile grid[][GRIDH], std::vector<Agent*> &agent){
	if(grid[xT][yT].isTaken()){
		sf::Vector2i vecT(xT,yT);
		//find agent living on this tile
		std::vector<Agent*>::iterator it;
		bool found= false;
		for(it=agent.begin(); it != agent.end(); ++it){
			if((*(*it)).getCoord() == vecT){ 
				found = true;
				break;
			}
		}
		if(!found)
			return;
		
		Agent* a = (*it); //wrapper for (*(*it)) -> (*a)

		if((*a).isFertile() && this->isFertile() && (*a).gender != this->gender ){
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
			
			double childMet = (rand()%2) ? this->getMetabolRate() : (*a).getMetabolRate() ;
			int childVis = (rand()%2) ? this->vision : (*a).vision ;
			//childMet = this->getMetabolRate() | (*it).getMetabolRate();
			//childVis = this->vision & (*it).vision;
			Agent *child =  new Agent((*fieldsIt).x, (*fieldsIt).y, 
									this->sugarStart/2+(*a).sugarStart/2, 
									childMet,
									childVis);
			this->sugar -= this->sugarStart/2;
			(*a).sugar -= (*a).sugarStart/2;
			int s = grid[(*fieldsIt).x][(*fieldsIt).y].eat();
			child->addSugar(s);
			agent.push_back(child);
			int childId = (*agent.back()).getId();
			(*a).addChild(childId);
			this->addChild(childId);
		 }
	}
}

bool Agent::isFertile(){
	return (age>=puberty && age<endFertility && sugar>=sugarStart);
}

double Agent::getWealth(){
	return sugar;
}

double Agent::getMetabolRate(){
	return metabolism;
}

sf::Vector2i Agent::getCoord(){
	return sf::Vector2i((x), (y));
}

Sex Agent::getGender(){
	return gender;
}

int Agent::getVision(){
	return vision;
}

void Agent::addChild(int child){
	children.push_back(child);
}

void Agent::addSugar(int amount){
	if(amount>0) sugar+= amount;
}

void Agent::leaveLegacy(std::vector<Agent*> &agent){
	if(sugar<=0) return;
	//what if no children?
	int sugareach = (int)floor(sugar/children.size());
	std::vector<int>::iterator cit=children.begin();
	std::vector<Agent*>::iterator it;
	while(cit!=children.end()){
		for(it=agent.begin(); it != agent.end(); ++it){
			if( (*(*it)).getId() == (*cit) )
			{
				(*(*it)).addSugar(sugareach);
				break;
			}
		}
		++cit;
	}
}

int Agent::getId(){
	return id;
}