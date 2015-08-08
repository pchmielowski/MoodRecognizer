#pragma once;
#include "Types.h"

class FileReader;

class MoodsInterface {
public:
	MoodsInterface() {};
	MoodsInterface(FileReader& fileReader, const FileName moodsFileName) {};
	virtual Mood getNextMood() = 0;
	virtual void addMood(Mood mood) = 0;
	virtual ~MoodsInterface() {}
};
