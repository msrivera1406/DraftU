#pragma once
#include "Course.h"
#include <iostream>

struct Nodo {
	Nodo* ant;
	Course* curso;
	Nodo* sig;
};

class ListaCursos
{

public:
	ListaCursos();
	ListaCursos(const ListaCursos& other);
	ListaCursos& operator=(const ListaCursos& other);
	~ListaCursos();
	void Insertar(Course*);
	Course* Extraer(std::string);
	Course* findCourseById(std::string);
	void Mostrar();
	Nodo* getCabecera() const;
	int getTam();
	void clear(bool deleteCourses=true);
private:
	Nodo* cabecera, * final, * nodo;
	int tam;
	
};

