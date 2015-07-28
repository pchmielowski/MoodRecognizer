#pragma once;
#include "Types.h"
#include "MoodsInterface.h"

class Moods : public MoodsInterface {
public:
	Moods();
	Moods(FileName moodsFileName);
	Mood getNextMood() override;
	void addMood(Mood mood) override;
	~Moods();
};
