#pragma once
class Scheduler;

#define TAM 20

class PilaScheduler
{
public:
	PilaScheduler();
	Scheduler* Consultar();
	Scheduler* Extraer();
	int Insertar(Scheduler*);
	void Mostrar();

private:
	Scheduler* arreglo[TAM];
	int min, max, tope;
};