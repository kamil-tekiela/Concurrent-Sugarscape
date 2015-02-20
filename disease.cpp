#include "disease.h"


Disease::Disease(void)
{
	randMax = (rand() % MAXDISEASELEN)+1;	//1 to MAXDISEASELEN inclusive
	for(int i=0;i<randMax;i++)		bitSet[i] = (rand()%2) != 0;
	for(int i=0;i<randMax;i++)		id += 1<<i;
}

int Disease::numberOfBitsSet(void)
{
	int ret=0;
	for(int i=0;i<randMax;++i)		ret+= i[bitSet];
	return ret;
}

void Disease::affected(Disease &affector){
	/*int tag = rand()%IMMUNELENGTH;
	this->bitSet[tag] = affector.bitSet[tag];*/
}

bool Disease::operator== (Disease &d2){
	return this->id == d2.id;
}

bool Disease::operator!= (Disease &d2){
	return this->id != d2.id;
}
