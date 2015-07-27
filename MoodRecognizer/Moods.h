#pragma once;
#include "Types.h"

class Moods {
public:
	Moods();
	Moods(FileName moodsFileName);
	Mood getNextMood();
	void addMood(Mood mood);
};
