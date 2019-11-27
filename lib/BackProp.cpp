#include <math.h> // for exp()
#include <iostream> // for RAND_MAX
#include <string.h> // for memcpy

#include "BackProp.h"


BackProp::BackProp(const unsigned N, unsigned *sizes, double e, double a){
	
	this->eta = e; // eta - the "learning rate"
	this->alpha = a; // alpha - the "momentum"
	this->NL = N; // number of layers (input, middle, output)
	this->lsz = new unsigned [N]; // size of each layer
	memcpy(lsz, sizes, N*sizeof(unsigned));
	
	this->out = new double* [N]; // output of each neuron
	this->delta = new double* [N]; // delta - the error value for each neuron
	this->weight = new double** [N]; // weights of each link
	this->prevwt = new double** [N]; // previous weights

	out[0] = new double [lsz[0]]; // for layer 0
	// delta, weight, prevwt do not need space for layer 0 because
	// the input layer only stores the input and thus there is no 
	//  weigth or delta associated with it:

	// ∀ layers
	for(int i = N; --i; ){ 	
		out[i] = new double [lsz[i]];
		delta[i] = new double [lsz[i]];
		weight[i] = new double* [lsz[i]];
		prevwt[i] = new double* [lsz[i]];
		// ∀ neurons
		for(int j = lsz[i]; j-- ; ){
			weight[i][j] = new double [lsz[i-1]+1];
			prevwt[i][j] = new double [lsz[i-1]+1]{0};
		}
	}

	// Generate random weights to initialize NN
	srand(time(NULL));
	for(int i = 1; i<N; i++) // ∀ layers
		for(int j = 0; j<lsz[i]; j++) // ∀ neurons
			for(int k = 0; k < lsz[i-1]+1; k++) // ∀ links
				weight[i][j][k]= (double) rand()/(RAND_MAX/2) - 1;
				// every neuron of a layer connects to
				// all neurons of the next layer

}


BackProp::~BackProp() {
	for(int i=NL; --i;){
		for(int j=lsz[i]; j--;){
			delete[] weight[i][j];
			delete[] prevwt[i][j];
		}
		delete[] weight[i];
		delete[] prevwt[i];
		delete[] out[i];
		delete[] delta[i];
	}
	delete[] out[0];
	delete[] lsz;
	delete[] out;
	delete[] delta;
	delete[] weight;
	delete[] prevwt;
}


double BackProp::outval(unsigned i) const {
	return this->out[NL-1][i];
}

// ============== feed forward one set of input =================
void BackProp::ffwd(double *in) {
	
	//	assign content to input layer
	memcpy(out[0], in, lsz[0]*sizeof(double));

	double sum;
	//	assign activation value to each neuron 
	// ∀ layers
	for(int ii = 1; ii < NL; ii++) {
		// ∀ neurons
		for(int jj = 0; jj < lsz[ii]; jj++) {
			sum = weight[ii][jj][lsz[ii-1]]; // bias
			// ∀ links
			for(int kk = 0; kk < lsz[ii-1]; kk++)
				sum += out[ii-1][kk] * weight[ii][jj][kk]; // weights
			out[ii][jj] = (1.0 / (1.0 + exp(-sum))); // sigmoid fn
		}
	}
}

