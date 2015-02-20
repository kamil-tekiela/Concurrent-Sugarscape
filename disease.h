#pragma once

#include "settings.h"


struct Disease
{
	bool bitSet[MAXDISEASELEN];
	int randMax;
	int id;
	Disease(void);
	void affected(Disease &affector);
	int numberOfBitsSet(void);
	bool operator== (Disease &d2);
	bool operator!= (Disease &d2);
};

