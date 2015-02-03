#include "agent.h"

int Agent::idCounter;

//helper function
Agent* whoOwnTile(int x, int y, Agent* agents[GRIDW][GRIDH]){
	int i = (x-MAXVISION)<0 ? (x-MAXVISION)+GRIDW : x-MAXVISION;
	int xLimit = (x+MAXVISION)>=GRIDW ? (x+MAXVISION)-GRIDW : x+MAXVISION;
	int j = (y-MAXVISION)<0 ? (y-MAXVISION)+GRIDH : y-MAXVISION;
	int yLimit = (y+MAXVISION)==GRIDH ? (y+MAXVISION)-GRIDH : y+MAXVISION;
	for(i; i<xLimit; i++)
		for(j; j<yLimit; j++)
			if(agents[i][j])
				if(agents[i][j]->getCoord()==sf::Vector2i(x,y)) 
					return agents[i][j];
	return NULL;
}

bool isVulnerableToRetalation(int x, int y, Agent* agents[GRIDW][GRIDH], int myWealth, int myVision){
	int xOffset = -myVision;
	int yOffset = -myVision;
	for(xOffset; xOffset<myVision; xOffset++){
		int i = (x+xOffset);
		i = i<0 ? i+GRIDW : ((i>=GRIDW) ? i-GRIDW : i);
		for(yOffset; yOffset<myVision; yOffset++)
		{
			if(xOffset * yOffset) continue;//one of offsets must be 0
			int j = (y+yOffset);
			j = j<0 ? j+GRIDW : ((j>=GRIDW) ? j-GRIDW : j);
			if(agents[i][j] && agents[i][j]->getWealth() > myWealth) return true;
		}
	}
}



Agent::Agent(){
	this->setVariables();
}

Agent::Agent(int x, int y){
	this->setVariables();
	this->x=			x;
	this->y=			y;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
}

Agent::Agent(int x, int y, double wealth, double met, int vis, TagString tags){
	this->setVariables();
	this->x=			x;
	this->y=			y;
	this->sugar=		wealth;
	this->sugarStart=	(int)wealth;
	this->vision=		vis;
	this->metabolism=	met;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
	this->tagString =	tags;
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
	assert(sugar==sugarStart);
	this->vision=		(rand()% MAXVISION)+1;
	this->metabolism=	(rand()% MAXMETABOL)+1;
	this->puberty =		((rand()% (4 * AGEM))+ 12 * AGEM); //12 - 15
	this->endFertility =( (gender==F) ? ((rand()% (11 * AGEM))+ 40 * AGEM) : ((rand()% (11 * AGEM))+ 50 * AGEM) );
	toDelete = false;
}

