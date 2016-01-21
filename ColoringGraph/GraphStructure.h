#include <list>
#include <vector>
// 4 Color for Coloring MAP
enum Color{
	RED,
	GREEN,
	BLUE,
	WHITE
};
//Graph Class for showing MAP and adjacent
class VERTEX;
class EDGE
{
public:
	EDGE(VERTEX *, EDGE *);
	~EDGE();
	VERTEX *getEnd();
	EDGE *getNext();
	void print();
private:
	VERTEX *end;
	EDGE *next;
};
list<VERTEX>::iterator VertexIt;
list<EDGE>::iterator EdgeIt;
EDGE::EDGE(VERTEX *, EDGE *)
{
}

EDGE::~EDGE()
{
}


class VERTEX {
public:
	VERTEX(char, Color, VERTEX *);
	~VERTEX();
	Color getColor();
	void printEdgs();
	void printGraph();
	void connectTo(VERTEX *);
	VERTEX *getNext();
	EDGE *getFirstEdge();
private:
	char Name;
	Color color;
	EDGE *edges;
	VERTEX *next;
};
Color VERTEX::getColor(){
	return color;
}
void VERTEX::printEdgs(){
	printf("\nEdges for %s is=", Name);
	do{
		printf("%s ", next->Name);
	} while (next);
}
void VERTEX::printGraph(){
	printf(" %s ",Name);
	if (next) next->printGraph();
}
void VERTEX::connectTo(VERTEX *Input){
	//edges->getEnd;
}
class Graph {
public:
	Graph();
	~Graph();
	VERTEX *find(char);
	bool AddVertex(char);
	bool AddEdge(char, char);
	void print();

private:
	VERTEX *first;
};
