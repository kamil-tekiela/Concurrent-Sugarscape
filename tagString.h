#pragma once

#include "settings.h"

/**
* Struct defining tag strings for agents cultural tags.
* Each tag is made up of array of booleans. The amount of 1s vs 0s determines which group the agent belongs to. If 1s outnumber 0s then its group 1 and vice versa.
**/
struct TagString
{
	bool tags[TAGCOUNT];
	TagString(void);
	int getGroup();
	void setFromParents(TagString &a, TagString &b);
	void affected(TagString &affector);
	int numberOfBitsSet(void);
};

