#include "loan.h"


Loan::Loan(Agent* lender, Agent* borrower, int amount, int date)
{
	this->amount =		amount;
	this->lender =		lender;
	this->borrower =	borrower;
	this->takenOn =		date;
}

int Loan::toRepay(){
	return amount+(amount*CREDITRATE/100);
}


bool Loan::operator==(Loan loan){
	return	this->amount == loan.amount &&
			this->lender == loan.lender &&
			this->borrower == loan.borrower &&
			this->takenOn == loan.takenOn;
}
