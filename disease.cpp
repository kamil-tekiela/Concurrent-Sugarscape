#include "disease.h"


Disease::Disease(void)
{
	randMax = (rand() % MAXDISEASELEN)+1;	//1 to MAXDISEASELEN inclusive
	for(int i=0;i<randMax;i++)		bitSet[i] = (rand()%2) != 0;
	for(int i=0;i<randMax;i++)		id += 1<<i;
}

bool Disease::operator== (Disease &d2){
	return this->id == d2.id;
}

bool Disease::operator!= (Disease &d2){
	return this->id != d2.id;
}
