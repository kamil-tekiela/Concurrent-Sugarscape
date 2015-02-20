#pragma once

#include "settings.h"


struct TagString
{
	bool tags[TAGCOUNT];
	TagString(void);
	int getGroup();
	void setFromParents(TagString &a, TagString &b);
	void affected(TagString &affector);
	int numberOfBitsSet(void);
};

