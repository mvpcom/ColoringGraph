////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Solve Map Coloring Problem using Genetic Algorithm
// Design & Programming by Mojtaba Valipour @ CyberHands - JSU 
// 3.10.1393 - 25.12.2014
// Online CV: ir.linkedin.com/in/mojtabavalipour
// cyberhands.org	- CyberHands Research Team
// cpna.ir	- CyberPress News Agency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ColoringGraph.cpp : main project file.

#include "stdafx.h"
#include <random>
#include<time.h>
#include <list>

using namespace System;
using namespace std;


//const int Population_Size = 4;					// Size of Initial Random Population - This is a sample
const int Population_Size = 10;					// Size of Initial Random Population - Mentor Sample Test
const int Node_Quantity = 8;						// Number of Nodes - Mentor Sample Test
//const int Node_Quantity = 6;						// Number of Nodes 
const int Color_Num = 4;						//You Must change this if you want coloring wuth more colors
int Population_Fitness_Value[Population_Size];	//Define Global
int repeat = 0;			//Maximum 
//int k = Population_Size;						// K is Variable for using in Circles (While,do etc.) Easing use of Population_Size


enum class COLOR{  //Color_Num is 4 Because We have 4 colors for coloring
	R,   //RED
	G,	 //GREEN
	B,	 //BLUE
	W	 //WHITE
};

//Nodes Colors for one Individual in Population
struct MAP_COLOR_ARRAY{
	COLOR Nodes_Colors[Node_Quantity];
};

// A Node in Adjacency LIST
struct Node
{
	int name;
	COLOR color;
	Node* next;
};

//list<Node> Adjacency; // This is a list for each node in graph
//ITERATOR for searchin in Adjacency LISTs
std::list<Node>::iterator it;

// Graph is new Data Structures for saving our MAP
struct Graph
{
	int Size = Node_Quantity;
	Node Nodes_Array[Node_Quantity];
	list<Node> Adjacency[Node_Quantity];
} MAP;

// Adding Edge To Our MAP
void Edge_ADD(int src, int dest)
{
	//Node NEW_NODE_src,NEW_NODE_dest;
	//NEW_NODE_src.name = dest;
	//NEW_NODE_src.color = COLOR::R;
	//NEW_NODE_src.next = NULL;
	it = MAP.Adjacency[src].begin();
	//MAP.Adjacency[src].insert(it, NEW_NODE_src);
	MAP.Adjacency[src].insert(it, MAP.Nodes_Array[dest]);
	//NEW_NODE_dest.name = src;
	//NEW_NODE_dest.color = COLOR::R;
	//NEW_NODE_dest.next = NULL;
	it = MAP.Adjacency[dest].begin();
	//MAP.Adjacency[dest].insert(it, NEW_NODE_dest);
	MAP.Adjacency[dest].insert(it, MAP.Nodes_Array[src]);
}
// Print MAP
void Graph_PRINT()
{
	for (int i = 0; i< MAP.Size; i++)
	{
		it = MAP.Adjacency[i].begin();
		printf("\n Adjacency list of Nodes %d\n Front ", MAP.Nodes_Array[i].name);
		while (it != MAP.Adjacency[i].end())
		{
			printf("-> %d", it->name);
			it++;
		}
		printf("\n");
	}
}

