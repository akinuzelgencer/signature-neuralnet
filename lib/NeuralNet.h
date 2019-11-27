#include <vector>
#include <iostream>
#include "BackProp.h"

class NeuralNet{
	BackProp *bp = nullptr;
	// vector of stuct{userid, imgdata} signatures
	struct _signature{unsigned uid; double *imgdata;};
	std::vector <struct _signature> signatures;  
	// set of string names; userid = array pos
	std::vector <std::string> names;
	unsigned insertName(std::string name);

public:
	void insert(const char *imgpath); 		// push to respective databases
	void test(const char *imgpath); 		// prints JSON of probabilities
	void train();							// processs data into the NN
	~NeuralNet();
	// NeuralNet(const char *openpath); 	// load from file
	// void save(const char *savepath); 	// saves to file
};