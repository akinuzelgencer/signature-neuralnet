#include <iostream>
using namespace std;
#include "NeuralNet.h"
#include "Image.h"

unsigned NeuralNet::insertName(string name){

	// compare each element in the vector
	for(unsigned i=this->names.size(); i--;)
		if(this->names[i] == name) return i;

	// if not found push_back
	this->names.push_back(name);
	return (this->names.size()-1);

}


void NeuralNet::insert(const char *imgpath){
	if(FILE *fp = fopen(imgpath,"r")) fclose(fp);
	else {fprintf(stderr,"Image \"%s\" not found!\n",imgpath); exit(-1);}

	// --- STEP1: name = extract_username(filename) ---
	string name(imgpath);
	auto n1 = name.find_last_of("/\\") + 1, n2 = name.find_last_of('-');
	name = name.substr(n1, n2-n1);

	// --- STEP2: id = namesDB.insert(name) ---
	unsigned uid = this->insertName(name);

	// --- STEP3: data = extract_imgdata(filename) ---
	double *imgdata = imgextactdata(imgpath);

	// --- STEP4: signaturesDB.insert(id,data) ---
	this->signatures.push_back({uid,imgdata});

}


void NeuralNet::test(const char *imgpath){

	// -- STEP1: signature = extract_imgdata(filename) ---
	double *signature = imgextactdata(imgpath);

	// -- STEP2: bp.ffwd(signature) ---
	this->bp->ffwd(signature);

	// -- STEP3: (print JSON) probability[users] = bp.Out(i) ---
	printf("\"%s\": {\n", imgpath);
	for(int i=this->names.size();i--;)
		printf("\t\"%s\": %lf,\n",this->names[i].c_str(), this->bp->outval(i));
	printf("},\n");

	// cleanup
	delete[] signature;
}


void NeuralNet::train(){

	// config data
	const double 
		alpha = 0.1, 
		eta = 0.3,
		threshold =  0.00001;
	const unsigned
		iterations = 30000,
		hidden_nodes = 30;

	// --- STEP1: bp = new BackProp(|sizes|, sizes, beta, alpha) 
	// 	where sizes = {|datasize|,|hidden|,|users|}
	unsigned *sizes=new unsigned[3]{IMGDATALEN, hidden_nodes, (unsigned)this->names.size()};
	this->bp = new BackProp(3, sizes, eta, alpha);
	delete[] sizes;

	// --- STEP2: answer[signatures]x[userid] = {is_signature_i_from_userid_j}
	double **answer = new double* [this->signatures.size()];
	for(int i=this->signatures.size(); i--; ){
		answer[i] = new double[this->names.size()] {0};
		answer[i][this->signatures[i].uid] = 1;
	}

	// --- STEP3: BackPropagation	
	for(unsigned i=0; i<iterations; i++){							// for(i=0; i<iterations; i++)
		unsigned j = i%(this->signatures.size());					// 	j = i%|signatures|;
		this->bp->bpgt(this->signatures[j].imgdata, answer[j]);		// 	bp.bpgt(signature[j].data, answer[j])
		double MSE = this->bp->mse(answer[j]);						// 	MSE = bp.mse(answer[j])
		if(MSE < threshold) break;									//  if(MSE < threshold) break
	}

	// cleanup
	for(int i=this->signatures.size(); i--;) delete[] answer[i];
	delete[] answer;

}

NeuralNet::~NeuralNet(){
	delete bp;
	for(auto &v: signatures) delete v.imgdata;
	names.clear();
	signatures.clear();
}