class BackProp{
	unsigned 
		NL,				// number of layers (input, middle, output)
		*lsz;			// size of each layer
	double
		alpha,			// alpha - the "momentum"
		eta,			// eta - the "learning rate"
		**out,			// activation of each neuron
		**delta,		// delta - the error value for each neuron
		***weight,		// weights of each link
		***prevwt;		// previous weights


public:
	BackProp(const unsigned, unsigned*, double, double);
	~BackProp();
	void bpgt(double*, double*); 		// backpropogates error for one set of input (training)
	void ffwd(double*);					// feeds forward activations for one set of inputs	
	double outval(unsigned) const;		// returns i'th output of the net
	double mse(double*) const;			// returns the mean square error of the net	
	// BackProp(string filename);		// load from file
	// bool save(string);				// save to file
};