int Random_Selection(){		//Random Selection regards to goodness and randomness
	
	//Deprecated because rand() has problem and generate same number
	//float r =  (float) rand() / RAND_MAX;	//Random Number between 0 and 1

	std::random_device rseed;
	std::mt19937 rgen(rseed()); // mersenne_twister
	std::uniform_real_distribution<float> idist(0, 1); // [0,Node_Quantity]
	float r = idist(rgen); // Random Number

	float Temp = 0;		//Temporary Number for using goodness in randomize process
	float Sum = 0;		//Sum of All Population Values
	int worst = 0;
	int Temp_Population[Population_Size];
	for (int i = 0; i < Population_Size; i++){	//Copy Population_Fitness_Value in Temp Array
		Temp_Population[i] = Population_Fitness_Value[i];
	}
	for (int i = 0; i < Population_Size; i++){	//Find worst
		if (Population_Fitness_Value[i] < worst){
			worst = Population_Fitness_Value[i];
		}
	}
	for (int i = 0; i < Population_Size; i++){	//Shift all and change the value of worst and best
		Temp_Population[i] -= worst;
	}
	for (int i = 0; i < Population_Size; i++){	//Compute Sum of All Population Size
		Sum += Temp_Population[i];
	}

	//Sorting and keep Name of each fitness value
	int Array_2D[Population_Size][2];
	for (int i = 0; i < Population_Size; i++){
		Array_2D[i][0] = i;
		Array_2D[i][1] = Temp_Population[i];
	}
	//shell sort Algorithm for sorting
	int gap_sequence[] = { 13, 9, 5, 2, 1 };
	int n = Population_Size;
	// for (gap = n/2; gap > 0; gap /= 2)
	for (int gap : gap_sequence) if (gap < n)
	{
		// for (i = gap; i < n; i++)
		for (int i = gap; i < n; ++i)
			// for (j=i-gap; j>=0 && v[j]>v[j+gap]; j-=gap)
			for (int j = i - gap; j >= 0 && Array_2D[j][1] > Array_2D[j + gap][1]; j -= gap){
				/*
				temp = v[j];
				v[j] = v[j+gap];
				v[j+gap] = temp;
				*/
				std::swap(Array_2D[j][0], Array_2D[j + gap][0]);
				std::swap(Array_2D[j][1], Array_2D[j + gap][1]);
			}
	}
	//Randomize process according to goodness and randomness
	for (int i = 0; i < Population_Size; i++){
		Temp += Array_2D[i][1] / Sum;
		if (r <= Temp) return Array_2D[i][0];
	}
	////Randomize process according to goodness and randomness
	//for (int i = 0; i < Population_Size; i++){
	//	Temp += Temp_Population[i] / Sum;
	//	if (r <= Temp) return i;
	//}
	return 0;
}

void Pop_Fitness_Evaluator(MAP_COLOR_ARRAY Initital_Population[Population_Size]){		// GoodNess Evaluator for our Initial Population - Fitness Function
	//int Population_Fitness_Value[Population_Size];
	int value;
	for (int i = 0; i < Population_Size; i++){
		value = 0;
		//Coloring Graph with each Individuals in Population 
		for (int j = 0; j < Node_Quantity; j++){
			MAP.Nodes_Array[j].color = Initital_Population[i].Nodes_Colors[j];
		}
		list<Node>::iterator it_Temp;
		//Compute Fitness Value
		for (int k = 0; k< MAP.Size; k++)
		{
			it = MAP.Adjacency[k].begin();
			//it_Temp = MAP.Nodes_Array[k];		//Deprecated
			while (it != MAP.Adjacency[k].end())
			{
				//it++;
				int Temp1 = MAP.Nodes_Array[k].name;
				COLOR Temp2 = MAP.Nodes_Array[k].color;
				int Temp3 = it->name;
				COLOR Temp4 = MAP.Nodes_Array[it->name].color;
				//printf("\nNode %d Color is=%d, Node %d color is %d --> Value=%d\n",Temp1,Temp2,Temp3,Temp4,value);		//Test & Debug This line of Code
				if (MAP.Nodes_Array[it->name].color == MAP.Nodes_Array[k].color){
					value--;
				}
				//else{
				//	value--;
				//}
				it++;
			}
		}
		Population_Fitness_Value[i] = value;
	}
	//return Population_Fitness_Value;
}

MAP_COLOR_ARRAY Reproduce(MAP_COLOR_ARRAY Population[Population_Size],int x, int y){		//CrossOver Operation in Genetic Algorithm
	MAP_COLOR_ARRAY Result;
	//Select Real X and Y according to their name x and y
	MAP_COLOR_ARRAY X_MAP_COLOR;
	MAP_COLOR_ARRAY Y_MAP_COLOR;
	for (int i = 0; i < Node_Quantity; i++){
		X_MAP_COLOR.Nodes_Colors[i] = Population[x].Nodes_Colors[i];
		Y_MAP_COLOR.Nodes_Colors[i] = Population[y].Nodes_Colors[i];
	}

	//int n = Node_Quantity;	//Number of Nodes in Population X for Example

	//Deprecated because rand() has problem and generate same number
	//int c = rand() % n;

	std::random_device rseed;
	std::mt19937 rgen(rseed()); // mersenne_twister
	std::uniform_int_distribution<int> idist(0, Node_Quantity-1); // [0,Node_Quantity]
	int c = idist(rgen);

	//CrossOver compound two Population
	for (int i = c; i < Node_Quantity;i++){
		X_MAP_COLOR.Nodes_Colors[i] = Y_MAP_COLOR.Nodes_Colors[i];
	}
	Result = X_MAP_COLOR;
	return Result;
}

