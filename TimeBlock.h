#pragma once
#include <string>
#include <locale.h>
class TimeBlock
{
public:
	enum Day {
		MON, TUE, WED, THU, FRI, SAT, SUN, DAY_COUNT};
	TimeBlock(Day, unsigned int, unsigned int, std::string);
	TimeBlock(); 
	~TimeBlock();
	bool conflictsWith(const TimeBlock&) const;
	void showTimeBlockInfo();
	static unsigned int timeToMinutes(const std::string&);
	static std::string minutesToTime(unsigned int);
	static std::string dayToString(Day); 
	Day getDayOfWeek() const;
	int getStartTime() const; 
	int getEndTime() const;

private:
	Day dayOfWeek;
	unsigned int startTime;
	unsigned int endTime;
	std::string courseId;
};