#include "Moods.h"
#include "FileReader.h"
#include <string>
#include <sstream>

using namespace std;

Moods::Moods()
{

}

Moods::Moods(FileReader& fileReader, const FileName moodsFileName)
{
	fileReader_ = &fileReader;
	fileReader_->open(moodsFileName);
	parseFile();
}

void Moods::addMood(Mood mood)
{
	moodsQueue_.push(mood);
}

Moods::~Moods()
{

}

void Moods::parseFile() 
{
	istringstream content;
	content.str(fileReader_->getContent());
	string moodAsString;
	while (getline(content, moodAsString)) {
		moodsQueue_.push(atoi(moodAsString.c_str()));
	}
}

Mood Moods::getNextMood()
{
	if (!moodsQueue_.empty()) {
		Mood nextMood = moodsQueue_.front();
		moodsQueue_.pop();
		return nextMood;
	}
	else {
		throw std::runtime_error("Moods queue is empty");
	}
}
