#include "Scheduler.h"
#include "Colors.h"
#include <iostream>
#include <limits>
Scheduler::Scheduler(){}
Scheduler::~Scheduler() {
	this->finalSchedule.clear(false);//Primero borra el finalSchedule sin borrar los cursos
}
Scheduler::Scheduler(const Scheduler& other)
	:conflictDetector(other.conflictDetector) {
	Nodo* tempCatalog = other.courseCatalog.getCabecera();
	while (tempCatalog != nullptr) {
		Course* newCourse = new Course(*tempCatalog->curso);
		this->courseCatalog.Insertar(newCourse);
		tempCatalog = tempCatalog->sig;
	}

	Nodo* tempSchedule = other.finalSchedule.getCabecera();
	while (tempSchedule != nullptr) {
		std::string id = tempSchedule->curso->getId();
		Course* ptrEnMiCatalogo = this->courseCatalog.findCourseById(id);

		
		if (ptrEnMiCatalogo != nullptr) {
			this->finalSchedule.Insertar(ptrEnMiCatalogo);
		}
		tempSchedule = tempSchedule->sig;
	}
}
Scheduler& Scheduler::operator=(const Scheduler& other) {
	if (this == &other)return *this;
	this->conflictDetector = other.conflictDetector; //Copiar el ABB
	this->finalSchedule.clear(false); //Limpiar el horario final sin borrar los cursos
	this->courseCatalog.clear(true); //Limpiar el catálogo de cursos y borrar los cursos
	Nodo* tempCatalog = other.courseCatalog.getCabecera();
	while (tempCatalog != nullptr) {//Copiar el catálogo de cursos
		Course* newCourse = new Course(*tempCatalog->curso);
		this->courseCatalog.Insertar(newCourse);
		tempCatalog = tempCatalog->sig;
	}
	Nodo* tempSchedule = other.finalSchedule.getCabecera();//Copiar el horario final
	while (tempSchedule != nullptr) {
		std::string id = tempSchedule->curso->getId();
		Course* ptrEnMiCatalogo = this->courseCatalog.findCourseById(id);

		if (ptrEnMiCatalogo != nullptr) {
			this->finalSchedule.Insertar(ptrEnMiCatalogo);
		}
		tempSchedule = tempSchedule->sig;
	}
	return *this;//Retornar el objeto actual
}
//MÉTODOS DE AYUDA PRIVADOS (LO RUDO)
bool Scheduler::checkAllBlocks(Course& course) {
	TimeBlock* block; 
	for (unsigned short i = 0; i < course.getTimeBlocksCount(); i++) {
		block = course.getTimeBlock(i);//Obtiene el bloque de tiempo i-ésimo del curso
		if (block == nullptr)continue;
		if (conflictDetector.checkConflict(*block, conflictDetector.getRoot())) {//Verifica si hay conflicto en el ABB
			return false;//Si hay por lo menos un conflicto, retorna false
		}
	}
	return true; 
}
void Scheduler::insertAllBlocksToABB(Course& course) {
	TimeBlock* block; 
	for (unsigned short i = 0; i < course.getTimeBlocksCount(); i++) {
		block = course.getTimeBlock(i);//Obtiene el bloque de tiempo i-ésimo del curso
		if (block == nullptr)continue;
		conflictDetector.crearNodo(*block);//Crea un nodo con el bloque de tiempo
		conflictDetector.insert(conflictDetector.getRoot());//Inserta el nodo en el ABB
	}
}
void Scheduler::removeAllBlocksFromABB(Course& course) {
	TimeBlock* block;
	for (unsigned short i = 0; i < course.getTimeBlocksCount(); i++) {
		block = course.getTimeBlock(i);//Obtiene el bloque de tiempo i-ésimo del curso
		if (block == nullptr)continue;
		conflictDetector.remove(*block);//Elimina el bloque de tiempo del ABB
	}
}
Course* Scheduler::findCourseInCatalog(const std::string& courseId) { //POR IMPLEMENTAR
	//return courseCatalog.findCourseById(courseId);
	return courseCatalog.findCourseById(courseId);
}
Course* Scheduler::findCourseInSchedule(const std::string& courseId) {
	return finalSchedule.findCourseById(courseId);
}
//MÉTODOS PÚBLICOS (BONITOS)
bool Scheduler::addCourseToSchedule(const std::string& courseId) {
	undoStack.Insertar(this); //Guardar estado actual antes de hacer cambios
	Course* courseToAdd = findCourseInCatalog(courseId);//Buscar el curso en el catálogo
	if (courseToAdd == nullptr) { //Si no se encuentra el curso
		std::cout << "\n" << Color::RED << Color::BOLD << "[Scheduler ERROR]" << Color::RESET << Color::RED
			<< " No se encontro el curso con ID: '" << courseId << "' en el catalogo." << Color::RESET << "\n";
		return false;
	}
	std::cout << "\n" << Color::CYAN << "[Scheduler INFO]" << Color::RESET << " Verificando materia: "
		<< Color::BOLD << courseToAdd->getName() << Color::RESET << "...\n";
	if (checkAllBlocks(*courseToAdd) == false) {//Verificar si hay conflictos mandando cada bloque de tiempo al ABB
		std::cout << Color::YELLOW << "[Scheduler RECHAZADO]" << Color::RESET << " " << courseToAdd->getName()
			<< " no se puede inscribir debido a un conflicto.\n";
		return false;
	}
	std::cout << Color::CYAN << "[Scheduler INFO]" << Color::RESET << " Sin conflictos. Inscribiendo "
		<< Color::BOLD << courseToAdd->getName() << Color::RESET << ".\n";
	insertAllBlocksToABB(*courseToAdd);//Insertar todos los bloques de tiempo del curso en el ABB
	finalSchedule.Insertar(courseToAdd);//Insertar el curso en el horario final
	
	std::cout << Color::GREEN << Color::BOLD << "[Scheduler EXITO]" << Color::RESET << Color::GREEN
		<< " Curso '" << courseToAdd->getName() << "' inscrito correctamente." << Color::RESET << "\n";
	return true;
}
bool Scheduler::removeCourseFromSchedule(const std::string& courseId) {
	undoStack.Insertar(this);//Guardar estado actual antes de hacer cambios
	Course* courseToRemove = this->findCourseInSchedule(courseId);//Buscar el curso en el horario final
	if (courseToRemove == nullptr) { //Si no se encuentra el curso
		std::cout << "\n" << Color::RED << Color::BOLD << "[Scheduler ERROR]" << Color::RESET << Color::RED
			<< " No se encontro el curso con ID: '" << courseId
			<< "' en el horario inscrito." << Color::RESET << "\n";
		return false;
	}
	std::cout << "\n" << Color::CYAN << "[Scheduler INFO]" << Color::RESET << " Eliminando materia: "
		<< Color::BOLD << courseToRemove->getName() << Color::RESET << "...\n";

	removeAllBlocksFromABB(*courseToRemove);//Eliminar todos los bloques de tiempo del curso del ABB

	Course* extractedPtr = finalSchedule.Extraer(courseId);//Eliminar el curso del horario final
	if (extractedPtr==nullptr) {
		std::cout << "\n" << Color::RED << Color::BOLD << " [Scheduler error] No se pudo eliminar el curso del horario.\n" << Color::RESET;
		return false;
	}
	std::cout << Color::GREEN << Color::BOLD << "[Scheduler EXITO]" << Color::RESET << Color::GREEN
		<< " Curso '" << courseToRemove->getName() << "' eliminado correctamente." << Color::RESET << "\n";
	return true;
}
void Scheduler::createCourseInCatalog() {
	std::cout << "\n" << Color::BOLD << Color::YELLOW
		<< "--- CREAR NUEVO CURSO EN EL CATALOGO ---\n" << Color::RESET;
	Course* newCourse= new Course(); 
	newCourse->setIdNameProf();
	unsigned short timeBlockCount;
	std::cout << Color::CYAN << "Ingrese la cantidad de bloques de tiempo para "
		<< newCourse->getName() << ": " << Color::RESET;

	while (!(std::cin >> timeBlockCount) || timeBlockCount == 0) {
		std::cout << "\t" << Color::RED << "Entrada invalida. Por favor ingrese un numero: " << Color::RESET;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	newCourse->initalizeTimeBlocks(timeBlockCount);
	courseCatalog.Insertar(newCourse);
	std::cout << "\n" << Color::GREEN << Color::BOLD << "[Scheduler EXITO]" << Color::RESET << Color::GREEN
		<< " Curso '" << newCourse->getName()
		<< "' ha sido creado en el catalogo." << Color::RESET << "\n";
}
void Scheduler::displayStudentSchedule() {
	std::cout << "\n" << Color::BOLD << Color::YELLOW
		<< "--- HORARIO FINAL INSCRITO ---\n" << Color::RESET;
	if (this->finalSchedule.getTam() == 0) {
		std::cout << "\t" << Color::YELLOW << "(Horario Vacio)\n" << Color::RESET;
		return;
	}
	this->finalSchedule.Mostrar();
}
void Scheduler::displayCourseCatalog(){
	std::cout << "\n" << Color::BOLD << Color::YELLOW
		<< "--- CATALOGO DE CURSOS DISPONIBLES ---\n" << Color::RESET;
	
	if (this->courseCatalog.getTam() == 0) {
		std::cout << "\t" << Color::YELLOW << "(Catalogo Vacio)\n" << Color::RESET;
		return;
	}

	this->courseCatalog.Mostrar();
}
void Scheduler::debugDisplayABB() {
	std::cout << "\n" << Color::BOLD << Color::YELLOW
		<< "--- (DEBUG) ESTADO DEL ARBOL DE CONFLICTOS (ABB) ---\n" << Color::RESET;
	if (conflictDetector.isEmpty()) {
		std::cout << Color::YELLOW << "\t(Arbol Vacio)\n" << Color::RESET;
		return;
	}
	conflictDetector.inOrder(conflictDetector.getRoot());
}
void Scheduler::undoLastAction() {
	std::cout << "\n" << Color::CYAN << "[Scheduler INFO]" << Color::RESET
		<< " Deshaciendo ultima accion...\n";
	Scheduler* estadoAnterior = undoStack.Extraer();
	if (estadoAnterior == nullptr) {
		std::cout << Color::YELLOW << "\tNo hay mas acciones que deshacer.\n" << Color::RESET;
		return;
	}
	*this = *estadoAnterior;//Usar el operador de asignación sobrecargado para restaurar el estado anterior
	delete estadoAnterior; //Llama a los destructores en el orden apropiado
	std::cout << Color::GREEN << "\t¡Accion deshecha!\n" << Color::RESET;
}