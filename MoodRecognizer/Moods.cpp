#include "Moods.h"
#include "FileReaderInterface.h"

Moods::Moods()
{

}

Moods::Moods(FileReaderInterface& fileReader, const FileName moodsFileName)
{
	fileReader_ = &fileReader;
}

void Moods::addMood(Mood mood)
{
	moodsQueue_.push(mood);
}

Moods::~Moods()
{

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
