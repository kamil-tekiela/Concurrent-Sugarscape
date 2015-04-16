#pragma once

#include "settings.h"

/**
* Disease rule; Disease tag string. 
* Just like cultural string this struct bears many similarities.
* Needed for the immune system struct.
**/
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

