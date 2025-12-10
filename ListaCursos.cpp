#include "ListaCursos.h"

ListaCursos::ListaCursos()
{
	cabecera = final = nullptr;
	tam = 0;
}
ListaCursos::ListaCursos(const ListaCursos& other) {
	this->cabecera = this->final = this->nodo = nullptr;
	this->tam = 0;
	Nodo* temp = other.cabecera;
	while (temp != nullptr) {
		Course* nuevoCurso = new Course(*(temp->curso));
		this->Insertar(nuevoCurso);
		temp = temp->sig;
	}
}
ListaCursos& ListaCursos::operator=(const ListaCursos& other) {
	if (this == &other) {
		return *this;
	}
	this->clear(true);
	Nodo* temp = other.cabecera;
	while (temp != nullptr) {
		Course* nuevoCurso = new Course(*(temp->curso));
		this->Insertar(nuevoCurso);
		temp = temp->sig;
	}
	return *this;
}
void ListaCursos::clear(bool deleteCourses) {
	nodo = cabecera;
	Nodo* siguiente;
	while (nodo != nullptr) {
		siguiente = nodo->sig;
		if (deleteCourses) {
			delete nodo->curso;
		}
		delete nodo;
		nodo = siguiente;
	}
	cabecera = final = nullptr;
	tam = 0;
}
ListaCursos::~ListaCursos() {
	this->clear(true); 
}
void ListaCursos::Insertar(Course* nuevoCurso)
{
	Nodo* nuevo = new Nodo;
	nuevo->curso = nuevoCurso;
	nuevo->ant = nullptr;
	nuevo->sig = nullptr;

	//La lista está vacía
	if (cabecera == nullptr) {
		cabecera = final = nuevo;
		tam++;
		return;
	}

	//La lista no está vacía. Encontrar la posición correcta. Ordenado por ID Alfabéticamente
	std::string nuevoId = nuevoCurso->getId();
	nodo = cabecera;
	//Avanzar mientras el id del nodo actual sea menor que el id del nuevo curso
	while (nodo != nullptr && nodo->curso->getId() < nuevoId) {
		nodo = nodo->sig;
	}
	if (nodo == cabecera) { //Insertar al inicio
		nuevo->sig = cabecera;
		cabecera->ant = nuevo;
		cabecera = nuevo;
	}
	else if (nodo == nullptr) { //Insertar al final
		nuevo->ant = final;
		final->sig = nuevo;
		final = nuevo;
	}
	else { //Insertar en el medio
		nuevo->sig = nodo;
		nuevo->ant = nodo->ant;
		nodo->ant->sig = nuevo;
		nodo->ant = nuevo;
	}
	tam++;
}
int ListaCursos::getTam()
{
	return tam;
}
Course* ListaCursos::Extraer(std::string id)
{
	if (cabecera == nullptr) {
		return nullptr;
	}

	nodo = cabecera;

	while (nodo != nullptr) {
		if (nodo->curso->getId() == id) {
			Course* extraido = nodo->curso;
			if (nodo == cabecera && nodo == final) {
				cabecera = final = nullptr;
			}
			else if (nodo == cabecera) {
				cabecera = nodo->sig;
				cabecera->ant = nullptr;
			}
			else if (nodo == final) {
				final = nodo->ant;
				final->sig = nullptr;
			}
			else {
				nodo->ant->sig = nodo->sig;
				nodo->sig->ant = nodo->ant;
			}
			delete nodo;
			tam--;
			return extraido;
		}
		nodo = nodo->sig;
	}
	return nullptr;
}
Course* ListaCursos::findCourseById(std::string id) {
	if (cabecera == nullptr) {
		return nullptr;
	}
	nodo = cabecera;
	while (nodo != nullptr) {
		if (nodo->curso->getId() == id) {
			return nodo->curso;
		}
		nodo = nodo->sig;
	}
	return nullptr;
}
void ListaCursos::Mostrar()
{
	if (cabecera == nullptr) {
		std::cout << "\tNo hay clases en el horario.\n";
		return;
	}

	std::cout << "\n----- Horario de Clases -----\n";

	nodo = cabecera;
	int contador = 1;
	while (nodo != nullptr) {
		std::cout << "\nCurso " << contador++ << ":\n";
		nodo->curso->showCourseInfo();
		nodo = nodo->sig;
	}
	std::cout << "\n------------------------------\n";
}
Nodo* ListaCursos::getCabecera() const
{
	return cabecera;
}