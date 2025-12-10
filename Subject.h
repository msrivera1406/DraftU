#pragma once
#include <string>
#include <vector>
#include "Course.h"
class Subject
{
private:
	std::string name; 
	std::vector<Course*> options; 
public: 
	Subject(std::string name); 
	void addOption(Course* courseOption); 
	const std::vector <Course*>& getOptions() const;
	std::string getName() const; 
};