#pragma once

#include "settings.h"

#include "tile.h"
#include "tagString.h"
#include "immuneSys.h"
#include "disease.h"
#include "loan.h"
#include <list>

#include <assert.h>

enum Sex{
	M,
	F
};

class Agent : public sf::CircleShape 
{
private:
	static int idCounter;
	int id;
	int x;
	int y;
	double sugar;
	int sugarStart;
	double spices;
	short int vision;
	double metabolism;
	double metabolismSpice;
	double metabolicFee;
	short int maxAge;
	short int age;
	short int puberty;
	short int endFertility;
	Sex	gender;
	std::vector<int> children;
	bool dead;
	ImmuneSys phenotype;
	std::vector<Disease> diseases;
	std::list<Loan> loansTaken;
	std::list<Loan> loansGiven;

	
	void move(Tile[][GRIDH]);
	void moveWPollution(Tile[][GRIDH]);
	void moveWCombat(Tile[][GRIDH],  Agent *[GRIDW*GRIDH]);
	void setVariables();

public:
	TagString tagString;
	const ImmuneSys genotype;

public:
	Agent();
	Agent(int x, int y);
	Agent(int x, int y, double wealth, double met, int vis, TagString tags, ImmuneSys immuneSys);
	
	bool update(Tile[][GRIDH], Agent *[GRIDW*GRIDH], double s);
	void sex(Tile[][GRIDH], Agent *[GRIDW*GRIDH], Agent* &a);
	void giveDisease(Agent* &a);
	void immuneResponse();
	void trade(Agent* &a);
	sf::Vector2i getCoord();
	double getWealth();
	double getSpices();
	double getMetabolRate();
	Sex getGender();
	bool isFertile();
	int getVision();
	void addChild(int child);
	void addSugar(double amount);
	void subSugar(double amount);
	void addSpices(double amount);
	void subSpices(double amount);
	int canLend();
	bool wantsToBorrow();
	void takesLoan(Loan l);
	void givesLoan(Loan l);
	void removeLoanGiven(Loan l);
	void removeLoanTaken(Loan l);
	void payDebts(int time);
	/**
	* Iterates over all children and looks for each of them in an agent vector
	* If it is alive then give it its inheritance
	**/
	void leaveLegacy(Agent *[GRIDW*GRIDH]);
	int getId();
	void kill(int sugarTaken=0);
	void receiveDisease(Disease disease);
	double welfare(double w1, double w2);
	double getMRS(double sug=0, double spi=0);
	bool isDead();
	int test();

	bool moved;
};

struct point {
	int x;
	int y;
	int dist;
	point(int px, int py, int pdist){x=px;y=py;dist=pdist;}
	bool operator<( const point& val ) const { 
    	return dist < val.dist; 
    }
};

struct pointWCombat {
	int x;
	int y;
	Agent* agent;
	int sugar;
	int dist;
	pointWCombat(int px, int py, int pdist, int psugar, Agent* pagent){x=px;y=py;dist=pdist;sugar=psugar;agent=pagent;}
	bool operator<( const pointWCombat& val ) const { 
    	return dist < val.dist; 
    }
};

#define MIN(a,b) (((a)<(b))?(a):(b))