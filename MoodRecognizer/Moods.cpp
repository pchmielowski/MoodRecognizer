#include "Moods.h"
#include "FileReaderInterface.h"
#include <string>
#include <sstream>

using namespace std;

Moods::Moods()
{

}

Moods::Moods(FileReaderInterface& fileReader, const FileName moodsFileName)
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

void Moods::parseFile() {
	istringstream content;
	content.str(fileReader_->getContent());
	string moodAsString;
	while (getline(content, moodAsString)) {
		moodsQueue_.push(atoi(moodAsString.c_str()));
	}
}

Mood Moods::getNextMood()
{
	try {
		Mood nextMood = moodsQueue_.front();
		moodsQueue_.pop();
		return nextMood;
	}
	catch (...) {
		throw std::runtime_error("Queue is empty");
	}
}
