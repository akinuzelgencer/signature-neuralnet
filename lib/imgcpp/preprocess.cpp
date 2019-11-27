// tested okay

double findTiltedAxis(const Mat &img, Vec4f &axis){
	vector<Point> median; 						// to hold the medians
	median.reserve(img.cols); 					// atmost |img.cols| medians
	for(auto i=0; i<img.cols; i++){

		auto N = countNonZero(img.col(i)); 		// how many non-zero elements
		if(N==0) continue; 						// skip empty columns
		auto k = N/2 - (N&1?0:-1), j=0; 		// how many non-zeros to cross

		while(k) if(img.at<uchar>(++j,i)) --k;	// jump across that many non-zeroes

		Point p(i,j);							// position of median
		if(!N&1){								// if(N==even) 
			while(img.at<uchar>(++j,i));		// 		median=avg(j, next_j)
			p.y = (p.y + j)/2;
		}
		median.push_back(p);
	}
	fitLine(median,axis,DIST_L2,0,0.01,0.01);
	if(DEBUGGING==4) for(auto v:median) cout<<v<<endl;
	return (atan(axis[1]/axis[0])*180.0/M_PI);
}

char rotateImg(Mat &src, const double angle){
	// https://stackoverflow.com/a/24352524
	// get rotation matrix for rotating the image around its center in pixel coordinates
	Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
	Mat rot = getRotationMatrix2D(center, angle, 1.0);
	Rect2f bbox = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0,2) += bbox.width/2.0 - src.cols/2.0;
	rot.at<double>(1,2) += bbox.height/2.0 - src.rows/2.0;
	// save
	Mat dst;
	warpAffine(src, dst, rot, bbox.size());
	rot.release();
	src.release();
	src = dst;
	return 0;
}

char cropImg(Mat &src){
	// !maybedo: forEach optimize
	vector<Point> lst;
	findNonZero(src,lst);
	if(lst.empty()) return -1;
	unsigned xMin, xMax, yMin, yMax;
	xMin=xMax=lst[0].x;
	yMin=yMax=lst[0].y;
	for(auto e:lst){
		if(e.x<xMin) xMin=e.x; else if(e.x>xMax) xMax=e.x;
		if(e.y<yMin) yMin=e.y; else if(e.y>yMax) yMax=e.y;
	}		
	Rect cbox(xMin,yMin,xMax-xMin+1,yMax-yMin+1);
	src = (src(cbox));
	return 0;
}

char stretchImg(Mat &src, const unsigned width, const unsigned height){
	Mat dst;
	resize(src,dst,Size(width,height));
	src.release();
	src = (dst > 64);
	return 0;
}

/*
char compressImg(Mat &src){
	// !maybedo: remove potential race condition
	Mat dst = Mat::zeros(src.rows,src.cols/8,0);
	src.forEach<uchar>([&dst](uchar &v, const int *p) -> void {
		if(v) dst.at<uchar>(p[0],p[1]/8) |= (1<<(7-p[1]%8)); 
		// &row = pos[0], &col=pos[1];
	});
	// !maybedo: determine if src=dest is equivalent to this
	src.release();
	src = dst.clone();
	dst.release();
	return 0;
}
*/


template <typename type>
type vecmedian(vector<type> lst){
	size_t N = lst.size();
	if (N == 0) return 0; // undefined
	sort(lst.begin(), lst.end());
	if (N%2) return lst[N/2];
	else return (lst[N/2 - 1] + lst[N/2])/2;
}
template <typename type>
double vecmean(vector<type> lst){
	size_t N = lst.size();
	if (N == 0) return 0; // undefined
	double mean=0;
	for(auto x:lst) mean+=(x/N);
	return mean;
}



void smoothenArr(double *dst, vector<double> src, const char factor){
	// median of blocks of size <factor>
	vector<double> lst;
	lst.reserve(factor);
	const auto N=src.size();
	for(auto i=0, j=0; i<N; j++){
		for(auto k=0; k<factor; i++, k++) if(src[i]>=0) lst.push_back(src[i]);
		dst[j] = vecmedian<double>(lst);//vecmean<double>(lst);
		lst.clear();
	}
}