MAP_COLOR_ARRAY Mutation(MAP_COLOR_ARRAY Child){		//Mutation Operation in Genetic Algorithm
	//srand(time(NULL));							//Seed Random Again
	MAP_COLOR_ARRAY Result;
	//int Max_Change = rand() % Node_Quantity; //Maximum Node's Color Change with Mutation - This is Random Process 
	
	//Mutation Chance is 5 Percent
	
	//Deprecated because rand() has problem and generate same number in long time
	//float Chance = rand() % 20;

	std::random_device rseed;
	std::mt19937 rgen(rseed()); // mersenne_twister
	std::uniform_int_distribution<int> idist(0, 20); 
	int Chance = idist(rgen); 

	if (Chance>1){
		return Child;
	}
	//printf("\n\nChance is Succeed\n\n");		//This is for Debugging


	//int Max_Change = rand() % Node_Quantity; //Maximum Node's Color Change with Mutation - This is Random Process 
	std::uniform_int_distribution<int> idist2(0, Node_Quantity-1);
	int Max_Change = idist2(rgen);
	
	for (int i = 0; i < Max_Change; i++){
		
		//Deprecated because rand() has problem and generate same number
		//int Rand_Num = rand() % Node_Quantity;

		std::uniform_int_distribution<int> idist3(0, Node_Quantity - 1);
		int Rand_Num = idist3(rgen);
		
		//Deprecated because rand() has problem and generate same number
		//int Rand_Color_Num = rand() % Color_Num;
		std::uniform_int_distribution<int> idist4(0, Color_Num-1);
		int Rand_Color_Num = idist4(rgen);


		switch (Rand_Color_Num){
		case 0:
			Child.Nodes_Colors[Rand_Num] = COLOR::R;
			break;
		case 1:
			Child.Nodes_Colors[Rand_Num] = COLOR::G;
			break;
		case 2:
			Child.Nodes_Colors[Rand_Num] = COLOR::B;
			break;
		case 3:
			Child.Nodes_Colors[Rand_Num] = COLOR::W;
			break;
		}
	}
	Result = Child;
	return Result;
}
//See if All the Constraints is pass and return false if we have unpass constraint - Use global Variables
bool Fit_Enough(){
	bool Result = false;
	for (int i = 0; i < Population_Size;i++){
		if (Population_Fitness_Value[i] == 0)
		{
			Result = true;
			return Result;
		}
	}
	return Result;
}
void Genetic_Algorithm(){
	//int Population_Fitness_Value[Population_Size];
	//int *Population_Fitness_Value_Pointer;
	bool Fitness_Func_Result = false;

	MAP_COLOR_ARRAY Initial_Population[Population_Size];

	//Initial Random Population Making Process  
	for (int i = 0; i < Population_Size; i++){
		for (int j = 0; j < Node_Quantity; j++){
			int temp = rand() % Color_Num;  // 4 is number of COLORs we have
			switch (temp)	// Random choosing a color
			{
			case 0:
				Initial_Population[i].Nodes_Colors[j] = COLOR::R;
				break;
			case 1:
				Initial_Population[i].Nodes_Colors[j] = COLOR::G;
				break;
			case 2:
				Initial_Population[i].Nodes_Colors[j] = COLOR::B;
				break;
			case 3:
				Initial_Population[i].Nodes_Colors[j] = COLOR::W;
				break;
			}
		}
	}

	do{
		MAP_COLOR_ARRAY New_Population[Population_Size];
		repeat++;
		//Population_Fitness_Value_Pointer = Pop_Fitness_Evaluator(Initial_Population);	//Fitness & Goodness evaluator
		//for (int i = 0; i < Population_Size; i++){
		//	Population_Fitness_Value[i]=Population_Fitness_Value_Pointer[i];
		//}
		for (int i = 0; i < Population_Size; i++){
			int x = Random_Selection(); //Using global variable Population_Fitness_Value
			int y = Random_Selection();

			// These are only for test
			//std::random_device rseed;
			//std::mt19937 rgen(rseed()); // mersenne_twister
			//std::uniform_int_distribution<int> idist(0, Node_Quantity-1); // [0,Node_Quantity]
			//int x = idist(rgen); //Using global variable Population_Fitness_Value
			//int y = idist(rgen);


			MAP_COLOR_ARRAY Child = Reproduce(Initial_Population,x, y);
			Child = Mutation(Child);
			
			////Debugging Process
			//printf("\n%d is\n",i);
			//printf("\nChild %d(X)=",x);
			//for (int j = 0; j < Node_Quantity; j++){
			//	printf("%d", Initial_Population[x].Nodes_Colors[j]);
			//}
			//printf(" and %d(Y)",y);
			//for (int j = 0; j < Node_Quantity; j++){
			//	printf("%d", Initial_Population[y].Nodes_Colors[j]);
			//}
			//printf(" is ");
			//for (int j = 0; j < Node_Quantity; j++){
			//	printf("%d",Child.Nodes_Colors[j]);
			//}
			//printf("\n");

			//Copy Array
			for (int j = 0; j < Node_Quantity; j++){
				New_Population[i].Nodes_Colors[j] = Child.Nodes_Colors[j];
			}
		}
		//Copy Arrays
		for (int i = 0; i < Population_Size; i++){
			for (int j = 0; j < Node_Quantity; j++){
				Initial_Population[i].Nodes_Colors[j] = New_Population[i].Nodes_Colors[j];
			}
		}
		Pop_Fitness_Evaluator(Initial_Population);	//Fitness & Goodness evaluator
		Fitness_Func_Result = Fit_Enough();

		//Debug Process
		//printf("\nRepeat is=%d\n",repeat);

	} while (repeat<10000 && !Fitness_Func_Result);
	
	////Answer Not found :C
	//if (!Fitness_Func_Result){
	//	printf("\nGenetic Algorithm can't find best answer for this problem, I'm sorry! :C\n");
	//}
	int Best_Individual = -1;
	for (int i = 0; i < Population_Size; i++){
		if (Population_Fitness_Value[i] == 0)
			Best_Individual = i;
	}
	if (Best_Individual == -1){
		//Answer Not found :C
		if (!Fitness_Func_Result){
			printf("\nGenetic Algorithm can't find best answer for this problem, I'm sorry! :C\n");
		}
	}
	else{
		//Print Best Population-Test
		for (int i = 0; i < Node_Quantity; i++){
			printf("\n The Color of Node %d=%d\n", i, Initial_Population[Best_Individual].Nodes_Colors[i]);
		}
	}

	//return New_Population;
}

