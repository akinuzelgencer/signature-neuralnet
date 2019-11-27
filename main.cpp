#include "lib/NeuralNet.h"
#include <stdio.h>

void fulltest();

int main(){
	fulltest();
	// NeuralNet Ann;
	// Ann.insert("path_to_your_png");
	// Ann.train();
	// Ann.test("path_to_your_png");
}


void fulltest(){
	unsigned i;
	char buff[64];
	NeuralNet Ann;
	for(i=1; i<=15; i++){
		sprintf(buff,"dataset/train/Amy-%3.3u.png", i); Ann.insert(buff);
		sprintf(buff,"dataset/train/Bob-%3.3u.png", i); Ann.insert(buff);
		sprintf(buff,"dataset/train/Eva-%3.3u.png", i); Ann.insert(buff);
	}
	Ann.train();
	for(i=1; i<=250; i++){sprintf(buff,"dataset/test/Amy-%3.3u.png", i); Ann.test(buff);}
	for(i=1; i<= 99; i++){sprintf(buff,"dataset/test/Bob-%3.3u.png", i); Ann.test(buff);}
	for(i=1; i<=100; i++){sprintf(buff,"dataset/test/Eva-%3.3u.png", i); Ann.test(buff);}
}