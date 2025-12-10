#include "ABB.h"
#include <iostream>

ABB::ABB(){
	root = nullptr;
	auxNode = nullptr;
}
void ABB::clearRecursive(NodeABB* node) { //Limpiar el arbol en postorden
	if (node == nullptr)return; //Caso base
	clearRecursive(node->left); //limpiar la izquierda
	clearRecursive(node->right); //limpiar la derecha
	delete node; //borrar el nodo
}
NodeABB* ABB::copyRecursive(NodeABB* otherNode) { //Copiar el arbol desde la raíz
	if (otherNode == nullptr) return nullptr; //Caso base
	NodeABB* newNode = new NodeABB; //Crear nuevo nodo
	newNode->block = otherNode->block; //Copiar el bloque de tiempo de la raiz
	newNode->left = copyRecursive(otherNode->left); //Copiar la izquierda
	newNode->right = copyRecursive(otherNode->right); //Copiar la derecha
	return newNode; //Retornar el nuevo nodo
}
ABB::~ABB() {
	clearRecursive(root); 
}
ABB::ABB(const ABB& other){//Constructor de copia para crear un objeto desde cero
	this->root = nullptr; 
	this->auxNode = nullptr; 
	this->root = copyRecursive(other.root);
}
ABB& ABB::operator=(const ABB& other) { //Operador de asignación que necesita limpiar y copiar todo el arbol. Es para sobreescribir un objeto existente
	if (this == &other)return *this; //No se puede copiar a sí mismo
	clearRecursive(this->root);//Borrar la memoria que teníamos antes
	this->root = nullptr; //reiniciar su estado
	this->auxNode = nullptr;
	this->root = copyRecursive(other.root); //Llamar al método de copia
	return *this;
}
NodeABB* ABB::getRoot()
{
	return root;
}
bool ABB::checkConflict(TimeBlock& nuevo, NodeABB* actual)   //True = si hay conflict con el horario, False = ps que no :v
{
	if (actual == nullptr)
		return false;

	if (nuevo.conflictsWith(actual->block)) {   //Checa si es todo es igualito
		std::cout << "\n¡Conflicto Detectado!\n";
		std::cout << "Bloque Nuevo: \n";
		nuevo.showTimeBlockInfo();
		std::cout << "Bloque Existente: \n";
		actual->block.showTimeBlockInfo();
		return true;
	}

	//Si no conflictua tengo que checar el horario para saber hacia que lado del arbol debo irme a checa de nuevo

	if (nuevo.getDayOfWeek() < actual->block.getDayOfWeek())
		return checkConflict(nuevo, actual->left);
	else if (nuevo.getDayOfWeek() > actual->block.getDayOfWeek())
		return checkConflict(nuevo, actual->right);
	else {
		// Suponiendo que el dia e el mismo
		//Checa ambos lados para garantizar una detección adecuada para periodos más extensos: Ej: NUEVO- 10:00-16:00, conflictúa con 12:00-14:00 y 15:00-17:00
		bool conflictLeft = false; 
		if (nuevo.getStartTime() < actual->block.getStartTime())
			conflictLeft= checkConflict(nuevo, actual->left);
		if (conflictLeft)return true;
		bool conflictRight = false;
		if (nuevo.getStartTime() > actual->block.getStartTime())
			conflictRight= checkConflict(nuevo, actual->right);
		return conflictRight; 
	} 
}
void ABB::inOrder(NodeABB* inicio)
{
	if (root == nullptr)
	{
		cout << "\n. . . Empty . . .\n";
		return;
	}

	if (inicio->left != nullptr)
		inOrder(inicio->left);
	inicio->block.showTimeBlockInfo();
	cout << endl;
	if (inicio->right != nullptr)
		inOrder(inicio->right);
}
bool ABB::isEmpty() {
	return root == nullptr;
}
void ABB::crearNodo(TimeBlock& block) {
	auxNode = new NodeABB; 
	auxNode->block = block;
	auxNode->left = nullptr;
	auxNode->right = nullptr;
}
void ABB::insert(NodeABB* current) {
	if (root == nullptr) { //Insertar en la raiz
		root = auxNode;
		auxNode = nullptr;
		std::cout << "\tInserted as root\n" << std::endl;
		return; 
	}
	if (current == nullptr)return; //Caso base (no debería pasar)

	if (auxNode->block.getDayOfWeek() < current->block.getDayOfWeek() || 
		(auxNode->block.getDayOfWeek() == current->block.getDayOfWeek() &&
			auxNode->block.getStartTime() < current->block.getStartTime())) { //Si el día es menor o si es el mismo día pero la hora de inicio es menor
		if (current->left == nullptr) { //Insertar a la izquierda
			current->left = auxNode;
			auxNode = nullptr; 
			std::cout << "\tInserted to the left\n" << std::endl;
			return; 
		}
		else { //Seguir buscando a la izquierda
			insert(current->left);
			return; 
		}
	}
	else {
		if (current->right == nullptr) { //Insertar a la derecha
			current->right = auxNode;
			auxNode = nullptr;
			std::cout << "\tInserted to the right\n" << std::endl;
			return; 
		}
		else { //Seguir buscando a la derecha
			insert(current->right);
			return; 
		}
	}
}
	NodeABB* ABB::findPredecessor(NodeABB* node) { //Encontrar el predecesor de un nodo (el más grande del subárbol izquierdo)
		if (node == nullptr || node->left == nullptr)return nullptr; //Si no hay nodo o no hay subárbol izquierdo no hay predecesor
		NodeABB* current = node->left; //Primero va al subarbol izquierdo
		while (current->right != nullptr) { //Busca el nodo más a la derecha del subarbol izquierdo
			current = current->right;
		}
		return current;
	}
	bool ABB::remove(TimeBlock& valor) { //Método Público para eliminar un bloque de tiempo
		if (root == nullptr) {
			std::cout << "\tEl arbol esta vacio\n";
			return false;
		}
		return removeRecursive(nullptr, root, valor, "");
	}
	bool ABB::removeRecursive(NodeABB* padre, NodeABB* inicio, TimeBlock& valor, std::string subarbol) {
		NodeABB* hijo; 
		int comparison; 
		//Fase 1: Buscar el nodo a eliminar
		if (inicio == nullptr) {
			std::cout << "\tNo se encontro el bloque de tiempo a eliminar\n";
			return false; 
		}
		comparison = compareTimeBlocks(inicio->block, valor); //Comparar los bloques de tiempo para saber hacia donde ir (izq o der)

		if (comparison > 0) {//Ir a la izquierda
			return removeRecursive(inicio, inicio->left, valor, "left"); 
		}
		else if (comparison < 0) {//Ir a la derecha
			return removeRecursive(inicio, inicio->right, valor, "right");
		}
		//Fase 2: Eliminar el nodo encontrado
		else {	
			if (inicio->left == nullptr || inicio->right == nullptr) { //Caso 1 y 2: Nodo con 0 o 1 hijo
				hijo = (inicio->left != nullptr) ? inicio->left : inicio->right; //Asignar el hijo no nulo (o nulo si no tiene hijos)

				if (inicio == root) {
					root = hijo;
				}
				else if (subarbol == "left") {
					padre->left = hijo;
				}
				else {
					padre->right = hijo;
				}
				delete inicio; 
				std::cout << "\tBloque de tiempo eliminado correctamente\n";
				return true;
			}
			else {//Caso 3: Dos hijos
				NodeABB* predecesor = findPredecessor(inicio); //Encontrar el predecesor inorden
				inicio->block = predecesor->block;//Reemplaza el timeblock a borar por el predecesor y lo sobreescribe
				return removeRecursive(inicio, inicio->left, predecesor->block, "left");// Llamar recursivamente para eliminar el predecesor que casi siempre es una hoja o tiene un hijo
			}
		}
	}
int ABB::compareTimeBlocks(TimeBlock& b1, TimeBlock& b2) {
	if (b1.getDayOfWeek() != b2.getDayOfWeek()) { //Si los dias son distintos, los restamos para ver si es mayor o menor
		return (int)b1.getDayOfWeek() - (int)b2.getDayOfWeek();
	}
	if (b1.getStartTime() != b2.getStartTime()) { //Si los días son iguales, y las horas de inicio son distintos los restamos
		return b1.getStartTime() - b2.getStartTime();
	}
	return b1.getEndTime() - b2.getEndTime();//Si los días
}