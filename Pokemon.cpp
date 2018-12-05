//*****************************************************************************
// Pokemon.cpp
// Author: Kevin Peters
// Date: December 11, 2017
// Class: CIS 350, Bruce Elenbogen
// Description/Purpose: This program is a twist on the traveling salesperson problem, with a couple extra conditions.
// The goal is to find the shortest distance to travel to catch all unique pokemon and return to the origin.
// 

// Input:  int pokeStops, xCoord, yCoord
//		   string pokeName
// Output: int shortestPath

//*****************************************************************************

/********************INCLUDES**************************************************/

#include <iostream>
#include <algorithm>	//is needed?
#include <vector>
#include <math.h>		//is needed?
#include <string>
#include <queue>

/********************DATA TYPES************************************************/

struct Node {
	Node();
	int vertexNum;
	std::vector<std::string> pokemonCaught;
	int weightSoFar;
	std::vector<int> pokePathTook;			
	int bound;
	
};
struct Pokemon {
	int vertexNum;
	int xCoord;
	int yCoord;
	std::string pokeName;
};

//******************************************************************
// operator<(const Node & rhs, const Node & lhs)
// Purpose: Operator overload used to compare the Node's bounds of the priority queue
// Pre: Two valid Nodes to compare
// Post: Returns true if the right hand bound is greater than the left hand bound passed in
//******************************************************************
bool operator<(const Node & rhs, const Node & lhs) {
	return rhs.bound > lhs.bound;
}

//******************************************************************
// Node()
// Purpose: Node struct contructor.
// Pre: None
// Post: Creates an instance of Node struct
// and sets the bound equal to 0
//******************************************************************
Node::Node(){
	bound = -1;
}

//******************************************************************
// pokeInput(int stops,std::priority_queue <Node> &myQueue, std::vector<Pokemon> &myPokemon)
// Purpose: Input the initial data for the calculation and 
// set the initial data in both the priority queue and vector of pokemon
// Pre: stop > 0,myQueue and myPokemon are empty when passed in
// Post: myQueue and myPokemon are filled at each stop and 
// by reference passed back to pokeQueue and listOfPokemon respectively
//******************************************************************
void pokeInput(int stops,std::priority_queue <Node> &myQueue, std::vector<Pokemon> &myPokemon) {
	Node tempNode;
	Pokemon tempPokemon;
	for (int i = 0; i < stops; i++) {			// n time , 
		tempNode.vertexNum = i;
		tempPokemon.vertexNum = i;
		std::cin >> tempPokemon.xCoord;
		std::cin >> tempPokemon.yCoord;
		std::cin >> tempPokemon.pokeName;
		tempNode.pokemonCaught.push_back(tempPokemon.pokeName);						// 1 time, 1 space
		tempNode.weightSoFar = abs(tempPokemon.xCoord) + abs(tempPokemon.yCoord);
		tempNode.pokePathTook.push_back(i);
		tempNode.bound = 2 * (abs(tempPokemon.xCoord) + abs(tempPokemon.yCoord));
		//if (tempNode.bound == 80) {
		//	std::cout << "noooooo";
		//}
		myQueue.push(tempNode);														//lg(n) time, 1 space
		myPokemon.push_back(tempPokemon);											//1 time, 1 space
		tempNode.pokemonCaught.pop_back();											// 1 time, 1 space
		tempNode.pokePathTook.pop_back();											// 1 time, 1 space
	}
}

//******************************************************************
// checkBestNode(Node checkNode, Node & myBestNode)
// Purpose: Compare a node with all pokemon caught to see if it is the shortest path
// Pre: checkNode and myBestNode both have bounds greater than 0
// Post: compares and replaces bestNode if checkNode has the new lowest bound
//******************************************************************
void checkBestNode(Node checkNode, Node & myBestNode) {
	//myBestNode will be 0 when initialized
	//if (myBestNode.bound == 80) {
	//	std::cout << "nooooooooo";
	//}
	if (myBestNode.bound == -1) {
		//replace because this is first node will all pokemon
		myBestNode = checkNode;
	}
	else if (checkNode.bound < myBestNode.bound) {
		myBestNode = checkNode;
	}
	else {
		return;
	}
}

