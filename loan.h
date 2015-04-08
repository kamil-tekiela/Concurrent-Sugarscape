#pragma once
#include "settings.h"

class Agent;

class Loan
{
public:
	Loan(Agent* lender, Agent* borrower, int amount, int date);
	int amount;
	Agent * lender;
	Agent * borrower;
	int takenOn;

	int toRepay();
	bool operator==(Loan loan);
};

