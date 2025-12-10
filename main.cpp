#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <locale.h>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;
#include "Scheduler.h"
#include "SchedulerGenerator.h"
#include "Subject.h"
#include "Utils.h"
#include "Colors.h"

void displayMenu() {
	std::cout << Color::BOLD << "============================   BIENVENIDO A " << Color::RESET;
	std::cout << Color::BOLD << Color::YELLOW << "Node" << Color::RESET;
	std::cout << Color::BOLD << Color::MAGENTA << "Guard   " << Color::RESET;
	std::cout << Color::BOLD  << "============================\n" << Color::RESET;
	std::cout << Color::CYAN << "\t1." << Color::RESET << " Crear nuevo curso en el catalogo\n";
	std::cout << Color::CYAN << "\t2." << Color::RESET << " Ver catalogo de cursos\n";
	std::cout << Color::CYAN << "\t3." << Color::RESET << " Inscribir curso al horario\n";
	std::cout << Color::CYAN << "\t4." << Color::RESET << " Eliminar curso del horario\n";
	std::cout << Color::CYAN << "\t5." << Color::RESET << " Mostrar horario final inscrito ordenado por Materia\n";
	std::cout << Color::CYAN << "\t6." << Color::RESET << " Mostrar horario final inscrito ordenado por Calendario\n";
	std::cout << Color::YELLOW << "\t7." << Color::RESET << " Deshacer ultima accion (Undo)\n"; 
	std::cout << Color::RED << "\t8." << Color::RESET << " Salir\n"; 
	std::cout << "====================================================================================\n";
}
int getUserOption() {
	int option = 0;
	std::cout << Color::BOLD << "\nSeleccione una opcion: " << Color::RESET;

	while (!(std::cin >> option)) {
		std::cout << Color::RED << "Entrada invalida. Por favor ingrese un numero: " << Color::RESET;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return option;
}
void mainCaseAddCourse(Scheduler& scheduler) {
	std::string courseId;
	std::cout << Color::CYAN << "\nIngrese el ID del curso a "
		<< Color::BOLD << "inscribir" << Color::RESET << Color::CYAN
		<< " (del catalogo): " << Color::RESET;
	std::getline(std::cin, courseId);
	scheduler.addCourseToSchedule(courseId);
}
void mainCaseRemoveCourse(Scheduler& scheduler) {
	std::string courseId;
	std::cout << Color::CYAN << "\nIngrese el ID del curso a "
		<< Color::BOLD << "eliminar" << Color::RESET << Color::CYAN
		<< " (del horario): " << Color::RESET;
	std::getline(std::cin, courseId);

	scheduler.removeCourseFromSchedule(courseId);
}
void mainCaseDebugInOrder(Scheduler& scheduler) {
	scheduler.debugDisplayABB();
}
void testAutomaticGenerator() {
	clearConsole(); 
	std::cout << Color::BOLD << Color::YELLOW
		<< "=== PRUEBA DEL GENERADOR AUTOMATICO DE HORARIOS ===\n" << Color::RESET;
	std::cout << "Creando materias y grupos de prueba...\n\n";
	Subject materiaCalculo("Calculo Diferencial");

	// Opción A: Lunes y Miércoles 7:00 - 9:00 (Curso* se debe crear con new)
	Course* calcA = new Course();
	std::vector<TimeBlock> blocksCalcA = {
		TimeBlock(TimeBlock::MON, 420, 540, "CAL_A"), // 7:00 - 9:00
		TimeBlock(TimeBlock::WED, 420, 540, "CAL_A")
	};
	calcA->manualSetup("CAL_A", "Calculo Dif (Gpo A)", "Prof. Bueno", 2, blocksCalcA);
	materiaCalculo.addOption(calcA);

	// Opción B: Lunes y Miércoles 10:00 - 12:00 (Curso*)
	Course* calcB = new Course();
	std::vector<TimeBlock> blocksCalcB = {
		TimeBlock(TimeBlock::MON, 600, 720, "CAL_B"), // 10:00 - 12:00
		TimeBlock(TimeBlock::WED, 600, 720, "CAL_B")
	};
	calcB->manualSetup("CAL_B", "Calculo Dif (Gpo B)", "Prof. Barco", 2, blocksCalcB);
	materiaCalculo.addOption(calcB);


	// MATERIA 2: FÍSICA (Tiene 2 opciones)
	Subject materiaFisica("Fisica I");

	// Opción A: Lunes 10:00 - 12:00 (¡CHOCA CON CALCULO B!)
	Course* fisA = new Course();
	std::vector<TimeBlock> blocksFisA = {
		TimeBlock(TimeBlock::MON, 600, 720, "FIS_A") // 10:00 - 12:00
	};
	fisA->manualSetup("FIS_A", "Fisica I (Gpo A)", "Prof. Newton", 1, blocksFisA);
	materiaFisica.addOption(fisA);

	// Opción B: Martes 10:00 - 12:00 (Libre)
	Course* fisB = new Course();
	std::vector<TimeBlock> blocksFisB = {
		TimeBlock(TimeBlock::TUE, 600, 720, "FIS_B") // 10:00 - 12:00
	};
	fisB->manualSetup("FIS_B", "Fisica I (Gpo B)", "Prof. Einstein", 1, blocksFisB);
	materiaFisica.addOption(fisB);


	// MATERIA 3: PROGRAMACIÓN (1 opción)
	Subject materiaProgra("Programacion");

	// Opción A: Viernes 7:00 - 9:00 (Libre)
	Course* progA = new Course();
	std::vector<TimeBlock> blocksProgA = {
		TimeBlock(TimeBlock::FRI, 420, 540, "PROG_A") // 7:00 - 9:00
	};
	progA->manualSetup("PROG_A", "Progra (Gpo A)", "Prof. Turing", 1, blocksProgA);
	materiaProgra.addOption(progA);

	// --- 2. EJECUCIÓN DEL GENERADOR ---

	std::vector<Subject> materiasDeseadas;
	materiasDeseadas.push_back(materiaCalculo);
	materiasDeseadas.push_back(materiaFisica);
	materiasDeseadas.push_back(materiaProgra);

	std::cout << Color::CYAN << "Generando combinaciones posibles para:\n" << Color::RESET;
	for (const auto& sub : materiasDeseadas) {
		std::cout << "- " << sub.getName() << " (" << sub.getOptions().size() << " opciones)\n";
	}

	SchedulerGenerator generator;
	// ¡AQUÍ OCURRE LA MAGIA!
	std::vector<std::vector<Course*>> resultados = generator.generateSchedules(materiasDeseadas);


	// --- 3. MOSTRAR RESULTADOS ---

	std::cout << "\n" << Color::BOLD << Color::GREEN
		<< "=== RESULTADOS ENCONTRADOS: " << resultados.size() << " ===\n" << Color::RESET;

	int opcionNum = 1;
	for (const auto& horario : resultados) {
		std::cout << Color::YELLOW << "\n--- OPCION DE HORARIO #" << opcionNum << " ---\n" << Color::RESET;
		for (Course* curso : horario) {
			std::cout << "  > [" << curso->getId() << "] " << curso->getName()
				<< " (" << curso->getProfessor() << ")\n";
			// Opcional: Mostrar detalles de hora
			// for(int i=0; i<curso->getTimeBlocksCount(); i++) curso->getTimeBlock(i)->showTimeBlockInfo();
		}
		opcionNum++;
	}

	if (resultados.empty()) {
		std::cout << Color::RED << "No se encontraron combinaciones validas :( Intenta con otros grupos.\n" << Color::RESET;
	}

	// Limpieza de memoria (Mocks)
	// En un sistema real esto se maneja con destructores más complejos o smart pointers,
	// pero para este test manual borramos lo que creamos con 'new'.
	delete calcA; delete calcB;
	delete fisA; delete fisB;
	delete progA;
}
std::vector<Subject> loadSubjectsFromJSON(std::string filename) {
	std::vector<Subject> materiasCargadas;
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Error: No se pudo abrir " << filename << "\n";
		return materiasCargadas;
	}

	json data;
	try {
		file >> data; // Parsear el JSON mágicamente
	}
	catch (json::parse_error& e) {
		std::cerr << "Error de sintaxis en JSON: " << e.what() << "\n";
		return materiasCargadas;
	}

	// Iterar sobre las materias (El JSON es una lista de objetos)
	for (const auto& itemMateria : data) {
		std::string nombreMateria = itemMateria["nombre"];
		Subject nuevaMateria(nombreMateria);

		// Iterar sobre los grupos (opciones) de esa materia
		for (const auto& itemGrupo : itemMateria["opciones"]) {
			Course* nuevoCurso = new Course();
			std::string idGrupo = itemGrupo["id_grupo"];
			std::string profe = itemGrupo["profesor"];

			// Recolectar bloques de tiempo
			std::vector<TimeBlock> bloques;
			for (const auto& itemBloque : itemGrupo["bloques"]) {
				// Asegurarse de convertir a los tipos correctos
				int dia = itemBloque["dia"];
				int inicio = itemBloque["inicio"];
				int fin = itemBloque["fin"];

				// Convertir int a enum TimeBlock::Day
				TimeBlock::Day diaEnum = static_cast<TimeBlock::Day>(dia);

				bloques.push_back(TimeBlock(diaEnum, inicio, fin, idGrupo));
			}

			// Configurar el curso
			// Nota: Asumo que 1 bloque = 1 crédito por simplicidad, o calculamos count
			nuevoCurso->manualSetup(idGrupo, nombreMateria, profe, bloques.size(), bloques);

			// Añadir opción a la materia
			nuevaMateria.addOption(nuevoCurso);
		}

		materiasCargadas.push_back(nuevaMateria);
	}

	return materiasCargadas;
}




