#pragma once;
#include "Types.h"

class MoodsInterface {
public:
	MoodsInterface() {};
	MoodsInterface(FileName moodsFileName) {};
	virtual Mood getNextMood() = 0;
	virtual void addMood(Mood mood) = 0;
	virtual ~MoodsInterface() {}
};
