#include "PilaScheduler.h"
#include "Scheduler.h"
#include <iostream>

PilaScheduler::PilaScheduler()
{
	min = 0;
	max = TAM - 1;
	tope = -1;
	for (int i = 0; i < TAM; i++) {
		arreglo[i] = nullptr;
	}
}
Scheduler* PilaScheduler::Consultar()
{
	if (tope < min) {
		return nullptr;
	}
	return arreglo[tope];
}
Scheduler* PilaScheduler::Extraer()
{
	Scheduler* extraido;
	if (tope < min) {
		return nullptr;
	}
	extraido = arreglo[tope];
	arreglo[tope] = nullptr;
	tope--;

	return extraido;
}
int PilaScheduler::Insertar(Scheduler* actual)
{
	if (tope == max) { //La pila está llena
		std::cout << "\tPila llena. Descartando versiones más antiguas.\n";
		delete arreglo[min];//Eliminamos el elemento más pequeño
		for (int i = min; i < max; i++) { //recorremos cada elemento un espacio hacia adelante
			arreglo[i] = arreglo[i + 1];
		}
		arreglo[max] = new Scheduler(*actual);//Insertamos el nuevo elemento en la nueva posición disponible
	}
	else {
		tope++;//Incrementamos el tope
		arreglo[tope] = new Scheduler(*actual);//Insertamos el nuevo elemento en la nueva posición disponible
	}
	
	return 0;
}
void PilaScheduler::Mostrar()
{
	std::cout << "\n---- Historial De Versiones ----\n";
	if (tope < min) {
		std::cout << "\tNo hay versiones guardadas.\n";
		return;
	}

	for (int i = tope; i >= min; i--) {
		std::cout << "Versión " << i + 1 << ":";
		if (i == tope) {
			std::cout << " <-- Más reciente";
		}
		std::cout << "\n";
		if (arreglo[i] != nullptr) {
			std::cout << "\tCursos en esta versión: "; 
				//<< arreglo[i] << "\n";
		}
	}
	std::cout << "---------------------------\n";
}