void saveToJSON(const std::vector<std::vector<Course*>>& schedules, std::string filename) {
	// Crear el objeto JSON raíz
	json root;
	root["status"] = "success";
	root["total_options"] = schedules.size();
	root["schedules"] = json::array();

	// Iterar por cada opción de horario completo
	for (size_t i = 0; i < schedules.size(); i++) {
		json scheduleObj;
		scheduleObj["id_opcion"] = i + 1;
		scheduleObj["cursos"] = json::array();

		// Iterar por cada curso dentro de esa opción
		for (Course* c : schedules[i]) {
			json courseObj;
			courseObj["id"] = c->getId();
			courseObj["nombre"] = c->getName();
			courseObj["profesor"] = c->getProfessor();
			courseObj["bloques"] = json::array(); // Aquí guardaremos los tiempos

			// --- LO NUEVO: ITERAR LOS BLOQUES DE TIEMPO ---
			for (unsigned short k = 0; k < c->getTimeBlocksCount(); k++) {
				TimeBlock* tb = c->getTimeBlock(k);
				if (tb != nullptr) {
					json blockObj;
					blockObj["dia"] = (int)tb->getDayOfWeek(); // Cast a int (0=Lunes, etc)
					blockObj["inicio"] = tb->getStartTime(); // Minutos (ej. 420)
					blockObj["fin"] = tb->getEndTime();      // Minutos (ej. 540)

					courseObj["bloques"].push_back(blockObj);
				}
			}
			// ---------------------------------------------

			scheduleObj["cursos"].push_back(courseObj);
		}
		root["schedules"].push_back(scheduleObj);
	}

	// Escribir al archivo
	std::ofstream file(filename);
	if (file.is_open()) {
		file << root.dump(4); // El 4 es para indentar bonito (pretty print)
		file.close();
	}
	else {
		std::cerr << "Error al crear el archivo JSON de salida.\n";
	}
}
void runHeadlessMode(std::string outputFile, std::string inputFile) {
	// 1. CARGAR DATOS REALES DESDE PYTHON
	std::vector<Subject> materiasDeseadas = loadSubjectsFromJSON(inputFile);

	if (materiasDeseadas.empty()) {
		// Generar un JSON de error si no hay datos
		std::ofstream file(outputFile);
		file << "{ \"status\": \"error\", \"message\": \"No se cargaron materias del input.json\" }";
		return;
	}

	// 2. EJECUTAR GENERADOR
	SchedulerGenerator generator;
	std::vector<std::vector<Course*>> resultados = generator.generateSchedules(materiasDeseadas);

	// 3. GUARDAR RESULTADOS
	saveToJSON(resultados, outputFile);

	// Limpieza de memoria (Importante: borrar los cursos creados con new)
	for (Subject& s : materiasDeseadas) {
		// Nota: En una implementación real, Subject debería tener un destructor que limpie sus opciones
		// Por ahora, confiamos en que el SO limpie al terminar el programa
	}
}