//******************************************************************
// calcBound(std::priority_queue <Node> & myQueue, std::vector<Pokemon> myPokemon, Node & bestNode)
// Purpose: Branch and bound calculations to find the minimum path
// Pre: myQueue and myPokemon must not be empty or nothing will happen in this function
// also bestNode's bound should = 0 when entering this function
// Post: myQueue will be empty and the minimum path will be set in the bestNode
//******************************************************************
void calcBound(std::priority_queue <Node> & myQueue, std::vector<Pokemon> myPokemon, Node & bestNode) {
	//n! time
	while (!myQueue.empty()) {
		Node tempNode;
		Node newPokemon;
		int pokemonCaught = 0;
		bool isCaught = false;
		tempNode = myQueue.top();
		//if (myQueue.top().bound == 80) {
		//	std::cout << "nooooo";
		//}
		//lg(n) time, 1 space
		myQueue.pop();
		//check if all pokemon caught at this stop
		//nested for loops n^2 time because tempNode.pokemonCaught approaches myPokemon.size, 1 space
		for (int i = 0; i < myPokemon.size(); i++) {
			for (int j = 0; j < tempNode.pokemonCaught.size(); j++) {
				if (myPokemon[i].pokeName == tempNode.pokemonCaught[j] && !isCaught) {
					isCaught = true;
					pokemonCaught++;
				}
			}
			isCaught = false;
		}
		//if all the pokemon are caught, then check if its the minimum path
		if (pokemonCaught == myPokemon.size()) {
			checkBestNode(tempNode, bestNode);
		}
		//if we haven't caught all the pokemon, keep going
		else {
			//nested for loops n^2 time because tempNode.pokemonCaught approaches myPokemon.size
			for (int i = 0; i < myPokemon.size(); i++) {
				bool isNewPokemon = true;
				for (int j = 0; j < tempNode.pokemonCaught.size(); j++) {
					if (tempNode.pokemonCaught[j] == myPokemon[i].pokeName) {
						isNewPokemon = false;
					}
				}
				//we don't have to do any of this if its a pokemon we already caught
				if (isNewPokemon) {
					//1 time, 1 space
					tempNode.pokemonCaught.push_back(myPokemon[i].pokeName);
					//1 time, 1 space
					tempNode.pokePathTook.push_back(i);
					newPokemon.pokemonCaught = tempNode.pokemonCaught;
					newPokemon.pokePathTook = tempNode.pokePathTook;
					newPokemon.weightSoFar = tempNode.weightSoFar + abs(myPokemon[tempNode.vertexNum].xCoord - myPokemon[i].xCoord) + abs(myPokemon[tempNode.vertexNum].yCoord - myPokemon[i].yCoord);
					newPokemon.bound = newPokemon.weightSoFar + abs(myPokemon[i].xCoord + myPokemon[i].yCoord);
					newPokemon.vertexNum = i;
					// lg(n) time, 1 space
					myQueue.push(newPokemon);
					//1 time, 1 space
					tempNode.pokemonCaught.pop_back();
					//1 time, 1 space
					tempNode.pokePathTook.pop_back();
				}
			}
		}
	}
}

int main() {
	int pokeStops;
	bool pokeToCatch = false;
	std::priority_queue <Node> pokeQueue;
	std::vector<Pokemon> listOfPokemon;
	Node bestNode;
	std::vector<int> numCaught;
	std::cin >> pokeStops;
	pokeInput(pokeStops, pokeQueue, listOfPokemon);
	for (int i = 0; i < pokeStops; i++) {
		if (listOfPokemon[i].xCoord != 0 || listOfPokemon[i].yCoord != 0) {
			pokeToCatch = true;
		}
	}
	if (!pokeToCatch) {
		std::cout << '0';
	}
	else {
		calcBound(pokeQueue, listOfPokemon, bestNode);
		std::cout << bestNode.bound;
	}
	//for (int i = 0; i < bestNode.pokePathTook.size(); i++) {
	//	std::cout << bestNode.pokePathTook[i] << '\n';
	//}
	//std::cout << "\n\n\nThe answer is: ";
	return 0;
}
//5 9 Eevee 10 10 Flareon 1 1 Flareon 1 8 Jolteon 2 8 Umbreon
//5 9 Eevee 10 10 Eevee 1 1 Eevee 1 8 Eevee 2 8 Eevee
//3 0 0 pickachu 0 0 mewtwo 0 0 zapadose 3 pokemon all at 0,0
//8 10 0 poke1 10 10 poke2 0 10 poke3 -10 10 poken4 -10 0 poke5 -10 -10 poke6 0 -10 poke7 10 -10 poke8
//6 0 1 p1 0 2 p2 0 3 p3 0 4 p4 0 5 p5 0 6 p1
//4 10 10 poke1 10 -10 poke2 -10 -10 poke3 -10 10 poken4
