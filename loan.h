#pragma once
#include "settings.h"

class Agent;

/**
* Credit rule; Loan class.
* A container for a loan. Each loan needs a lender, borrower, amount of sugar borrower(int) and the timestamp it was taken on.
**/
class Loan
{
public:
	Loan(Agent* lender, Agent* borrower, int amount, int date);
	int amount;
	Agent * lender;
	Agent * borrower;
	int takenOn;
	
	/**
	* Return the amount to be payed back i.e. with credit rate applied to it.
	**/
	int toRepay();
	bool operator==(Loan loan);
};