// ============= backpropogate errors from the output layer =============
void BackProp::bpgt(double *data, double *correctoutput) {

	//	update output values for each neuron
	this->ffwd(data);

	// delta for output layer
	for(int ii = 0; ii < lsz[NL-1]; ii++) {
		delta[NL-1][ii] = 
			out[NL-1][ii] *
			(1-out[NL-1][ii]) * 
			(correctoutput[ii] - out[NL-1][ii]);
	}

	// delta for hidden layers	
	double sum;
	for(int ii = NL-2; ii > 0; ii--) {
		for(int jj = 0; jj < lsz[ii]; jj++) {
			sum = 0.0;
			for(int kk = 0; kk < lsz[ii+1]; kk++)
				sum += delta[ii+1][kk]*weight[ii+1][kk][jj];
			delta[ii][jj] = out[ii][jj]*(1-out[ii][jj])*sum;
		}
	}

	//	apply momentum 
	if(alpha!=0){
		for(int ii = 1; ii < NL; ii++) {
			for(int jj = 0; jj < lsz[ii]; jj++) {
				for(int kk = 0; kk < lsz[ii-1]; kk++)
					weight[ii][jj][kk] += alpha*prevwt[ii][jj][kk];
				weight[ii][jj][lsz[ii-1]] += alpha*prevwt[ii][jj][lsz[ii-1]];
			}
		}
	}

	//	adjust weights (apply eta) 
	for(int ii = 1; ii < NL; ii++) {
		for(int jj = 0; jj < lsz[ii]; jj++) {
			for(int kk = 0; kk < lsz[ii-1]; kk++) {
				prevwt[ii][jj][kk] = eta*delta[ii][jj]*out[ii-1][kk];
				weight[ii][jj][kk] += prevwt[ii][jj][kk];
			}
			prevwt[ii][jj][lsz[ii-1]] = eta*delta[ii][jj];
			weight[ii][jj][lsz[ii-1]] += prevwt[ii][jj][lsz[ii-1]];
		}
	}
}

double BackProp::mse(double *tgt) const {
	double mse = 0;
	for(int i = lsz[NL-1]; i--; ) mse += pow(tgt[i]-out[NL-1][i],2);
	return mse/2;
}

// ============= archived, untested ==============
/*
BackProp::BackProp(string filePath)
{
	int ii, jj, kk;
	ifstream file(filepath.c_str());
	
	if (nLayers != 0) {	// overwriting net
		freeWeights();
		delete[] layerSize;
	}
	
	// Read configuration
	file >> nLayers;
	layerSize = new int [nLayers];
	file.ignore();
	
	file >> eta;
	file.ignore();
	file >> alpha;
	file.ignore();
	
	for (ii = 0; ii < nLayers; ii++) {
		file >> layerSize[ii];
		file.ignore();
	}
	file.ignore();
	
	// Read the weights 
	allocateWeights();
	for(int ii = 1; ii < nLayers; ii++)		// layers
		for(jj = 0; jj < layerSize[ii]; jj++)	// neurons
			for(kk = 0; kk < layerSize[ii-1]+1; kk++) {	// links
				file >> weight[ii][jj][kk];
				file.ignore();
			}
	
	
	file.close();
	
	load(filePath);
}



 
bool BackProp::save(string filepath) {
	int ii, jj, kk;
	ofstream file(filepath.c_str());
	
	
	// Write configuration
	file << nLayers << endl << eta << endl << alpha << endl;
	for (ii = 0; ii < nLayers; ii++)
		file << layerSize[ii] << " ";	// " " will have to be ignored
	file << endl;	// endl will also have to be ignored i.e. ignore.in(1)
	
	// Write the weights  
	for(int ii = 1; ii < nLayers; ii++)		// layers
		for(jj = 0; jj < layerSize[ii]; jj++)	// neurons
			for(kk = 0; kk < layerSize[ii-1]+1; kk++)	// links
				file << weight[ii][jj][kk] << " "; // " " again
	
	file.close();
	return true;
}

bool BackProp::load(string filepath)
{
	int ii, jj, kk;
	ifstream file(filepath.c_str());
	
	if (nLayers != 0) {	// overwriting net
		freeWeights();
		delete[] layerSize;
	}
	
	// Read configuration 
	file >> nLayers;
	layerSize = new int [nLayers];
	file.ignore();
	
	file >> eta;
	file.ignore();
	file >> alpha;
	file.ignore();
	
	for (ii = 0; ii < nLayers; ii++) {
		file >> layerSize[ii];
		file.ignore();
	}
	file.ignore();
	
	// Read the weights 
	allocateWeights();
	for(int ii = 1; ii < nLayers; ii++)		// layers
		for(jj = 0; jj < layerSize[ii]; jj++)	// neurons
			for(kk = 0; kk < layerSize[ii-1]+1; kk++) {	// links
				file >> weight[ii][jj][kk];
				file.ignore();
			}
	
	
	file.close();
	return true;
}
*/