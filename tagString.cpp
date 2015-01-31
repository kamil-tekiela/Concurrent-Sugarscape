#include "tagString.h"


TagString::TagString(void)
{
	for(int i=0;i<TAGCOUNT;i++){
		tags[i] = (rand()%2) != 0;
	}
}

TagString::TagString(int)
{
	NULL;
}

int TagString::getGroup(void)
{
	float ret=0;
	for(int i=0;i<TAGCOUNT;i++){
		ret+= i[tags];
	}
	return (int)((ret/TAGCOUNT)+0.5);
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


