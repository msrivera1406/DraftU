#pragma once
#include "TimeBlock.h"
#include <string>
#include <vector>
#include <locale.h>

class Course
{
private: 
	std::string id;  //ID del curso
	std::string name; //Nombre del curso
	std::string professor; //Nombre del profesor
	TimeBlock* timeblocks;  //Array dinámico de bloques de tiempo
	unsigned short timeBlocksCount; //Cantidad de timeBlocks de la clase
public: 
	//Constructor y Destructor
	Course(); 
	Course(const Course& other); 
	Course& operator=(const Course& other);
	~Course(); 

	//Setters
	void setIdNameProf();
	void initalizeTimeBlocks(unsigned short);
	void manualSetup(std::string id, std::string name, std::string professor, unsigned short count, const std::vector<TimeBlock>& blocks);

	//Getters
	unsigned short getTimeBlocksCount();
	std::string getId();
	std::string getName();
	std::string getProfessor();

	//Métodos adicionales
	TimeBlock* getTimeBlock(unsigned int short); 
	void showCourseInfo();
};