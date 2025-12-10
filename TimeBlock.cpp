#include "TimeBlock.h"
#include<sstream>
#include <iostream>
//CONSTRUCTOR Y DESTRUCTOR
TimeBlock::TimeBlock() {
	this->courseId = ""; 
	this->dayOfWeek = MON;
	this->startTime = 0;
	this->endTime = 0;
}
TimeBlock::TimeBlock(Day day, unsigned int start, unsigned int end, std::string courseId) { //Constructor del bloque de tiempo
	this->dayOfWeek = day;
	this->startTime = start;
	this->endTime = end;
	this->courseId = courseId;
};
TimeBlock::~TimeBlock() {}

//MOSTRAR 
void TimeBlock::showTimeBlockInfo() {//	Mostrar la info del bloque de tiempo (Se puede quitar)

	std::cout << "\t" << TimeBlock::dayToString(this->dayOfWeek) << "\n\tID:" << this->courseId << "\n";
	std::cout << "\tStart Time: " << minutesToTime(this->startTime) << "\n";
	std::cout << "\tEnd Time: " << minutesToTime(this->endTime) << "\n";
}

//MÉTODOS ADICIONALES
bool TimeBlock::conflictsWith(const TimeBlock& other) const { //Para determinar si un bloque de tiempo se solapa con otro
	if (this->dayOfWeek != other.dayOfWeek) {
		return false;
	}
	return !(this->endTime <= other.startTime || this->startTime >= other.endTime);
}
TimeBlock::Day TimeBlock::getDayOfWeek() const{
	return this->dayOfWeek;
}
int TimeBlock::getStartTime() const {
	return this->startTime;
}
int TimeBlock::getEndTime() const {
	return this->endTime;
}

//MÉTODOS ESTÁTICOS DE CONVERSIÓN
unsigned int TimeBlock::timeToMinutes(const std::string& timeStr) { //Método estático para convertir una cadena de tiempo en minutos desde la medianoche para facilitar la comparación entre horas
	if (timeStr.length() != 5 || timeStr[2] != ':') {
		throw std::invalid_argument("Invalid time format. Expected HH:MM");
	}//10:00
	try {
		unsigned int hours = std::stoi(timeStr.substr(0, 2));
		unsigned int minutes = std::stoi(timeStr.substr(3, 2));//PORQUE HICE ESTO!!!!!!!

		if (hours < 0 || hours>23 || minutes < 0 || minutes>59) {
			throw std::invalid_argument("Invalid time value.");
		}
		return (hours * 60) + minutes;
	}
	catch (const std::exception& e) {
		throw std::invalid_argument("Invalid time format. Expected HH:MM");
	}
}
std::string TimeBlock::minutesToTime(unsigned int minutos) { //Método estático para convertir minutos desde la medianoche a una cadena de tiempo en formato HH:MM
	std::string time;
	unsigned int hours = minutos / 60;
	unsigned int minutes = minutos % 60;

	if (hours > 23)hours %= 24; 

	if (hours < 10) {
		time += "0" + std::to_string(hours) + ":";
		if (minutes < 10) {
			time += "0" + std::to_string(minutes);
		}
		else {
			time += std::to_string(minutes);
		}
	}
	else {
		time += std::to_string(hours)+":";
		if (minutes < 10) {
			time += "0" + std::to_string(minutes);
		}
		else {
			time += std::to_string(minutes);
		}
	}
	return time;
}
std::string TimeBlock::dayToString(Day day) {//Método estático para convertir el día enumerado a una cadena legible
	std::string days[]{ "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
	return days[day];
}