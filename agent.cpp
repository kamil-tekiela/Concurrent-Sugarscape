#include "agent.h"

int Agent::idCounter;

//helper functions
Agent* getAgentByCoord(std::vector<Agent*> &agent, int x, int y){
	std::vector<Agent*>::iterator it;
	sf::Vector2i pos(x,y);
	for(it=agent.begin(); it != agent.end(); ++it){
		if( (*(*it)).getCoord() == pos)
		{
			return (*it);
		}
	}
	return NULL;
}

bool isVulnerableToRetaliation(std::vector<Agent*> &agent, int x, int y, int myGroup, int myVision, double myWealth){
	int xLow = (x-myVision)<0 ? (x-myVision)+GRIDW : (x-myVision);
	int xHigh = (x+myVision)>=GRIDW ? (x+myVision)-GRIDW : (x+myVision);
	int yLow = (y-myVision)<0 ? (y-myVision)+GRIDH : (y-myVision);
	int yHigh = (y+myVision)>=GRIDH ? (y+myVision)-GRIDH : (y+myVision);
	std::vector<Agent*>::iterator it;
	sf::Vector2i pos(x,y);
	for(it=agent.begin(); it != agent.end(); ++it){
		Agent* temp = (*it);
		sf::Vector2i vec = temp->getCoord();
		if((vec.x==x && vec.y>=yLow && vec.y<=yHigh) || (vec.y==y && vec.x>=xLow && vec.x<=xHigh))
			if(temp->tagString.getGroup() != myGroup && temp->getWealth() > myWealth) 
				return true;
	}
	return false;
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

Agent::Agent(int x, int y, double wealth, double met, int vis, TagString tags, ImmuneSys immuneSys) 
: genotype(immuneSys) {
	this->setVariables();
	this->x=			x;
	this->y=			y;
	this->sugar=		wealth;
	this->sugarStart=	static_cast<int>(wealth);
	this->vision=		vis;
	this->metabolism=	met;
	this->setPosition((float) x*TILEW, (float) y*TILEH);
	this->tagString =	tags;
	this->phenotype =	immuneSys;
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
	this->sugarStart=	static_cast<int>(sugar);
	this->spices=		(rand()% 50)+50; 
	this->vision=		(rand()% MAXVISION)+1;
	this->metabolism=	(rand()% MAXMETABOL)+1;
	this->metabolismSpice=(rand()% MAXMETABOL)+1;
	this->puberty =		((rand()% (4 * AGEM))+ 12 * AGEM); //12 - 15
	this->endFertility =( (gender==F) ? ((rand()% (11 * AGEM))+ 40 * AGEM) : ((rand()% (11 * AGEM))+ 50 * AGEM) );
	this->dead =		false;
	this->phenotype =	this->genotype;
	this->metabolicFee =0;
}

bool Agent::update(Tile grid[][GRIDH], std::vector<Agent*> &agent, double s){
	//coloring
	if(tagString.getGroup())
	//if(diseases.size()==0)
		setFillColor(sf::Color::Blue);
	else
		setFillColor(sf::Color::Red);
	sugar -= metabolism + metabolicFee;
	if(MOVEMENT==WithTrade){
		spices -= metabolismSpice + metabolicFee;
	}
	//pollution
	if(MOVEMENT==WithPollution){
		grid[x][y].genPollutionM(metabolism);
	}
	age++;
	
	//death rule
	if(sugar <=0 || age>maxAge || dead || (MOVEMENT==WithTrade && spices<=0)){
		return false;
	}

	//movement rule
	if(MOVEMENT==WithCombat)
		moveWCombat(grid, agent);
	else
		move(grid);

	//Von Neumann's neighbourhood
	std::vector<sf::Vector2i> positions;
	positions.push_back(sf::Vector2i(x+1>=GRIDW ? x+1-GRIDW : x+1, y));
	positions.push_back(sf::Vector2i(x, y+1>=GRIDH ? y+1-GRIDH : y+1));
	positions.push_back(sf::Vector2i(x-1<0 ? x-1+GRIDW : x-1, y));
	positions.push_back(sf::Vector2i(x, y-1<0 ? y-1+GRIDH : y-1));
	std::vector<Agent*> vonNeumanns;
	for(unsigned i=0; i<positions.size(); ++i){
		if(grid[positions.at(i).x][positions.at(i).y].isTaken()){
			//find agent living on this tile
			std::vector<Agent*>::iterator it;
			for(it=agent.begin(); it != agent.end(); ++it){
				if((*(*it)).getCoord() == positions.at(i)){ 
					vonNeumanns.push_back((*it));
					break;
				}
			}
		}
	}
	//apply rules to neighbours
	for(unsigned i=0; i<vonNeumanns.size(); ++i){
		Agent* a = vonNeumanns.at(i);
		if(MATING)		sex(grid, agent, a);
		if(CULTURE)		tagString.affected(a->tagString);
		if(DISEASE)		giveDisease(a);
		if(TRADING)		trade(a);
	}

	if(DISEASE)		this->immuneResponse(); 

	return true;
}

void Agent::move(Tile grid[][GRIDH]){
	std::vector<point> points;
	double high = 0;
	double lvl = 0;
	
	for(int a=x-vision; a<=x+vision; a++){
		int aT = a < 0 ? GRIDW+a : a >= GRIDW ? a-GRIDW : a;
		if(grid[aT][y].isTaken()) continue;
		if(MOVEMENT==WithPollution){
			lvl = grid[aT][y].getS_Pratio();
		}
		else if(MOVEMENT==WithTrade){
			//welfare
			double sug = grid[aT][y].getSugarLvl();
			double spi = grid[aT][y].getSpiceLvl();
			lvl = static_cast<int>(this->welfare(this->sugar+sug, this->spices+spi));
		}
		else{
			lvl = grid[aT][y].getSugarLvl();
		}
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
		if(MOVEMENT==WithPollution){
			lvl = grid[aT][y].getS_Pratio();
		}
		else if(MOVEMENT==WithTrade){
			//welfare
			double sug = grid[x][aT].getSugarLvl();
			double spi = grid[x][aT].getSpiceLvl();
			lvl = static_cast<int>(this->welfare(this->sugar+sug, this->spices+spi));
		}
		else{
			lvl = grid[x][aT].getSugarLvl();
		}
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
	spices += grid[x][y].eatSpice();
	//else stay on the same tile cause you cant move
}
// code copying;
void Agent::moveWCombat(Tile grid[][GRIDH], std::vector<Agent*> &agent){
	std::vector<pointWCombat> points;
	int high = 0;
	int myGroup = this->tagString.getGroup();

	for(int a=x-vision; a<=x+vision; a++){
		int aT = a < 0 ? GRIDW+a : a >= GRIDW ? a-GRIDW : a;
		Agent* temp = getAgentByCoord(agent, aT, y);
		if(grid[aT][y].isTaken() 
			&& (temp->tagString.getGroup()==myGroup
				|| temp->getWealth() > this->getWealth() 
				|| isVulnerableToRetaliation(agent, aT, y, myGroup, this->getVision(), this->getWealth()) )) {
			continue;
		}
		int lvl = grid[aT][y].getSugarLvl();
		int bonusSugar = 0;
		if(temp)
			bonusSugar = static_cast<int>(MIN(LOOTLIMIT, floor(temp->getWealth())));
		lvl += bonusSugar;
		if(lvl == high){
			points.push_back(pointWCombat(aT,y,abs(x-a), bonusSugar, temp));
		}
		else if(lvl > high){
			points.clear();
			high = lvl;
			points.push_back(pointWCombat(aT,y,abs(x-a), bonusSugar, temp));
		}
	}
	for(int a=y-vision; a<=y+vision; a++){
		int aT = a < 0 ? GRIDH+a : a >= GRIDH ? a-GRIDH : a;
		Agent* temp = getAgentByCoord(agent, x, aT);
		if(grid[x][aT].isTaken() 
			&& (temp->tagString.getGroup()==myGroup
				|| temp->getWealth() > this->getWealth() 
				|| isVulnerableToRetaliation(agent, x, aT, myGroup, this->getVision(), this->getWealth()) )) {
			continue;
		}
		int lvl = grid[x][aT].getSugarLvl();
		int bonusSugar=0;
		if(temp)
			bonusSugar = static_cast<int>(MIN(LOOTLIMIT, floor(temp->getWealth())));
		lvl += bonusSugar;
		if(lvl == high){
			points.push_back(pointWCombat(x,aT,abs(y-a), bonusSugar, temp));
		}
		else if(lvl > high){
			points.clear();
			high = lvl;
			points.push_back(pointWCombat(x,aT,abs(y-a), bonusSugar, temp));
		}
	}

	int random;
	//add while loop for concurrency
	if(points.size()){
		//find the largest CLOSEST sugar tile
		//cumbersome but works
		std::sort( points.begin(), points.end() );
		int min = points.at(0).dist;
		for(unsigned int i=1;i<points.size();++i){
			if(points.at(i).dist>min){
				points.erase(points.begin()+i);
				i--;
			}
		}

		// if we have more than one possible destination then pick one randomly
		random = rand() % points.size();
		int oldx = x; int oldy = y;
		this->x= points.at(random).x;
		this->y= points.at(random).y;
		grid[oldx][oldy].freeUp();
		if(points.at(random).agent)
			points.at(random).agent->kill( points.at(random).sugar );
		this->addSugar( points.at(random).sugar);
		setPosition((float) x*TILEW, (float) y*TILEH);
	}
	sugar += grid[x][y].eat();
	//else stay on the same tile cause you cant move
}

void Agent::sex(Tile grid[][GRIDH], std::vector<Agent*> &agent, Agent* &a){
	int xT = a->getCoord().x;
	int yT = a->getCoord().y;

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
		tempTag.setFromParents(this->tagString, a->tagString);
		ImmuneSys tempImmu;
		tempImmu.setFromParents(this->genotype, a->genotype);

		//childMet = this->getMetabolRate() | (*it).getMetabolRate();
		//childVis = this->vision & (*it).vision;
		Agent *child =  new Agent((*fieldsIt).x, (*fieldsIt).y, 
								this->sugarStart/2+(*a).sugarStart/2, 
								childMet,
								childVis,
								tempTag, 
								tempImmu);
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

void Agent::trade(Agent* &a){
	double myMRS =	this->	getMRS();
	double hisMRS =	a->		getMRS();
	if(myMRS == hisMRS)		return;
	double p =				sqrt(myMRS * hisMRS);//geometric mean
	double sugarExch =		1;
	double spicesExch =		1;
	if(p>1)					spicesExch = p;
	else if(p<1)			sugarExch = 1/p;
	//sugar ->>> a
	//spice <<<- a
	if(myMRS < hisMRS){
		double wPre1 =	this->welfare(this->sugar, this->spices);
		double wPost1 =	this->welfare(this->sugar - sugarExch, this->spices + spicesExch);
		double wPre2 =	a->welfare(a->getWealth(), a->getSpices());
		double wPost2 =	a->welfare(a->getWealth() + sugarExch, a->getSpices() - spicesExch);
		double myMRSnew = this->getMRS(-sugarExch, +spicesExch);
		double hisMRSnew = a->getMRS(+sugarExch, -spicesExch);
		//are they better off, is there a crossover?
		if(wPre1<wPost1 && wPre2<wPost2 && myMRSnew<hisMRSnew){
			//make trade
			this->subSugar(sugarExch);
			this->addSpices(spicesExch);
			a->addSugar(sugarExch);
			a->subSpices(spicesExch);
			this->trade(a);
			//std::cout << sugarExch << "\t" << spicesExch << std::endl;
		}
	}
	//sugar <<<- a
	//spice ->>> a
	else if(myMRS > hisMRS){
		double wPre1 =	this->welfare(this->sugar, this->spices);
		double wPost1 =	this->welfare(this->sugar + sugarExch, this->spices - spicesExch);
		double wPre2 =	a->welfare(a->getWealth(), a->getSpices());
		double wPost2 =	a->welfare(a->getWealth() - sugarExch, a->getSpices() + spicesExch);
		double myMRSnew = this->getMRS(+sugarExch, -spicesExch);
		double hisMRSnew = a->getMRS(-sugarExch, +spicesExch);
		//are they better off, is there a crossover?
		if(wPre1<wPost1 && wPre2<wPost2 && myMRSnew>hisMRSnew){
			//make trade
			this->addSugar(sugarExch);
			this->subSpices(spicesExch);
			a->subSugar(sugarExch);
			a->addSpices(spicesExch);
			this->trade(a);
		}
	}
}

double Agent::welfare(double w1, double w2){
	double mt = metabolism+metabolismSpice;
	return std::pow(w1, metabolism/mt) * std::pow(w2, metabolismSpice/mt);
}

double Agent::getMRS(double sug, double spi){
	return (metabolism*(spices+spi))/(metabolismSpice*(sugar+sug));
}

bool Agent::isFertile(){
	return (age>=puberty && age<endFertility && sugar>=sugarStart);
}

double Agent::getWealth(){
	return sugar;
}

double Agent::getSpices(){
	return spices;
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

void Agent::addSugar(double amount){
	if(amount>0) sugar+= amount;
}
void Agent::subSugar(double amount){
	if(amount>0) sugar-= amount;
}
void Agent::addSpices(double amount){
	if(amount>0) spices+= amount;
}
void Agent::subSpices(double amount){
	if(amount>0) spices-= amount;
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

void Agent::kill(int sugarTaken){
	this->sugar -= sugarTaken>0 ? sugarTaken : 0;
	this->dead = true;//killed on next iteration
}

void Agent::receiveDisease(Disease disease){
	if(phenotype.isSubstring(disease)) return;
	for (std::vector<Disease>::iterator it = diseases.begin(); it != diseases.end(); ++it)
		if((*it)==disease) return;
	diseases.push_back(disease);
	metabolicFee += 0.5;
}

void Agent::giveDisease(Agent* &a){
	if(diseases.size()==0) return;
	int random = 0;
	if(diseases.size() > 1)
		random = rand() % diseases.size();
	a->receiveDisease(diseases.at(random));
}

void Agent::immuneResponse(){
	for (std::vector<Disease>::iterator it = diseases.begin(); it != diseases.end(); ){
		if(phenotype.affected((*it)))
		{
			//assuming the disease cannot be deleted, only its symptoms can be cured
			//it = diseases.erase(it);
			++it;
			//std::cout << "rid" << std::endl;
		}
		else
			++it;
	}
	metabolicFee = diseases.size() * 0.5;
}

int Agent::test(){
	return diseases.size();
}