#pragma once;
#include "Types.h"
#include "MoodsInterface.h"
#include <queue>

class FileReader;

class Moods : public MoodsInterface {
public:
	Moods();
	Moods(FileReader& fileReader, const FileName moodsFileName);
	Mood getNextMood() override;
	void addMood(Mood mood) override;
	~Moods();
private:
	FileReader* fileReader_;
	std::queue<Mood> moodsQueue_;
	void parseFile();
};
