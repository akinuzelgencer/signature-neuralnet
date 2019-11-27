#include <iostream>
#include <vector>
using namespace std;
#include <opencv2/opencv.hpp>
using namespace cv;

#include "Image.h"

// dirty hack
#include "imgcpp/debug.cpp"
#include "imgcpp/features.cpp"
#include "imgcpp/preprocess.cpp"

double* imgextactdata(const char *path){

	// will fill and return this
	double *ans = new double[IMGDATALEN]{0}; 
	
	// ===== STEP1: read grayscale and convert B&W binary ===== 
	// read grayscale
	Mat img = imread(path,IMREAD_GRAYSCALE);
	if (!img.data){cerr<<"No image data\n"; exit(1);}
	// convert to white-black binary
	img = img < THRESHOLD;
	if(DEBUGGING) showImage(img,"White & Black Original");


	// ===== STEP2: compute rotation angle by finding medians ===== 
	Vec4f xAxis;
	double rotAngle = findTiltedAxis(img, xAxis);
	if(DEBUGGING) showImageWithLine(img,xAxis,"X-Axis Line");


	// ===== STEP3: rotate, crop and stretch ===== 
	// --- 1. rotate to median angle
	rotateImg(img,rotAngle);
	if(DEBUGGING) showImage(img,"Rotated Image");
	// --- 2. crop to bounding box
	if(cropImg(img)){cerr<<"No image content\n"; exit(1);}
	if(DEBUGGING) showImage(img,"Cropped Image");
	ans[0] = img.cols/(double)img.rows; // HxW ratio
	// --- 3. stretch to HEIGHTxWIDTH
	stretchImg(img,WIDTH,HEIGHT); 
	if(DEBUGGING) showImage(img,"Stretched Image");


	// ===== STEP4: feature extraction ===== 
	vector<double> ftr;  // basic features
	ftr.reserve(4*(HEIGHT+WIDTH)); 
	inkVertArr(img,ftr);
	inkHorzArr(img,ftr);
	meanVertArr(img,ftr);
	meanHorzArr(img,ftr);
	boundUpArr(img,ftr);
	boundDownArr(img,ftr);
	boundLeftArr(img,ftr);
	boundRightArr(img,ftr);
	smoothenArr(ans+2, ftr, SMOOTHNESS); // smoothen the feaures
	ans[1] = countNonZero(img)/(double)(HEIGHT*WIDTH); // ink ratio

	// ===== exit ===== 
	return ans;
}