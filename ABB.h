#pragma once
#include "TimeBlock.h"
using namespace std; 

struct NodeABB {
	TimeBlock block; 
	NodeABB* left;
	NodeABB* right;
};
class ABB
{
private: 
	NodeABB* root, *auxNode;
	bool removeRecursive(NodeABB*, NodeABB*, TimeBlock&, std::string);
	void clearRecursive(NodeABB*);
	NodeABB* copyRecursive(NodeABB*);
public: 
	ABB(); 
	~ABB(); 
	ABB(const ABB& other);
	ABB& operator=(const ABB& other);
	NodeABB* getRoot();
	void crearNodo(TimeBlock&);
	void insert(NodeABB*);
	bool remove(TimeBlock&);
	int compareTimeBlocks(TimeBlock&, TimeBlock&);
	NodeABB* findPredecessor(NodeABB*);
	bool checkConflict(TimeBlock&, NodeABB*); 
	bool isEmpty();
	void inOrder(NodeABB*);
};

