#pragma once

#include "settings.h"
#include "disease.h"


struct ImmuneSys
{
	bool bitSet[IMMUNELENGTH];
	ImmuneSys(void);
	void setFromParents(const ImmuneSys &a, const ImmuneSys &b);
	bool affected(Disease &d);
	bool isSubstring(Disease &dis);
	int numberOfBitsSet(void);
};