bool Agent::update(Tile grid[][GRIDH], Agent* agents[GRIDW][GRIDH], double s, std::vector<Agent*>& newAgent){
	//movement rule
	//move(grid);	//collecting inside
	//moveWPollution(grid);
	moveWCombat(grid, agents);

	//coloring
	if(tagString.getGroup())
		setFillColor(sf::Color(0, 0, 255));
	else
		setFillColor(sf::Color(255, 0, 0));
	sugar -= metabolism;
	//pollution
	//grid[x][y].genPollutionM(metabolism);
	age++;
	
	//death rule
	if(sugar <=0 || age>maxAge){
		toDelete = true;
		return false;
	}

	//mating rule
	int xT = x+1;
	int yT = y;
	xT = xT>=GRIDW ? xT-GRIDW : xT;
	if(grid[xT][yT].isTaken()){
		if(agents[xT][yT]){
			Agent* a = agents[xT][yT];
			sex(xT, yT, grid, a, newAgent);
			tagString.affected((*a).tagString);
		}
	}
	xT = x;
	yT = y+1;
	yT = yT>=GRIDH ? yT-GRIDH : yT;
	if(grid[xT][yT].isTaken()){
		if(agents[xT][yT]){
			Agent* a = agents[xT][yT];
			sex(xT, yT, grid, a, newAgent);
			tagString.affected((*a).tagString);
		}
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
// code copying; only difference is the getSugarLvl to getS_Pratio()
void Agent::moveWPollution(Tile grid[][GRIDH]){
	std::vector<point> points;
	int high = 0;

	for(int a=x-vision; a<=x+vision; a++){
		int aT = a < 0 ? GRIDW+a : a >= GRIDW ? a-GRIDW : a;
		if(grid[aT][y].isTaken()) continue;
		int lvl = grid[aT][y].getS_Pratio();
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
		int lvl = grid[x][aT].getS_Pratio();
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
// code copying; only difference is the getSugarLvl
void Agent::moveWCombat(Tile grid[][GRIDH], Agent* agents[][GRIDH]){
	std::vector<pointWCombat> points;
	int high = 0;
	int myGroup = tagString.getGroup();

	for(int a=x-vision; a<=x+vision; a++){
		int aT = a < 0 ? GRIDW+a : a >= GRIDW ? a-GRIDW : a;
		Agent* temp = whoOwnTile(aT, y, agents);
		//assert(grid[aT][y].isTaken() && temp);
		if(grid[aT][y].isTaken()){//NULL is only if child is encountered
			if((temp==NULL 
				|| temp->tagString.getGroup()==myGroup 
				|| temp->getWealth() > this->getWealth() 
				|| isVulnerableToRetalation(aT, y, agents, this->getWealth(), this->getVision()) ) ) 
			continue;
		}
		int lvl = grid[aT][y].getSugarLvl();
		int sugarBonus = 0;
		if(temp)//if occupied
			sugarBonus = MIN(temp->getWealth(), LOOTLIMIT);
		lvl += sugarBonus;
		if(lvl == high){
			points.push_back(pointWCombat(aT,y,abs(x-a), sugarBonus, temp));
		}
		else if(lvl > high){
			points.clear();
			points.push_back(pointWCombat(aT,y,abs(x-a), sugarBonus, temp));
			high = lvl;
		}
	}
	//same for y axis vision
	for(int a=y-vision; a<=y+vision; a++){
		int aT = a < 0 ? GRIDH+a : a >= GRIDH ? a-GRIDH : a;
		Agent* temp = whoOwnTile(x, aT, agents);
		if(grid[x][aT].isTaken()){//NULL is only if child is encountered
			if((temp==NULL 
				|| temp->tagString.getGroup()==myGroup 
				|| temp->getWealth() > this->getWealth() 
				|| isVulnerableToRetalation(aT, y, agents, this->getWealth(), this->getVision()) ) ) 
			continue;
		}
		int lvl = grid[x][aT].getSugarLvl();
		int sugarBonus = 0;
		if(temp)//if occupied
			sugarBonus = MIN(temp->getWealth(), LOOTLIMIT);
		lvl += sugarBonus;
		if(lvl == high){
			points.push_back(pointWCombat(x,aT,abs(y-a), sugarBonus, temp));
		}
		else if(lvl > high){
			points.clear();
			points.push_back(pointWCombat(x,aT,abs(y-a), sugarBonus, temp));
			high = lvl;
		}
	}

	int random;
	//add while loop for concurrency
	if(points.size()){
		//find the largest CLOSEST sugar tile
		//cumbersome but works
		std::sort( points.begin(), points.end() );//sort calls point's operator >
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
		if(points.at(random).agent!=NULL)//prevent NULL data where the new position was empty
			points.at(random).agent->kill( points.at(random).sugar );//
		this->addSugar(points.at(random).sugar);//
		grid[oldx][oldy].freeUp();
		setPosition((float) x*TILEW, (float) y*TILEH);
	}
	sugar += grid[x][y].eat();
	//else stay on the same tile cause you cant move
}

void Agent::sex(int xT, int yT, Tile grid[][GRIDH], Agent* &a, std::vector<Agent*>& newAgent){

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
		TagString tempTag;
		tempTag.setFromParents(this->tagString, (*a).tagString);

		//childMet = this->getMetabolRate() | (*it).getMetabolRate();
		//childVis = this->vision & (*it).vision;
		Agent *child =  new Agent((*fieldsIt).x, (*fieldsIt).y, 
								this->sugarStart/2+(*a).sugarStart/2, 
								childMet,
								childVis,
								tempTag);
		this->sugar -= this->sugarStart/2;
		(*a).sugar -= (*a).sugarStart/2;
		int s = grid[(*fieldsIt).x][(*fieldsIt).y].eat();
		child->addSugar(s);
		newAgent.push_back(child);
		int childId = (*child).getId();
		(*a).addChild(childId);
		this->addChild(childId);
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

void Agent::leaveLegacy(Agent* agents[GRIDW][GRIDH]){
	if(sugar<=0) return;
	//what if no children?
	int sugareach = (int)floor(sugar/children.size());
	std::vector<int>::iterator cit=children.begin();
	//std::vector<Agent*>::iterator it;
	while(cit!=children.end()){
		for(int x=0; x<GRIDW; x++){
			for(int y=0; y<GRIDH; y++){
				if(agents[x][y]){
					Agent* a = agents[x][y];
					if( (*a).getId() == (*cit) )
					{
						(*a).addSugar(sugareach);
						break;
					}
				}
			}
		}

		++cit;
	}
}

int Agent::getId(){
	return id;
}

void Agent::kill(int sugarTaken){
	sugar-= sugarTaken>sugar ? sugar : sugarTaken;
	toDelete=true;
	std::cout << "Killed: " << sugarTaken << std::endl;
}