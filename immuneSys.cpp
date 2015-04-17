#include "immuneSys.h"


ImmuneSys::ImmuneSys(void)
{
	for(int i=0;i<IMMUNELENGTH;i++)		bitSet[i] = (rand()%2) != 0;
}

int ImmuneSys::numberOfBitsSet(void)
{
	int ret=0;
	for(int i=0;i<IMMUNELENGTH;++i)		ret+= i[bitSet];
	return ret;
}

void ImmuneSys::setFromParents(const ImmuneSys &a, const ImmuneSys &b)
{
	for(int i=0;i<IMMUNELENGTH;i++){
		if(a.bitSet[i] == b.bitSet[i]) this->bitSet[i] = a.bitSet[i];
		else this->bitSet[i] = (rand()%2) != 0;
	}
}

bool ImmuneSys::affected(Disease &d){
	int lenD = d.randMax;
	int hamm = lenD;
	int hammPos = 0;
	//look for the subset with the smallest hamming distance
	for(int i=0;i<IMMUNELENGTH;++i){
		int hammTemp = 0;
		if(IMMUNELENGTH-lenD < i) break;
		for(int j=0;j<lenD;++j){
			hammTemp += (bitSet[i+j] != d.bitSet[j]);
		}
		if(hammTemp<hamm){
			hamm = hammTemp;
			hammPos = i;
		}
	}
	//find the leftmost different bit
	for(int j=0;j<lenD;++j){
		if(bitSet[hammPos+j] != d.bitSet[j]){
			bitSet[hammPos+j] = d.bitSet[j];
			break;
		}
	}
	return isSubstring(d);
}

bool ImmuneSys::isSubstring(Disease &dis){
	int lenD = dis.randMax;
	for(int i=0;i<IMMUNELENGTH;++i){
		if((IMMUNELENGTH-lenD) < i) return false;
		bool match = true;
		for(int disi=0; disi<lenD; disi++){
			match &= (this->bitSet[i+disi] == dis.bitSet[disi]);
		}
		if(match) return true;
	}
	return false;
}