//int main() {
//	activateVirtualTerminal();
//	std::setlocale(LC_ALL,"");
//	Scheduler myScheduler;
//	int option = 0; 
//	
//	do {
//		clearConsole(); 
//		displayMenu(); 
//		std::cout << Color::CYAN << "\t9." << Color::RESET << " (BETA) Probar Generador Automatico\n";
//
//		option = getUserOption(); 
//
//		switch (option) {
//		case 1: 
//			myScheduler.createCourseInCatalog();
//			break;
//		case 2: 
//			myScheduler.displayCourseCatalog();
//			break; 
//		case 3: 
//			mainCaseAddCourse(myScheduler);
//			break; 
//		case 4: 
//			mainCaseRemoveCourse(myScheduler);
//			break; 
//		case 5: 
//			myScheduler.displayStudentSchedule();
//			break; 
//		case 6: 
//			mainCaseDebugInOrder(myScheduler);
//			break; 
//		case 7: 
//			myScheduler.undoLastAction();
//			break; 
//		case 8: 
//			std::cout << Color::CYAN << "\nSaliendo del programa. ¡Hasta luego!\n" << Color::RESET;
//			break;
//		case 9: 
//			testAutomaticGenerator();
//			break; 
//		default: 
//			std::cout << Color::RED << "\n\tOpcion no valida. Intente de nuevo.\n" << Color::RESET;
//			break;
//		}
//		if (option != 8) pauseConsole();
//	} while (option!=8); 
//}
int main(int argc, char* argv[]) {
	if (argc > 1 && std::string(argv[1]) == "--headless") {
		// Modo Robot activado
		std::string outputFile = "output.json"; // Nombre por defecto
		std::string inputFile = "input.json";

		// Si nos pasaron un segundo argumento, usamos ese como nombre de archivo
		if (argc > 2) outputFile = argv[2];
		if (argc > 3) inputFile = argv[3];

		runHeadlessMode(outputFile, inputFile);
		return 0; 
	}
	// 2. MODO INTERACTIVO (HUMANO)
	activateVirtualTerminal();
	std::setlocale(LC_ALL, ""); // Para tildes en consola
	Scheduler myScheduler;
	int option = 0;

	do {
		clearConsole();
		displayMenu();
		std::cout << Color::CYAN << "\t9." << Color::RESET << " (BETA) Probar Generador Automatico (Visual)\n";

		option = getUserOption();

		switch (option) {
		case 1: myScheduler.createCourseInCatalog(); break;
		case 2: myScheduler.displayCourseCatalog(); break;
		case 3: mainCaseAddCourse(myScheduler); break;
		case 4: mainCaseRemoveCourse(myScheduler); break;
		case 5: myScheduler.displayStudentSchedule(); break;
		case 6: myScheduler.debugDisplayABB(); break;
		case 7: myScheduler.undoLastAction(); break;
		case 8: std::cout << Color::CYAN << "\nSaliendo...\n" << Color::RESET; break;
		case 9: testAutomaticGenerator(); break;
		default: std::cout << Color::RED << "\n\tOpcion no valida.\n" << Color::RESET; break;
		}

		if (option != 8) pauseConsole();
	} while (option != 8);

	return 0;
}