int main(array<System::String ^> ^args)
{
	srand(time(NULL));								// Random Seeding rand Function 
	//a Sample Map , Please Consider for another problem you must change this
	//for (int i = Node_Quantity-1; i>=0 ; i--){
	//	MAP.Nodes_Array[i].color=COLOR::R;
	//	MAP.Nodes_Array[i].name = i;
	//	if (i == Node_Quantity - 1){
	//		MAP.Nodes_Array[i].next = NULL;
	//	}
	//	else{
	//		MAP.Nodes_Array[i].next = &MAP.Nodes_Array[i+1];
	//	}
	//}
	//Edge_ADD(0, 3);
	//Edge_ADD(0, 1);
	//Edge_ADD(1, 4);
	//Edge_ADD(1, 3);
	//Edge_ADD(2, 5);
	//Edge_ADD(2, 3);
	//Edge_ADD(3, 4);

	//Mentor Sample Test
	for (int i = Node_Quantity - 1; i >= 0; i--){
		MAP.Nodes_Array[i].color = COLOR::R;
		MAP.Nodes_Array[i].name = i;
		if (i == Node_Quantity - 1){
			MAP.Nodes_Array[i].next = NULL;
		}
		else{
			MAP.Nodes_Array[i].next = &MAP.Nodes_Array[i + 1];
		}
	}
	Edge_ADD(0, 1);
	Edge_ADD(0, 2);
	Edge_ADD(0, 3);
	Edge_ADD(0, 5);
	Edge_ADD(0, 6);
	Edge_ADD(1, 3);
	Edge_ADD(1, 4);
	Edge_ADD(1, 6);
	Edge_ADD(1, 7);
	Edge_ADD(2, 3);
	Edge_ADD(2, 5);
	Edge_ADD(3, 4);
	Edge_ADD(4, 7);
	Edge_ADD(5, 6);
	Edge_ADD(6, 7);


	Graph_PRINT();  // This is a test for graph maker

	//Answer achieve by Genetic Algorithm
	//MAP_COLOR_ARRAY Best_Population = Genetic_Algorithm(Population);
	Genetic_Algorithm();
	//int* Population_Fitness_Value = Pop_Fitness_Evaluator(Best_Population);

	//printf("\n\nThe Selected Number is = %i", Random_Selection(Population));	//Test Random Selection Function
	//Genetic_Algorithm(Population);					// Call Genetic Alogrithm with initial Population

	Console::WriteLine(L"\n\nProgramming by Mojtaba Valipour@ CyberHands.org -Artificial Intelligence");
	system("PAUSE");
	return 0;
}