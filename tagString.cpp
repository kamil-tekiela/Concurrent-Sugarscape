#include "tagString.h"


TagString::TagString(void)
{
	for(int i=0;i<TAGCOUNT;i++)		tags[i] = (rand()%2) != 0;
}

int TagString::getGroup(void)
{
	int ret=0;
	for(int i=0;i<TAGCOUNT;++i)		ret+= i[tags];
	return static_cast<int>((1.0*ret/TAGCOUNT)+0.5);	//assuming there exists only 2 groups
}

int TagString::numberOfBitsSet(void)
{
	int ret=0;
	for(int i=0;i<TAGCOUNT;++i)		ret+= i[tags];
	return ret;
}

void TagString::setFromParents(TagString &a, TagString &b)
{
	for(int i=0;i<TAGCOUNT;i++){
		if(a.tags[i] == b.tags[i]) this->tags[i] = a.tags[i];
		else this->tags[i] = (rand()%2) != 0;
	}
}

void TagString::affected(TagString &affector){
	int tag = rand()%TAGCOUNT;
	this->tags[tag] = affector.tags[tag];
}


