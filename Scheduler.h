#pragma once
#include "ABB.h"
#include "Course.h"
#include "ListaCursos.h"
#include "PilaScheduler.h"
#include <string>
class Scheduler
{
public: 
	Scheduler(); 
	~Scheduler();
	Scheduler(const Scheduler& other); 
	Scheduler& operator=(const Scheduler& other);
	void createCourseInCatalog(); 
	void displayCourseCatalog(); 
	bool addCourseToSchedule(const std::string& courseId);
	bool removeCourseFromSchedule(const std::string& courseId);
	void displayStudentSchedule();
	void undoLastAction();
	void debugDisplayABB(); 
private:
	ABB conflictDetector; 
	ListaCursos finalSchedule; 
	ListaCursos courseCatalog; 
	//courseCatalog es el dueño, finalSchedule toma prestado los punteros del catálogo de cursos
	PilaScheduler undoStack; 

	Course* findCourseInCatalog(const std::string& courseId);
	Course* findCourseInSchedule(const std::string& courseId);

	bool checkAllBlocks(Course& course); 
	void insertAllBlocksToABB(Course& course);
	void removeAllBlocksFromABB(Course& course);
}; 

