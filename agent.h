#pragma once

#include "settings.h"

#include "tile.h"
#include "tagString.h"
#include "immuneSys.h"
#include "disease.h"
#include "loan.h"
#include <list>


enum Sex{
	M,/**< Male. */ 
	F/**<Female.  */
};

/**
* Class defining a single agent.
**/
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

	
	/**
	* Movement rule; This method is called internally by the update method.
	* @param grid a pointer to a 2 dimensional array of sugarscape tiles
	* This method looks for a new site according to the movement rule and then collects the resources at the site
	* @see update
	* @see moveWCombat
	**/
	void move(Tile[][GRIDH]);/**
	* Movement rule; This method is called internally by the update method.
	* @see update
	* @see move
	**/
	void moveWCombat(Tile[][GRIDH],  Agent *[GRIDW*GRIDH]);
	void setVariables();

public:
	/**
	* Agents cultural tag string.
	* This is made public because neighbouring agent may call an affected function on this member object
	**/
	TagString tagString;
	/**
	* Agents immune system genotype.
	**/
	const ImmuneSys genotype;
	/*
	\cond HIDDEN_SYMBOLS
	*/
	bool moved;
	/*
	\endcond
	*/

public:
	/**
	* Default constructor.
	* @see Agent(int x, int y)
	* @see Agent(int x, int y, double wealth, double met, int vis, TagString tags, ImmuneSys immuneSys)
	**/
	Agent();
	/**
	* Constructor.
	* Creates an agent object with random characteristics.
	* Takes in as a parameter two integers specifing its position on the lattice
	* @see Agent(int x, int y, double wealth, double met, int vis, TagString tags, ImmuneSys immuneSys)
	**/
	Agent(int x, int y);
	/**
	* Constructor.
	* This constructor is called when you need to create an agent with specific inital characteristics. This is usually necessary only when a child is born.
	* @see Agent(int x, int y)
	**/
	Agent(int x, int y, double wealth, double met, int vis, TagString tags, ImmuneSys immuneSys);
	
	/**
	* Movement rule; This method is to be called first every iteration.
	* @param grid a pointer to a 2 dimensional array of sugarscape tiles
	* @param agent a pointer to an array of pointers to all the living agents
	* This method is responsible for agent's metabolism, aging, generating pollution and movement.
	**/
	void update(Tile[][GRIDH], Agent *[GRIDW*GRIDH]);
	/**
	* Mating rule.
	* @param grid a pointer to a 2 dimensional array of sugarscape tiles
	* @param agent a pointer to an array of pointers to all the living agents
	* @param agent a pointer to a sexual partner
	* This method is responsible for checking if mating is possible between those two agents and generating offspring.
	* The child is then automatically added to the lattice
	**/
	void sex(Tile[][GRIDH], Agent *[GRIDW*GRIDH], Agent* &a);
	/**
	* Disease rule.
	* @param agent a pointer to a neighbouring agent
	* This method is responsible for spreading diseases
	* A single disease is picked at random and given to the neighbour
	* @see immuneResponse
	**/
	void giveDisease(Agent* &a);
	/**
	* Disease rule.
	* This method is responsible for immune system according to the immune system rule
	* @see giveDisease
	**/
	void immuneResponse();
	/**
	* Trade rule.
	* @param agent a pointer to a neighbouring agent
	* If a trade between this two agents is possible then the agents will exchange resources
	**/
	void trade(Agent* &a);
	/**
	* Returns current position on the lattice as a sf::Vector2i.
	**/
	sf::Vector2i getCoord();
	/**
	* Returns amount of sugar accumulated.
	**/
	double getWealth();
	/**
	* Returns amount of spices accumulated.
	**/
	double getSpices();
	double getMetabolRate();
	Sex getGender();
	/**
	* Tells if the agent is able to have children this turn.
	**/
	bool isFertile();
	int getVision();
	/**
	* Informs the agent that it has just became a parent.
	**/
	void addChild(int child);
	/**
	* Add sugar to the agent.
	* @param amount A positive double number indicating the amount to be added
	**/
	void addSugar(double amount);
	/**
	* Subtract sugar from the agent.
	* @param amount A positive double number indicating the amount to be subtracted
	**/
	void subSugar(double amount);
	/**
	* Add spices to the agent.
	* @param amount A positive double number indicating the amount to be added
	**/
	void addSpices(double amount);
	/**
	* Subtract spices from the agent.
	* @param amount A positive double number indicating the amount to be subtracted
	**/
	void subSpices(double amount);
	/**
	* Credit rule; Returns the amount of sugar this agent is able to lend; else returns 0.
	**/
	int canLend();
	/**
	* Credit rule; Returns boolean indicating if this agent is a potential borrower.
	**/
	bool wantsToBorrow();
	/**
	* Add loan to the agent's list of taken loans.
	* @param loan A loan object.
	**/
	void takesLoan(Loan l);
	/**
	* Add loan to the agent's list of given loans.
	* @param loan A loan object.
	**/
	void givesLoan(Loan l);
	/**
	* Remove loan from agent's list of given loans.
	* @param loan A loan object.
	**/
	void removeLoanGiven(Loan l);
	/**
	* Remove loan from agent's list of taken loans.
	* @param loan A loan object.
	**/
	void removeLoanTaken(Loan l);
	/**
	* Credit rule; Iterates over list of taken loans and pays any that are over due date.
	* @param time Amount of iterations passed since simulation's start.
	**/
	void payDebts(int time);
	/**
	* Inheritance rule; Splits the wealth among all of the living children.
	* @param agent a pointer to an array of pointers to all the living agents
	* Iterates over children list and searches for this child in the agent array. If a children is found it is given its share of parent's wealth.
	* 
	**/
	void leaveLegacy(Agent *[GRIDW*GRIDH]);
	int getId();
	/**
	* Combat rule; This agent will be marked as dead.
	* @param sugarTaken Optional Amount of sugar to be deducted from agent's wealth. Usefull when a combat rule and inheritance rule is specified.
	**/
	void kill(int sugarTaken=0);
	bool isDead();
	/**
	* Disease rule; Gets sick.
	* @param disease Disease object that is to be added to the agent's list of diseases
	**/
	void receiveDisease(Disease disease);
	/**
	* Trade rule; Returns agents welfare based on given amount of resources.
	* @param w1 Amount of sugar
	* @param w2 Amount of spices
	**/
	double welfare(double w1, double w2);
	/**
	* Trade rule; Returns agents MRS.
	* @param w1 Optional Amount of sugar to be traded
	* @param w2 Optional Amount of spices to be traded
	**/
	double getMRS(double sug=0, double spi=0);

};

/**
* A temporary struct for storing a possible location of a new site; Combat rule.
* This struct is used only in moveWCombat() method. A pointer to an agent to be killed and the sugar to be deducted is stored here.
**/
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