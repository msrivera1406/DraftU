#include "Course.h"
#include <iostream>
#include <limits>
#include <vector>

//Constructor y Destructor
Course::Course() {
	this->id = "";
	this->name = "";
	this->professor = ""; 
	this->timeblocks = nullptr;
	this->timeBlocksCount = 0;
}
Course::~Course(){
	delete[] this->timeblocks;
}
Course::Course(const Course& other) {
	this->id = other.id;
	this->name = other.name;
	this->professor = other.professor;
	this->timeBlocksCount = other.timeBlocksCount;
	if (other.timeblocks != nullptr && other.timeBlocksCount > 0) {
		this->timeblocks = new TimeBlock[other.timeBlocksCount];
		for (unsigned short i = 0; i < other.timeBlocksCount; i++) {
			this->timeblocks[i] = other.timeblocks[i];
		}
	}
	else {
		this->timeblocks = nullptr;
	}
}
Course& Course::operator=(const Course& other) {
	if (this == &other) {
		return *this;
	}
	delete[] this->timeblocks;
	this->id = other.id;
	this->name = other.name;
	this->professor = other.professor;
	this->timeBlocksCount = other.timeBlocksCount;
	if (other.timeblocks != nullptr && other.timeBlocksCount > 0) {
		this->timeblocks = new TimeBlock[other.timeBlocksCount];
		for (unsigned short i = 0; i < other.timeBlocksCount; i++) {
			this->timeblocks[i] = other.timeblocks[i];
		}
	}
	else {
		this->timeblocks = nullptr;
	}
	return *this;
}
//Setters
void Course::setIdNameProf(){
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Ingrese el ID del curso: ";
	std::getline(std::cin, this->id);
	std::cout << "Ingrese el nombre del curso: ";
	std::getline(std::cin, this->name);
	std::cout << "Ingrese el nombre del profesor: ";
	std::getline(std::cin, this->professor);
}
void Course::initalizeTimeBlocks(unsigned short count){
	std::setlocale(LC_ALL, "");

	this->timeBlocksCount = count;
	this->timeblocks = new TimeBlock[count];
	int dayIndex = -1; 
	std::string starTime, endTime;
	unsigned short startMinutes, endMinutes;

	for (unsigned short i = 0; i < count; i++) {
		std::cout << "Inicializando Bloque " << i+1 << " de " << this->timeBlocksCount<<" para " << this->name <<"\n";
		dayIndex = -1;
		while (dayIndex < 0 || dayIndex >= TimeBlock::DAY_COUNT) {
			std::cout << "\tSeleccione el día de su clase (0-"<<TimeBlock::DAY_COUNT-1<<"):\n";
			for (unsigned int short d = 0; d < TimeBlock::DAY_COUNT; d++) {
				std::cout << "\t\t" << d << ": " << TimeBlock::dayToString(static_cast<TimeBlock::Day>(d)) << "\n";
			}
			std::cout << "\tOpción: ";
			if (!(std::cin >> dayIndex)) {
				std::cout << "\tEntrada inválida. Intente de nuevo.\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				dayIndex = -1; 
			}
			else if(dayIndex < 0 || dayIndex >= TimeBlock::DAY_COUNT){
					std::cout << "\tOpción fuera de rango. Intente de nuevo.\n";
			}
		}
		std::cout << "\tIngrese la hora de inicio (formato HH:MM): ";
		std::cin >> starTime;
		std::cout << "\tIngrese la hora de fin (formato HH:MM): ";
		std::cin >> endTime;

		try {
			startMinutes = TimeBlock::timeToMinutes(starTime);
			endMinutes = TimeBlock::timeToMinutes(endTime);
			if (startMinutes >= endMinutes) {
				throw std::invalid_argument("La hora de inicio debe ser menor que la hora de fin.");
			}
			this->timeblocks[i] = TimeBlock(static_cast<TimeBlock::Day>(dayIndex), startMinutes, endMinutes, this->id);
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		catch (const std::invalid_argument& e) {
			std::cout << "\tError: " << e.what() << ". Intente de nuevo.\n";
			i--;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	std::cout << "\tInicialización de bloques de tiempo completada para " << this->name << ".\n";
}
void Course::manualSetup(std::string id, std::string name, std::string professor, unsigned short count, const std::vector<TimeBlock>& blocks) {
	this->id = id;
	this->name = name;
	this->professor = professor;
	if (this->timeblocks != nullptr) {
		delete[] this->timeblocks; 
	}
	this->timeBlocksCount = count;
	this->timeblocks = new TimeBlock[count];
	for (unsigned short i = 0; i < count; i++) {
		this->timeblocks[i] = blocks[i];
	}
}

//Getters
unsigned short Course::getTimeBlocksCount(){
	return this->timeBlocksCount;
}
std::string Course::getId(){
	return this->id;
}
std::string Course::getName(){
	return this->name;
}
std::string Course::getProfessor() {
	return this->professor;
}

//Métodos adicionales
TimeBlock* Course::getTimeBlock(unsigned int short idx){
	if (idx >= this->timeBlocksCount) {
		return nullptr;
	}
	return &this->timeblocks[idx];
}
void Course::showCourseInfo() {
	std::setlocale(LC_ALL, "");
	std::cout << "--------------\n"; 
	std::cout<<this->getId();
	std::cout << " - " << this->getName() << "\n";
	std::cout << "Profesor: " << this->getProfessor() << "\n";
	for (unsigned short i = 0; i < this->timeBlocksCount; i++) {
		this->timeblocks[i].showTimeBlockInfo();
	}
}


//[MON, TUE, WED]-> Array dinámico
