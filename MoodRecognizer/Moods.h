#pragma once;
#include "Types.h"
#include "MoodsInterface.h"
#include <queue>

class FileReaderInterface;

class Moods : public MoodsInterface {
public:
	Moods();
	Moods(FileReaderInterface& fileReader, const FileName moodsFileName);
	Mood getNextMood() override;
	void addMood(Mood mood) override;
	~Moods();
private:
	FileReaderInterface* fileReader_;
	std::queue<Mood> moodsQueue_;
};
