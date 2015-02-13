#pragma once

#include "settings.h"


struct TagString
{
	bool tags[TAGCOUNT];
	TagString(void);
	TagString(int);
	int getGroup();
	void setFromParents(TagString &a, TagString &b);
	void affected(TagString &affector);
};

