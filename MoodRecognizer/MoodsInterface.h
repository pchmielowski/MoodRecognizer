#pragma once;
#include "Types.h"

class FileReaderInterface;

class MoodsInterface {
public:
	MoodsInterface() {};
	MoodsInterface(FileReaderInterface& fileReader, const FileName moodsFileName) {};
	virtual Mood getNextMood() = 0;
	virtual void addMood(Mood mood) = 0;
	virtual ~MoodsInterface() {}
};
