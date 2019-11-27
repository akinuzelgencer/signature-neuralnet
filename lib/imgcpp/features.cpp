// tested okay

void inkVertArr(Mat &img, vector<double>&arr){
	for(auto j=0; j<img.cols; j++)
		arr.push_back( countNonZero(img.col(j)) / (double)HEIGHT );
}
void inkHorzArr(Mat &img, vector<double>&arr){
	for(auto i=0; i<img.rows; i++)
		arr.push_back( countNonZero(img.row(i)) / (double)WIDTH );
}
void meanVertArr(Mat &img, vector<double>&arr){
	long unsigned sum;
	vector<Point> pts;
	for(auto j=0; j<img.cols; j++){
		findNonZero(img.col(j),pts);
		if(pts.size()){
			sum = 0;
			for(auto pt:pts) sum+=pt.y;
			arr.push_back( sum/(double)(HEIGHT*pts.size()) );
		}else arr.push_back(-1);
	}
}
void meanHorzArr(Mat &img, vector<double>&arr){
	long unsigned sum;
	vector<Point> pts;
	for(auto i=0; i<img.rows; i++){
		findNonZero(img.row(i),pts);
		if(pts.size()){
			sum=0;
			for(auto pt:pts) sum+=pt.x;
			arr.push_back( sum/(double)(WIDTH*pts.size()) );
		}else arr.push_back(-1);
	}
}
void boundUpArr(Mat &img, vector<double>&arr){
	for(int j=0; j<img.cols; j++){
		int i=0;
		while(i<img.rows && img.at<uchar>(i++,j)==0);
		arr.push_back( i==img.rows?-1: ((i-1)/(double)(HEIGHT)) );
	}
}
void boundDownArr(Mat &img, vector<double>&arr){
	for(int j=0; j<img.cols; j++){
		int i=img.rows;
		while(--i>=0 && img.at<uchar>(i,j)==0);
		arr.push_back(i<0?-1: (i/(double)(HEIGHT)) );
	}
}
void boundLeftArr(Mat &img, vector<double>&arr){
	for(int i=0; i<img.rows; i++){
		int j=0;
		while(j<img.cols && img.at<uchar>(i,j++)==0);
		arr.push_back(j==img.cols?-1: ((j-1)/(double)(WIDTH)) );
	}	
}
void boundRightArr(Mat &img, vector<double>&arr){
	for(int i=0; i<img.rows; i++){
		int j=img.cols;
		while(--j>=0 && img.at<uchar>(i,j)==0);
		arr.push_back(j<0?-1: (j/(double)(WIDTH)) );
	}
}