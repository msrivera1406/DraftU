#pragma once
#include <vector>
#include "Subject.h"
#include "ABB.h"
class SchedulerGenerator
{
private:
	void backtrack(size_t subjectIndex, 
		const std::vector<Subject>& subjects,
		std::vector<Course*>& currentSchedule, 
		ABB& conflictDetector,
		std::vector<std::vector<Course*>>& validSchedules);
public: 
	std::vector<std::vector<Course*>> generateSchedules(const std::vector<Subject>& subjectsToTake);
};

