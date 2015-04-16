#pragma once

#include "settings.h"
#include "disease.h"

/**
* Disease rule; Immune system tag string. 
* Just like cultural string this struct bears many similarities.
* Interacts with the disease struct.
**/
struct ImmuneSys
{
	bool bitSet[IMMUNELENGTH];
	ImmuneSys(void);
	void setFromParents(const ImmuneSys &a, const ImmuneSys &b);
	bool affected(Disease &d);
	bool isSubstring(Disease &dis);
	int numberOfBitsSet(void);
};

