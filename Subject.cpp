#include "Subject.h"
Subject::Subject(std::string name) {
	this->name = name;
}
void Subject::addOption(Course* courseOption){
	this->options.push_back(courseOption);
}
const std::vector <Course*>& Subject::getOptions() const{
	return this->options;
}
std::string Subject::getName() const{
	return this->name;
}