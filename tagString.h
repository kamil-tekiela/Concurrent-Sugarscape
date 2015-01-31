#pragma once
#include <algorithm>
#include <iostream>

#define TAGCOUNT 11

using namespace std;

struct TagString
{
	bool tags[TAGCOUNT];
	TagString(void);
	TagString(int);
	int getGroup();
	void setFromParents(TagString &a, TagString &b);
	void affected(TagString &affector);
};

