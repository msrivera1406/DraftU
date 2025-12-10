#include "SchedulerGenerator.h"
std::vector<std::vector<Course*>> SchedulerGenerator::generateSchedules(const std::vector<Subject>& subjectsToTake) {
	std::vector<std::vector<Course*>> validSchedules;
	std::vector <Course*> currentSchedule;
	ABB conflictDetector;
	backtrack(0, subjectsToTake, currentSchedule, conflictDetector, validSchedules);
	return validSchedules; 
}
void SchedulerGenerator::backtrack(size_t subjectIndex,
	const std::vector<Subject>& subjects,
	std::vector<Course*>& currentSchedule,
	ABB& conflictDetector,
	std::vector<std::vector<Course*>>& validSchedules) {

	//Caso Base
	if (subjectIndex == subjects.size()) {
		validSchedules.push_back(currentSchedule);
		return;
	}
	//PASO RECURSIVO
	const Subject& currentSubject = subjects[subjectIndex];
	const std::vector<Course*>& options = currentSubject.getOptions();

	bool hasConflict; 
	for (Course* courseOption : options) {
		hasConflict = false;
		for (unsigned short i = 0; i < courseOption->getTimeBlocksCount(); i++) {
			TimeBlock* block = courseOption->getTimeBlock(i);
			if (conflictDetector.checkConflict(*block, conflictDetector.getRoot())) {
				hasConflict = true;
				break;
			}
		}
		if (!hasConflict) {
			//Añadir al horario temporal
			currentSchedule.push_back(courseOption);
			//Añadir al ABB 
			for (unsigned short i = 0; i < courseOption->getTimeBlocksCount(); i++) {
				TimeBlock* block = courseOption->getTimeBlock(i);
				conflictDetector.crearNodo(*block);
				conflictDetector.insert(conflictDetector.getRoot());
			}
			//Llamada recursiva
			backtrack(subjectIndex + 1, subjects, currentSchedule, conflictDetector, validSchedules);
			//Remover del horario temporal
			currentSchedule.pop_back();
			//Remover del ABB
			for (unsigned short i = 0; i < courseOption->getTimeBlocksCount(); i++) {
				TimeBlock* block = courseOption->getTimeBlock(i);
				conflictDetector.remove(*block);
			}

		}
	}
	
}